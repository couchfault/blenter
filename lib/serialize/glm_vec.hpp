//
// Created by Henry Pitcairn on 5/28/25.
//

#pragma once

#include <ostream>
#include <istream>
#include <glm/glm.hpp>

auto operator<<(std::ostream& os, const glm::vec1& vec) -> std::ostream& {
    os.write(reinterpret_cast<const char*>(&vec.x), sizeof(float));
    return os;
}

auto operator>>(std::istream& is, glm::vec1& vec) -> std::istream& {
    is.read(reinterpret_cast<char*>(&vec.x), sizeof(float));
    return is;
}

auto operator<<(std::ostream& os, const glm::vec2& vec) -> std::ostream& {
    os.write(reinterpret_cast<const char*>(&vec.x), sizeof(float));
    os.write(reinterpret_cast<const char*>(&vec.y), sizeof(float));
    return os;
}

auto operator>>(std::istream& is, glm::vec2& vec) -> std::istream& {
    is.read(reinterpret_cast<char*>(&vec.x), sizeof(float));
    is.read(reinterpret_cast<char*>(&vec.y), sizeof(float));
    return is;
}

auto operator<<(std::ostream& os, const glm::vec3& vec) -> std::ostream& {
    os.write(reinterpret_cast<const char*>(&vec.x), sizeof(float));
    os.write(reinterpret_cast<const char*>(&vec.y), sizeof(float));
    os.write(reinterpret_cast<const char*>(&vec.z), sizeof(float));
    return os;
}

auto operator>>(std::istream& is, glm::vec3& vec) -> std::istream& {
    is.read(reinterpret_cast<char*>(&vec.x), sizeof(float));
    is.read(reinterpret_cast<char*>(&vec.y), sizeof(float));
    is.read(reinterpret_cast<char*>(&vec.z), sizeof(float));
    return is;
}

auto operator<<(std::ostream& os, const glm::vec4& vec) -> std::ostream& {
    os.write(reinterpret_cast<const char*>(&vec.x), sizeof(float));
    os.write(reinterpret_cast<const char*>(&vec.y), sizeof(float));
    os.write(reinterpret_cast<const char*>(&vec.z), sizeof(float));
    os.write(reinterpret_cast<const char*>(&vec.w), sizeof(float));
    return os;
}

auto operator>>(std::istream& is, glm::vec4& vec) -> std::istream& {
    is.read(reinterpret_cast<char*>(&vec.x), sizeof(float));
    is.read(reinterpret_cast<char*>(&vec.y), sizeof(float));
    is.read(reinterpret_cast<char*>(&vec.z), sizeof(float));
    is.read(reinterpret_cast<char*>(&vec.w), sizeof(float));
    return is;
}
