#ifndef DSAP_PLAYER_CONTROLLER_H
#define DSAP_PLAYER_CONTROLLER_H

#include "ISnakeController.h"

class PlayerController : public ISnakeController {
public:
    DirectionType NextDirection(const Game&, size_t) override {
        return playerDirection_;
    }
    void KeepForward() {
        playerDirection_ = DirectionType::kForward;
    }
    void TurnLeft() {
        playerDirection_ = DirectionType::kLeft;
    }
    void TurnRight() {
        playerDirection_ = DirectionType::kRight;
    }

private:
    DirectionType playerDirection_ = DirectionType::kForward;
};

#endif // DSAP_PLAYER_CONTROLLER_H