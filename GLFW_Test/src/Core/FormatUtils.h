#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>

using String = std::string;

namespace format {

    template<typename T>
    struct always_false : std::false_type {};

    template<typename T>
    String toString(T value) {
        if constexpr (std::is_same_v<T, bool>) {
            return (value) ? "true" : "false";
        }
        else if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(value);
        }
        else if constexpr (std::is_same_v<T, const char*>) {
            return String(value);
        }
        else if constexpr (std::is_same_v<T, String>) {
            return value;
        }
        else {
            static_assert(always_false<T>::value, "Unsupported type for toString");
        }
    }

    template<typename... Args>
    static String _joinArgsImpl(const String& separator, Args&&... args) {
        std::vector<String> strings;
        (strings.push_back(toString(std::forward<Args>(args))), ...);

        std::ostringstream result;
        for (size_t i = 0; i < strings.size(); ++i) {
            result << strings[i];
            if (i != strings.size() - 1)
                result << separator;
        }
        return result.str();
    }

    template<typename... Args>
    static String joinArgs(Args&&... args) {
        return _joinArgsImpl(", ", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static String joinArgs(const String& separator, Args&&... args) {
        return _joinArgsImpl(separator, std::forward<Args>(args)...);
    }
}