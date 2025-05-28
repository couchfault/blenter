//
// Created by James Pickering on 5/15/25.
//

#pragma once

#include "cursor_type.hpp"

#include <glm/vec4.hpp>

#include <unordered_map>

enum class Attr {
    FillColor,
    BorderColor,
    BorderWidth,
    CursorType,
};

enum class DataType {
    Vec4,
    Float,
    CursorType
};

struct Style {
    static const glm::vec4 DefaultColor;
    static const float DefaultFloat;
    static const CursorType DefaultCursorType;

    std::unordered_map<int, float> floats;
    std::unordered_map<int, glm::vec4> vec4s;
    std::unordered_map<int, CursorType> cursorTypes;
};

template <typename T>
auto getType() -> DataType;
template <typename T>
auto getDefaultValue() -> const T&;
template <typename T>
auto setStyleAttr(Style& style, Attr attr, int state, const T& val) -> void;
template <typename T>
auto getStyleAttr(const Style& style, Attr attr, int state) -> T;

inline auto getMap(Style& style, const DataType type) -> void* {
    switch (type) {
    case DataType::Float:
        return &style.floats;
    case DataType::Vec4:
        return &style.vec4s;
    case DataType::CursorType:
        return &style.cursorTypes;
    default:
        throw std::invalid_argument("getMap(): Invalid DataType");
    }
}

inline auto getMap(const Style& style, const DataType type) -> const void* {
    return getMap(const_cast<Style&>(style), type);
}

enum class StyleClass {
    Default,
    Button,
    Resizable,
};

auto applyStyle(Style& style, StyleClass type) -> void;