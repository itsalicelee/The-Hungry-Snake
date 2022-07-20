#ifndef DSAP_CUSTOM_CONTROLLER_H
#define DSAP_CUSTOM_CONTROLLER_H

#include "ISnakeController.h"
#include "DirectionType.h"
#include "Snake.h"
#include <vector>
#include <algorithm>
#include <time.h>
#include <random>
#include <cassert>

class CustomController : public ISnakeController {
public:
    
    DirectionType NextDirection(const Game&, size_t);
    DirectionType DecideTurnDirection(const Game&, size_t);
    const Position& SnakeHead(const Game& game, const size_t id){return game.Snakes().find(id)->second.Head();};
    const std::list<Position> SnakeBodyList(const Game& game, const size_t id){return game.Snakes().find(id)->second.Body();};
    const Position& MyHead(const Game& game){return game.Center();};
    const Snake Me(const Game& game){return game.Snakes().find(1)->second;};
    
    
private:
    enum class DirectionSymbol {
        RIGHT, DOWN, LEFT, UP, NONE
    };
    DirectionType _turnDirection;
    DirectionType _currentDirType;
    
    float _final_angle = 0;
    float turn_radius = 310;
    float accum_angle = 360;
    
    DirectionSymbol _dirSymbol;
    DirectionSymbol AngleToSymbol(float);
    float GetCollisionDistance(Position, DirectionSymbol, const Game&, size_t);
    float GetFrontCollisionDistance(Position, float, DirectionSymbol, Position, float);
    float FrontWallDistance(Position, DirectionSymbol, float, float);
};


DirectionType CustomController::NextDirection(const Game& game, size_t id) {
    // Print angle, direction type, and symbol
    int H = game.FieldHeight();
    int W = game.FieldWidth();
    const auto& snake = game.Snakes().at(id);
    Position pos = snake.Head();
    
    // if is turning around
    // keep turning
    if (_currentDirType != DirectionType::kForward) {
        
        float remaining_angle = abs(_final_angle - snake.Direction());
        if (remaining_angle > 0) {  //still turning
            return _currentDirType;
        }
        // finished turning
        _dirSymbol = AngleToSymbol(snake.Direction());
        
    }

    float distance = GetCollisionDistance(snake.Head(), _dirSymbol, game, id);
    if (distance > 0 && distance < turn_radius) {  // if danger!
        // start turning around
        _turnDirection = DecideTurnDirection(game, id);
        _currentDirType = _turnDirection;
        if (_currentDirType == DirectionType::kRight) {
            _final_angle = snake.Direction() + 90;
        }
        else {
            _final_angle = snake.Direction() - 90;
        }
    }
    else {  // no danger
        float cur_angle = abs(snake.Direction());
        while(cur_angle >= accum_angle){
            cur_angle -= accum_angle;
        }
        if( cur_angle <= 1e-2 && snake.Direction() != 0 && _currentDirType == DirectionType::kForward){
            turn_radius += 50;
            accum_angle += 360;
        }
        // no collision problem, just go straight forward
        _currentDirType = DirectionType::kForward;
    }
    return _currentDirType;
}  // end of NextDirection


DirectionType CustomController::DecideTurnDirection(const Game& game, size_t id){
    const auto& snake = game.Snakes().at(id);
    float dist_u = GetCollisionDistance(snake.Head(), DirectionSymbol::UP, game, id);
    float dist_d = GetCollisionDistance(snake.Head(), DirectionSymbol::DOWN, game, id);
    float dist_r = GetCollisionDistance(snake.Head(), DirectionSymbol::RIGHT, game, id);
    float dist_l = GetCollisionDistance(snake.Head(), DirectionSymbol::LEFT, game, id);
    if(_dirSymbol == DirectionSymbol::UP){
        if(dist_r > dist_l){
            return DirectionType::kRight;
        }
        else{
            return DirectionType::kLeft;
        }
    }
    else if(_dirSymbol == DirectionSymbol::DOWN){
        if(dist_l > dist_r){
            return DirectionType::kRight;
        }
        else{
            return DirectionType::kLeft;
        }
    }
    else if(_dirSymbol == DirectionSymbol::LEFT){
        if(dist_u > dist_d){
            return DirectionType::kRight;
        }
        else{
            return DirectionType::kLeft;
        }
    }
    else if(_dirSymbol == DirectionSymbol::RIGHT){
        if(dist_d > dist_u){
            return DirectionType::kRight;
        }
        else{
            return DirectionType::kLeft;
        }
    }
    else{
        std::cout << "NONE!" << std::endl;
    }
}


CustomController::DirectionSymbol CustomController::AngleToSymbol(float angle) {
    while(angle < 0){
        angle += 360;
    }
    
    // if angle is not a multiple of 90
    if (int(angle) % 90 != 0) {
        return DirectionSymbol::NONE;
    }
    // can be converted into 4 directions
    int dir = abs(angle / 90);
    dir %= 4;
    return static_cast<DirectionSymbol>(dir);
}


float CustomController::GetCollisionDistance(Position snakePos, DirectionSymbol dirSymbol, const Game& game, size_t id) {
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


float CustomController::GetFrontCollisionDistance(Position snakePos, float snakeRadius, DirectionSymbol dirSymbol, Position target, float targetRadius) {
    float distanceX = abs(snakePos.x - target.x) - snakeRadius - targetRadius;
    float distanceY = abs(snakePos.y - target.y) - snakeRadius - targetRadius;
    
    // if direction is Left/Right
    if(dirSymbol == DirectionSymbol::LEFT) {
        if(distanceY > 0 || (snakePos.x - target.x <= 0)){
            return -1;
        }
        return distanceX;
    }
    if (dirSymbol == DirectionSymbol::RIGHT) {
        if (distanceY > 0 || (snakePos.x - target.x > 0)) { // if will not hit target y, return -1
            return -1;
        }
        return distanceX;
    }
    
    // if direction is Up/Down
    if (dirSymbol == DirectionSymbol::UP) {
        if (distanceX > 0 || (snakePos.y - target.y <= 0)) { // if will not hit target x, return -1
            return -1;
        }
        return distanceY;
    }
    if (dirSymbol == DirectionSymbol::DOWN) {
        if (distanceX > 0 || (snakePos.y - target.y > 0)) { // if will not hit target x, return -1
            return -1;
        }
        return distanceY;
    }
    return -1;
}


float CustomController::FrontWallDistance(Position snakeHead, DirectionSymbol dirSymbol, float rightWall, float downWall) {
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

#endif // DSAP_CUSTOM_CONTROLLER_H



