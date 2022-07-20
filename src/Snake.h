#ifndef DSAP_SNAKE_H
#define DSAP_SNAKE_H

#include <list>
#include "Position.h"

class Snake {
public:
    static const int kSegmentSize = 20;

    Snake() = default;
    Snake(size_t id, const Position& pos, float dir, size_t len);

    void StepForward();
    void StepLeft();
    void StepRight();

    [[nodiscard]] float Direction() const { return direction_; }

    [[nodiscard]] const Position& Head() const { return body_.back(); }

    [[nodiscard]] const std::list<Position>& Body() const { return body_; }

    void AddScores(int added);

    [[nodiscard]] Position Forward() const;

    [[nodiscard]] size_t Id() const { return id_; }

    [[nodiscard]] int Scores() const { return scores_; }

private:
    float direction_ = 0;
    std::list<Position> body_;
    int scores_ = 0;
    size_t length_ = 0;
    size_t id_ = 0;
};

#endif // DSAP_SNAKE_H