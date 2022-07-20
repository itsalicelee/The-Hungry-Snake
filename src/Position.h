#ifndef DSAP_POSITION_H
#define DSAP_POSITION_H

#include <cmath>

struct Position {
    [[nodiscard]] float Length() const {
        return std::sqrt(x*x+y*y);
    }

    [[nodiscard]] float InnerProduct(Position position) const {
        return x * position.x + y* position.y;
    }

    float x, y;
};

Position operator+(const Position& lhs, const Position& rhs);
Position operator-(const Position& lhs, const Position& rhs);

#endif // DSAP_POSITION_H