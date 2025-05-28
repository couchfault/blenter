//
// Created by Henry Pitcairn on 5/28/25.
//

#pragma once
#include <ostream>
#include <istream>
#include <glm/glm.hpp>

auto operator<<(std::ostream& os, const glm::mat2& mat) -> std::ostream& {
    for (int i = 0; i < 2; ++i) {
        os.write(reinterpret_cast<const char*>(&mat[i].x), sizeof(float));
        os.write(reinterpret_cast<const char*>(&mat[i].y), sizeof(float));
    }
    return os;
}

auto operator>>(std::istream& is, glm::mat2& mat) -> std::istream& {
    for (int i = 0; i < 2; ++i) {
        is.read(reinterpret_cast<char*>(&mat[i].x), sizeof(float));
        is.read(reinterpret_cast<char*>(&mat[i].y), sizeof(float));
    }
    return is;
}

auto operator<<(std::ostream& os, const glm::mat3& mat) -> std::ostream& {
    for (int i = 0; i < 3; ++i) {
        os.write(reinterpret_cast<const char*>(&mat[i].x), sizeof(float));
        os.write(reinterpret_cast<const char*>(&mat[i].y), sizeof(float));
        os.write(reinterpret_cast<const char*>(&mat[i].z), sizeof(float));
    }
    return os;
}

auto operator>>(std::istream& is, glm::mat3& mat) -> std::istream& {
    for (int i = 0; i < 3; ++i) {
        is.read(reinterpret_cast<char*>(&mat[i].x), sizeof(float));
        is.read(reinterpret_cast<char*>(&mat[i].y), sizeof(float));
        is.read(reinterpret_cast<char*>(&mat[i].z), sizeof(float));
    }
    return is;
}

auto operator<<(std::ostream& os, const glm::mat4& mat) -> std::ostream& {
    for (int i = 0; i < 4; ++i) {
        os.write(reinterpret_cast<const char*>(&mat[i].x), sizeof(float));
        os.write(reinterpret_cast<const char*>(&mat[i].y), sizeof(float));
        os.write(reinterpret_cast<const char*>(&mat[i].z), sizeof(float));
        os.write(reinterpret_cast<const char*>(&mat[i].w), sizeof(float));
    }
    return os;
}

auto operator>>(std::istream& is, glm::mat4& mat) -> std::istream& {
    for (int i = 0; i < 4; ++i) {
        is.read(reinterpret_cast<char*>(&mat[i].x), sizeof(float));
        is.read(reinterpret_cast<char*>(&mat[i].y), sizeof(float));
        is.read(reinterpret_cast<char*>(&mat[i].z), sizeof(float));
        is.read(reinterpret_cast<char*>(&mat[i].w), sizeof(float));
    }
    return is;
}
