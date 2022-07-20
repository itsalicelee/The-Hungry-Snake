#ifndef DSAP_CONSTANT_DIRECTION_CONTROLLER_B_H
#define DSAP_CONSTANT_DIRECTION_CONTROLLER_B_H

#include "ISnakeController.h"

class ConstantDirectionControllerB : public ISnakeController {
public:
    DirectionType NextDirection(const Game&, size_t id) override {
		if (type_ == DirectionType::kForward) {
			type_ = DirectionType::kRight;
		} else {
			type_ = DirectionType::kForward;
		}
        return type_;
    }
private:
    DirectionType type_ = DirectionType::kRight;
};

#endif // DSAP_CONSTANT_DIRECTION_CONTROLLER_H
