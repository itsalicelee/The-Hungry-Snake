#ifndef DSAP_I_SNAKE_CONTROLLER_H
#define DSAP_I_SNAKE_CONTROLLER_H

#include "DirectionType.h"
#include "Game.h"

struct ISnakeController {
    virtual DirectionType NextDirection(const Game& game, size_t id) = 0;
    virtual ~ISnakeController() = default;
};

#endif //DSAP_I_SNAKE_CONTROLLER_H
