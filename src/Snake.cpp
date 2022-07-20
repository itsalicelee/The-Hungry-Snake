#define _USE_MATH_DEFINES

#include <cmath>

#include "Snake.h"

Snake::Snake(size_t id, const Position& pos, float dir, size_t len) {
    length_ = len;
    id_ = id;
    direction_ = dir;
    int body_size = static_cast<int>(length_) * Snake::kSegmentSize + 1;
    for (int i = body_size; i >= 0; --i) {
        body_.push_back(pos);
    }
}

void Snake::StepLeft() {
    direction_--;
    StepForward();
}

void Snake::StepForward() {
    body_.push_back(body_.back() + Forward());
    body_.pop_front();
}

void Snake::StepRight() {
    direction_++;
    StepForward();
}


void Snake::AddScores(int added) {
    scores_ += added;
    length_ = scores_;
    size_t body_size = length_ * Snake::kSegmentSize + 1;
    Position tail = body_.front();
    while (body_size > body_.size()) {
        body_.push_front(tail);
    }
}

Position Snake::Forward() const {
    return Position{
            static_cast<float>(2. * std::cos(direction_ / 180.f * M_PI)),
            static_cast<float>(2. * std::sin(direction_ / 180.f * M_PI))};
}
