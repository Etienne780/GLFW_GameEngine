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
    String toString(T value);

    namespace detail {

        template<typename... Args>
        String joinArgsImpl(const String& separator, Args&&... args) {
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

    } // namespace detail

    template<typename T, std::size_t N>
    String arrayToString(const T(&arr)[N]) {
        std::ostringstream result;
        result << "[";
        for (std::size_t i = 0; i < N; ++i) {
            result << toString(arr[i]);
            if (i != N - 1)
                result << ", ";
        }
        result << "]";
        return result.str();
    }

    template<std::size_t N>
    String toString(const char(&arr)[N]) {
        return String(arr);
    }

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
    String joinArgs(Args&&... args) {
        return detail::joinArgsImpl(", ", std::forward<Args>(args)...);
    }

    template<typename... Args>
    String joinArgsSeperator(const String& separator, Args&&... args) {
        return detail::joinArgsImpl(separator, std::forward<Args>(args)...);
    }

}