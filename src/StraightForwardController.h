#ifndef DSAP_STRAIGHT_CONTROLLER_H
#define DSAP_STRAIGHT_CONTROLLER_H

#include "ISnakeController.h"

class StraightForwardController : public ISnakeController {
public:
    explicit StraightForwardController(float init_dir, DirectionType turnDirection) :
        _final_angle { init_dir },
        _turnDirection { turnDirection },
        _currentDirType { DirectionType::kForward },
        _dirSymbol { AngleToSymbol(_final_angle) } {}
    DirectionType NextDirection(const Game&, size_t) override;
private:
    enum class DirectionSymbol {
        RIGHT, DOWN, LEFT, UP, NONE
    };
    DirectionType _turnDirection;
    DirectionType _currentDirType;
    float _final_angle;
    DirectionSymbol _dirSymbol;
    const float turn_radius =  3 * 180 / 3.1415926 + 30;

    DirectionSymbol AngleToSymbol(float);
    float GetCollisionDistance(Position, DirectionSymbol, const Game&, size_t);
    float GetFrontCollisionDistance(Position, float, DirectionSymbol, Position, float);
    float FrontWallDistance(Position, DirectionSymbol, float, float);
    
};

#endif 
