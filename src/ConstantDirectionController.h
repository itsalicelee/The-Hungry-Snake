#ifndef DSAP_CONSTANT_DIRECTION_CONTROLLER_H
#define DSAP_CONSTANT_DIRECTION_CONTROLLER_H

#include "ISnakeController.h"

class ConstantDirectionController : public ISnakeController {
public:
    explicit ConstantDirectionController(DirectionType type) : type_{type} {}
    DirectionType NextDirection(const Game&, size_t) override {
        return type_;
    }
private:
    DirectionType type_;
};

#endif // DSAP_CONSTANT_DIRECTION_CONTROLLER_H