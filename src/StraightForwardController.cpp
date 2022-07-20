#include "StraightForwardController.h"

DirectionType StraightForwardController::NextDirection(const Game& game, size_t id) {
    const auto& snake = game.Snakes().at(id);
    // if is turning around
    // keep turning
    if (_currentDirType != DirectionType::kForward) {
        float remaining_angle = abs(_final_angle - snake.Direction());
        if (remaining_angle > 0) { //still turning
            return _currentDirType;
        }
        // finished turning
        _dirSymbol = AngleToSymbol(snake.Direction());
    }

    float distance = GetCollisionDistance(snake.Head(), _dirSymbol, game, id);

    // if 0 < distance < min_distance
    if (distance > 0 && distance < turn_radius) {
        // start turning around
        _currentDirType = _turnDirection;
        if (_currentDirType == DirectionType::kRight) {
            _final_angle = snake.Direction() + 180;
        }
        else {
            _final_angle = snake.Direction() - 180;
        }
        
    }
    else {
        // no collision problem, just go straight forward
        _currentDirType = DirectionType::kForward;
    }
    
    return _currentDirType;
}

float StraightForwardController::GetCollisionDistance(Position snakePos, DirectionSymbol dirSymbol, const Game& game, size_t id) {
    
    // check front collision distance with field
    float distance = FrontWallDistance(snakePos, dirSymbol, game.FieldWidth(), game.FieldHeight());

    // check front collision distance with other snakes
    for (auto it = game.Snakes().begin(); it != game.Snakes().end(); ++it) {
        const size_t anotherID = it->first;
        const Snake& anotherSnake = it->second;
        if (anotherID == id) continue;

        float d = GetFrontCollisionDistance(snakePos, Game::kSnakeRadius * 2, dirSymbol, anotherSnake.Head(), Game::kSnakeRadius * 2);
        
        if (d > 0) {
            if (distance < 0)    distance = d;
            else {
                distance = std::min(distance, d);
            }
        }

        for (const Position& pos : anotherSnake.Body()) {
            float d_body = GetFrontCollisionDistance(snakePos, Game::kSnakeRadius, dirSymbol, pos, Game::kSnakeRadius);
            if (d_body > 0) {
                if (distance < 0)    distance = d_body;
                else {
                    distance = std::min(distance, d_body);
                }
            }
            
        }
        
    }
    return distance;
}

StraightForwardController::DirectionSymbol StraightForwardController::AngleToSymbol(float angle) {
    // if angle is not a multiple of 90
    if (int(angle) % 90 != 0) {
        return DirectionSymbol::NONE;
    }
    // can be converted into 4 directions
    int dir = abs(angle / 90);
    dir %= 4;
    return static_cast<DirectionSymbol>(dir);
}

float StraightForwardController::GetFrontCollisionDistance(Position snakePos, float snakeRadius, DirectionSymbol dirSymbol, Position target, float targetRadius) {
    float distanceX = abs(snakePos.x - target.x) - snakeRadius - targetRadius;
    float distanceY = abs(snakePos.y - target.y) - snakeRadius - targetRadius;
    
    // if direction is Left/Right
    if (dirSymbol == DirectionSymbol::LEFT || dirSymbol == DirectionSymbol::RIGHT) {
        if (distanceY > 0) { // if will not hit target y, return -1
            return -1;
        }
        return distanceX;
    }

    // if direction is Up/Down
    if (dirSymbol == DirectionSymbol::UP || dirSymbol == DirectionSymbol::DOWN) {
        if (distanceX > 0) { // if will not hit target x, return -1
            return -1;
        }
        
        return distanceY;
    }

    return -1;
}

float StraightForwardController::FrontWallDistance(Position snakeHead, DirectionSymbol dirSymbol, float rightWall, float downWall) {
    Position frontFieldCollisionPos{ 0, 0 };
    if (dirSymbol == DirectionSymbol::LEFT) {
        frontFieldCollisionPos.x = 0;
        frontFieldCollisionPos.y = snakeHead.y;
    }
    else if (dirSymbol == DirectionSymbol::RIGHT) {
        frontFieldCollisionPos.x = rightWall;
        frontFieldCollisionPos.y = snakeHead.y;
    }
    else if (dirSymbol == DirectionSymbol::UP) {
        frontFieldCollisionPos.x = snakeHead.x;
        frontFieldCollisionPos.y = 0;
    }
    else if (dirSymbol == DirectionSymbol::DOWN) {
        frontFieldCollisionPos.x = snakeHead.x;
        frontFieldCollisionPos.y = downWall;
    }
    
    return GetFrontCollisionDistance(snakeHead, Game::kSnakeRadius, dirSymbol, frontFieldCollisionPos, 0);

}
