#include <random>
#include <unordered_set>
#include <map>
#include <cmath>
#include "ISnakeController.h"
#include "DirectionType.h"

Game::Game(size_t number_of_rows, size_t number_of_cols, size_t timeLimit) :
        number_of_rows_{number_of_rows},
        number_of_cols_{number_of_cols},
        timeLimit_{timeLimit},
        ticks_{0} {}

void Game::Step() {
    if (is_game_over_) return;

    ++ticks_;

    if (timeLimit_ <= ticks_) {
        is_game_over_ = true;
        return;
    }

    // Move
    for (auto it = snakes_.begin(); it != snakes_.end(); ++it) {
        size_t id = it->first;
        Snake& snake = it->second;
        if (controllers.count(id)) {
            switch (controllers[id]->NextDirection(*this, id)) {
                case DirectionType::kLeft:
                    snake.StepLeft();
                    break;
                case DirectionType::kRight:
                    snake.StepRight();
                    break;
                case DirectionType::kForward:
                    snake.StepForward();
                    break;
            }
        }
    }

    // Dead
    std::unordered_set<size_t> dead_ids;
    for (auto it1 = snakes_.begin(); it1 != snakes_.end(); ++it1) {
        const size_t id1 = it1->first;
        const auto& snake1 = it1->second;
        const auto& forward = snake1.Forward();

        // Snake to Field
        if (IsCollidedWithRectangle(
                snake1.Head(), Game::kSnakeRadius,
                Position{0, 0},
                Position{
                        static_cast<float>(FieldWidth()),
                        static_cast<float>(FieldHeight())}
        )) {
            dead_ids.insert(id1);
            break;
        }

        for (auto it2 = snakes_.begin(); it2 != snakes_.end(); ++it2) {
            const size_t id2 = it2->first;
            const auto& snake2 = it2->second;

            if (id1 == id2) continue;

            // Snake to Snake
            if (IsCollidedWithCircle(snake1.Head(), Game::kSnakeRadius,
                                     snake2.Head(), Game::kSnakeRadius)) {
                const Position distance = snake2.Head() - snake1.Head();
                const float innerProduct = distance.InnerProduct(forward);
                if (innerProduct > 0) {
                    dead_ids.insert(id1);
                }
                break;
            }
            for (const Position& pos : snake2.Body()) {
                if (IsCollidedWithCircle(snake1.Head(), Game::kSnakeRadius,
                                         pos, Game::kSnakeRadius)) {
                    dead_ids.insert(id1);
                    break;
                }
            }
        }
    }

    if (!dead_ids.empty()) {
        if (dead_ids.find(1) != dead_ids.end()) {
            is_game_over_ = true;
            return;
        }

        std::map<size_t, Snake> newSnakes;
        for (auto it = snakes_.begin(); it != snakes_.end(); ++it) {
            const size_t id = it->first;
            Snake& snake = it->second;
            if (dead_ids.find(id) == dead_ids.end()) {
                newSnakes[id] = std::move(snake);
            } else {
                const int kPeriod = 20;
                int currIndex = 0;
                for (const auto& p : snake.Body()) {
                    if (currIndex % kPeriod == 0) {
                        foods_.push_back(p);
                    }
                    currIndex++;
                }
            }
        }
        snakes_ = std::move(newSnakes);
    }

    // Eat
    std::vector<Position> new_foods;
    for (auto & food : foods_) {
        bool is_eaten = false;
        for (auto it = snakes_.begin(); it != snakes_.end(); ++it) {
            Snake& snake = it->second;
            if (IsCollidedWithCircle(
                    snake.Head(), Game::kSnakeRadius,
                    food, Game::kFoodRadius)) {
                snake.AddScores(1);
                is_eaten = true;
                break;
            }
        }
        if (!is_eaten) {
            new_foods.push_back(food);
        }
    }
    std::swap(new_foods, foods_);
}

bool Game::IsCollidedWithCircle(
        const Position& center_a,
        int radius_a,
        const Position &center_b,
        int radius_b) {
    return ((center_a - center_b).Length() < static_cast<float>(radius_a + radius_b));
}

size_t Game::NumberOfCols() const {
    return number_of_cols_;
}

size_t Game::NumberOfRows() const {
    return number_of_rows_;
}

bool Game::IsOver() const {
    return is_game_over_;
}

bool Game::IsCollidedWithRectangle(
        const Position &position, const int radius,
        Position topLeft, Position bottomRight) {
    auto fRadius = static_cast<float>(radius);
    return
            position.x - fRadius < topLeft.x ||
            position.x + fRadius > bottomRight.x ||
            position.y - fRadius < topLeft.y  ||
            position.y + fRadius > bottomRight.y;
}

int Game::Scores() const {
    return snakes_.at(1).Scores();
}

size_t Game::Time() const {
    return ticks_;
}
