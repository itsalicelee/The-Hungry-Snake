#ifndef DSAP_GAME_H
#define DSAP_GAME_H

#include <memory>
#include <map>
#include <vector>

#include "Position.h"
#include "Snake.h"

struct ISnakeController;

class Game {
public:
    static const int kFoodRadius = 5;
    static const int kSnakeRadius = 50;
    static const int kCellSize = 100;

    Game(size_t number_of_rows, size_t number_of_cols, size_t timeLimit);

    void RunUntilTheEnd() {
        while (!IsOver()) Step();
    }

    void AddFood(const Position& pos) {
        foods_.push_back(pos);
    }

    void AddSnake(size_t id, const Position& pos, float dir, size_t len) {
        snakes_[id] = Snake(id, pos, dir, len);
    }

    void AddController(size_t id, const std::shared_ptr<ISnakeController>& controller) {
        controllers[id] = controller;
    }

    void Step();

    [[nodiscard]] const Position& Center() const {
        return snakes_.at(1).Head();
    }

    template<typename UnaryFunction> void TraverseFoods(UnaryFunction f) const;
    template<typename UnaryFunction> void TraverseSnakes(UnaryFunction f) const;

    [[nodiscard]] const std::vector<Position>& Foods() const { return foods_; }
    [[nodiscard]] const std::map<size_t, Snake>& Snakes() const { return snakes_; };

    [[nodiscard]] size_t NumberOfCols() const;
    [[nodiscard]] size_t NumberOfRows() const;

    [[nodiscard]] size_t FieldWidth() const {
        return NumberOfCols() * kCellSize;
    }

    [[nodiscard]] size_t FieldHeight() const {
        return NumberOfRows() * kCellSize;
    }

    [[nodiscard]] bool IsOver() const;

    [[nodiscard]] int Scores() const;

    [[nodiscard]] size_t Time() const;

    static bool IsCollidedWithCircle(
            const Position &center_a, int radius_a,
            const Position &center_b, int radius_b);

    static bool IsCollidedWithRectangle(
            const Position &position, int radius,
            Position topLeft, Position bottomRight);

private:
    std::map<size_t, Snake> snakes_;
    std::map<size_t, std::shared_ptr<ISnakeController>> controllers;
    std::vector<Position> foods_;
    bool is_game_over_ = false;

    size_t number_of_rows_;
    size_t number_of_cols_;
    size_t timeLimit_;
    size_t ticks_;


};

template<typename Func>
void Game::TraverseFoods(Func f) const {
    for (const auto& food : foods_) {
        f(food);
    }
}

template<typename Func>
void Game::TraverseSnakes(Func f) const {
    for (auto it = snakes_.begin(); it != snakes_.end(); ++it) {
        f(it->second);
    }
}

#endif // DSAP_GAME_H