//
// Created by Henry Pitcairn on 5/28/25.
//

#pragma once
#include <ostream>
#include <istream>
#include "../graphics/style.hpp"
#include "cursor_type.hpp"
#include "glm_vec.hpp"
#include "glm_mat.hpp"


auto operator<<(std::ostream& os, const Style& style) -> std::ostream& {
    auto size = static_cast<std::uint32_t>(style.floats.size());
    os.write(reinterpret_cast<const char*>(&size), sizeof(std::uint32_t));
    for (const auto& [key, value] : style.floats) {
        os.write(reinterpret_cast<const char*>(&key), sizeof(int));
        os.write(reinterpret_cast<const char*>(&value), sizeof(float));
    }

    size = static_cast<std::uint32_t>(style.vec4s.size());
    os.write(reinterpret_cast<const char*>(&size), sizeof(std::uint32_t));
    for (const auto& [key, value] : style.vec4s) {
        os.write(reinterpret_cast<const char*>(&key), sizeof(int));
        os << value;
    }

    size = static_cast<std::uint32_t>(style.cursorTypes.size());
    os.write(reinterpret_cast<const char*>(&size), sizeof(std::uint32_t));
    for (const auto& [key, value] : style.cursorTypes) {
        os.write(reinterpret_cast<const char*>(&key), sizeof(int));
        os << value;
    }

    return os;
}

auto operator>>(std::istream& is, Style& style) -> std::istream& {
    style.floats.clear();
    style.vec4s.clear();
    style.cursorTypes.clear();

    std::uint32_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(std::uint32_t));
    for (std::uint32_t i = 0; i < size; ++i) {
        int key;
        float value;
        is.read(reinterpret_cast<char*>(&key), sizeof(int));
        is.read(reinterpret_cast<char*>(&value), sizeof(float));
        style.floats[key] = value;
    }

    is.read(reinterpret_cast<char*>(&size), sizeof(std::uint32_t));
    for (std::uint32_t i = 0; i < size; ++i) {
        int key;
        glm::vec4 value;
        is.read(reinterpret_cast<char*>(&key), sizeof(int));
        is >> value;
        style.vec4s[key] = value;
    }

    is.read(reinterpret_cast<char*>(&size), sizeof(std::uint32_t));
    for (std::uint32_t i = 0; i < size; ++i) {
        int key;
        CursorType value;
        is.read(reinterpret_cast<char*>(&key), sizeof(int));
        is >> value;
        style.cursorTypes[key] = value;
    }

    return is;
}
