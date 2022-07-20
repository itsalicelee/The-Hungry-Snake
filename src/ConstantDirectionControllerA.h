#ifndef DSAP_CONSTANT_DIRECTION_CONTROLLER_A_H
#define DSAP_CONSTANT_DIRECTION_CONTROLLER_A_H

#include "ISnakeController.h"

class ConstantDirectionControllerA : public ISnakeController {
public:
    explicit ConstantDirectionControllerA(DirectionType type) : type_{type} {}
    DirectionType NextDirection(const Game&, size_t) override {
        return type_;
    }
private:
    DirectionType type_;
};

#endif // DSAP_CONSTANT_DIRECTION_CONTROLLER_A_H
