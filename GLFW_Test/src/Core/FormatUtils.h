#pragma once
#include <string>
#include <type_traits>

using String = std::string;

namespace format {

    template<typename T>
    struct always_false : std::false_type {};

    template<typename T>
    String toString(T value);

    template<typename T>
    String toString(T value) {
        if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(value);
        }
        else if constexpr (std::is_same_v<T, const char*>) {
            return String(value);
        }
        else if constexpr (std::is_same_v<T, String>) {
            return value;
        }
        else if constexpr (std::is_same_v<T, bool>) {
            return (value) ? "true" : "false";
        }
        else {
            static_assert(always_false<T>::value, "Unsupported type for toString");
        }
    }
}