//
// Created by James Pickering on 5/27/25.
//

#include "animate.hpp"

#include <cmath>

auto callFn(Fn fn, float t, bool forward) -> float {
    switch (fn) {
    case Fn::EaseExpo:
        return forward ? easeOutExp(t) : easeInExp(1 - t);
    }
    return 0.f;
}

auto easeInExp(float t) -> float {
    return t <= 0 ? 0 : std::powf(2, 10 * t - 10);
}

auto easeOutExp(const float t) -> float {
    return t > 1 ? 1.f : t < 0 ? 0.f
                               : 1.f - std::powf(2, -10 * t);
}
