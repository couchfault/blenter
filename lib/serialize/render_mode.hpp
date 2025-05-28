//
// Created by Henry Pitcairn on 5/28/25.
//


#pragma once
#include <ostream>
#include <istream>
#include "../graphics/entity.hpp"
#include "glm_vec.hpp"
#include "glm_mat.hpp"


auto operator<<(std::ostream& os, RenderMode mode) -> std::ostream& {
    auto value = static_cast<std::int32_t>(mode);
    os.write(reinterpret_cast<const char*>(&value), sizeof(std::int32_t));
    return os;
}

auto operator>>(std::istream& is, RenderMode& mode) -> std::istream& {
    std::int32_t value;
    is.read(reinterpret_cast<char*>(&value), sizeof(std::int32_t));
    mode = static_cast<RenderMode>(value);
    return is;
}
