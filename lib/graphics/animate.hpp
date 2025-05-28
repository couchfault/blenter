//
// Created by James Pickering on 5/27/25.
//

#pragma once

#include <chrono>
#include <functional>

using namespace std::chrono;

enum class Fn {
    EaseExpo
};

struct Animation {
    high_resolution_clock::time_point start;
    float duration = 1000.f;
    Fn fn = Fn::EaseExpo;
    bool running = false;
    bool forward = true;
    float t = 0.f;
    float startT = 0.f;
};

auto callFn(Fn fn, float t, bool forward) -> float;

auto easeInExp(float t) -> float;
auto easeOutExp(float t) -> float;
