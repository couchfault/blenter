#pragma once

#include <type_traits>

template <typename T>
concept UIntEnumC = std::is_enum_v<T> &&
                    std::is_unsigned_v<std::underlying_type_t<T>> &&
                    std::is_convertible_v<std::underlying_type_t<T>, unsigned int>;

template <UIntEnumC T>
auto operator|(T lhs, T rhs) -> T {
    return static_cast<T>(
        static_cast<std::underlying_type_t<T>>(lhs) |
        static_cast<std::underlying_type_t<T>>(rhs));
}

template <UIntEnumC T>
auto operator|=(T& lhs, T rhs) -> T& {
    return lhs = lhs | rhs;
}

template <UIntEnumC T>
auto operator~(const T& lhs) -> T {
    return static_cast<T>(~static_cast<std::underlying_type_t<T>>(lhs));
}

template <UIntEnumC T>
auto operator&(T lhs, T rhs) -> T {
    return static_cast<T>(
        static_cast<std::underlying_type_t<T>>(lhs) &
        static_cast<std::underlying_type_t<T>>(rhs));
}

template <UIntEnumC T>
auto operator&=(T& lhs, T rhs) -> T& {
    return lhs = lhs & rhs;
}