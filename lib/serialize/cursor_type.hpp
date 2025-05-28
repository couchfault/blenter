//
// Created by Henry Pitcairn on 5/28/25.
//

#pragma once
#include <ostream>
#include <istream>
#include "../graphics/cursor_type.hpp"


auto operator<<(std::ostream& os, CursorType cursor) -> std::ostream& {
    auto value = static_cast<std::int32_t>(cursor);
    os.write(reinterpret_cast<const char*>(&value), sizeof(std::int32_t));
    return os;
}

auto operator>>(std::istream& is, CursorType& cursor) -> std::istream& {
    std::int32_t value;
    is.read(reinterpret_cast<char*>(&value), sizeof(std::int32_t));
    cursor = static_cast<CursorType>(value);
    return is;
}
