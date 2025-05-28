//
// Created by James Pickering on 5/15/25.
//

#include "style.hpp"

#include "entity.hpp"

const glm::vec4 Style::DefaultColor = {0, 0, 0, 1};
const float Style::DefaultFloat = 1.f;
const CursorType Style::DefaultCursorType = CursorType::Pointer;

template <typename T>
auto getType() -> DataType {
    if constexpr (std::is_same<T, float>::value) {
        return DataType::Float;
    }
    if constexpr (std::is_same<T, glm::vec4>::value) {
        return DataType::Vec4;
    }
    if constexpr (std::is_same<T, CursorType>::value) {
        return DataType::CursorType;
    }
    throw std::runtime_error("getType(): unsupported type");
}

template auto getType<float>() -> DataType;
template auto getType<glm::vec4>() -> DataType;
template auto getType<CursorType>() -> DataType;

template <typename T>
auto getDefaultValue() -> const T& {
    if constexpr (std::is_same_v<T, float>) {
        return Style::DefaultFloat;
    } else if constexpr (std::is_same_v<T, glm::vec4>) {
        return Style::DefaultColor;
    } else if constexpr (std::is_same_v<T, CursorType>) {
        return Style::DefaultCursorType;
    }
    throw std::runtime_error("getDefaultValue(): unsupported type");
}

template auto getDefaultValue<float>() -> const float&;
template auto getDefaultValue<glm::vec4>() -> const glm::vec4&;
template auto getDefaultValue<CursorType>() -> const CursorType&;

template <typename T>
auto setStyleAttr(Style& style, Attr attr, int state, const T& val) -> void {
    const auto type = getType<T>();
    const auto map = static_cast<std::unordered_map<int, T>*>(getMap(style, type));

    (*map)[(int)attr * All + state] = val;
}

template auto setStyleAttr<float>(Style&, Attr, int, const float&) -> void;
template auto setStyleAttr<glm::vec4>(Style&, Attr, int, const glm::vec4&) -> void;
template auto setStyleAttr<CursorType>(Style&, Attr, int, const CursorType&) -> void;

template <typename T>
auto getStyleAttr(const Style& style, Attr attr, int state) -> T {
    const auto type = getType<T>();
    const auto map = static_cast<const std::unordered_map<int, T>*>(getMap(style, type));

    const auto baseKey = (int)attr * All;
    const auto& val = [&state, map, &baseKey] {
        if (state & Depressed) {
            if (map->contains(baseKey + Depressed)) {
                return map->at(baseKey + Depressed);
            }
        }

        if (state & Hovered) {
            if (map->contains(baseKey + Hovered)) {
                return map->at(baseKey + Hovered);
            }
        }

        if (state & Selected) {
            if (map->contains(baseKey + Selected)) {
                return map->at(baseKey + Selected);
            }
        }

        if (map->contains(baseKey)) {
            return map->at(baseKey);
        }

        return getDefaultValue<T>();
    }();

    if constexpr (std::is_same_v<T, glm::vec4>) {
        if (state & Depressed) {
            return val * 0.8f;
        }
    }

    return val;
}

template auto getStyleAttr<float>(const Style&, Attr, int) -> float;
template auto getStyleAttr<glm::vec4>(const Style&, Attr, int) -> glm::vec4;
template auto getStyleAttr<CursorType>(const Style&, Attr, int) -> CursorType;

auto applyStyle(Style& style, const StyleClass type) -> void {
    switch (type) {
    case StyleClass::Default:
        setStyleAttr(style, Attr::BorderColor, Hovered, glm::vec4{230.f / 255.f, 170.f / 255.f, 95.f / 255.f, 1.f});
        setStyleAttr(style, Attr::BorderColor, Selected, glm::vec4{1.f, 0.f, 0.f, 1.f});
        setStyleAttr(style, Attr::BorderWidth, Hovered, 4.f);
        setStyleAttr(style, Attr::BorderWidth, Selected, 4.f);
        setStyleAttr(style, Attr::BorderWidth, Depressed, 8.f);
        setStyleAttr(style, Attr::CursorType, Hovered, CursorType::Pointer);
        break;
    case StyleClass::Button:
        setStyleAttr(style, Attr::CursorType, Normal, CursorType::Pointer);
        break;
    case StyleClass::Resizable:
        setStyleAttr(style, Attr::CursorType, Normal, CursorType::Expand);
        setStyleAttr(style, Attr::CursorType, Depressed, CursorType::Fist);
    default:
        throw std::runtime_error("applyStyle(): unsupported style type");
    }
}