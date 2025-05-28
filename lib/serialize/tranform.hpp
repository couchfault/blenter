//
// Created by Henry Pitcairn on 5/28/25.
//

#pragma once
#include <glm/glm.hpp>
#include <ostream>
#include <istream>
#include "glm_vec.hpp"
#include "glm_mat.hpp"
#include "../graphics/entity.hpp"

auto operator<<(std::ostream& os, const Transform& transform) -> std::ostream& {
    os << transform.pos << transform.size << transform.rot;
    return os;
}

auto operator>>(std::istream& is, Transform& transform) -> std::istream& {
    is >> transform.pos >> transform.size >> transform.rot;
    return is;
}
