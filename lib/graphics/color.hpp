//
// Created by James Pickering on 5/23/25.
//

#pragma once

#include <glm/vec4.hpp>

namespace color {

constexpr auto rgb(int r, int g, int b) -> glm::vec4 {
    return glm::vec4((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, 1.f);
}

constexpr auto rgba(int r, int g, int b, float a) -> glm::vec4 {
    return glm::vec4((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, a);
}

constexpr auto creme = rgb(228, 224, 202);
constexpr auto dark = rgb(23, 25, 14);
constexpr auto turquoise = rgb(96, 151, 149);
constexpr auto green = rgb(33, 58, 39);
constexpr auto grey = rgb(120, 152, 135);
constexpr auto orange = rgb(160, 116, 70);
constexpr auto yellow = rgb(190, 185, 129);
}




