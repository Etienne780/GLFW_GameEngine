#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>

using String = std::string;

class FormatUtils {
public:
    template<typename T, std::size_t N>
    static String arrayToString(const T(&arr)[N]) {
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
    static String toString(const char(&arr)[N]) {
        return String(arr);
    }

    template<typename T>
    static String toString(T value) {
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
    static String joinArgs(Args&&... args) {
        return joinArgsImpl(", ", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static String joinArgsSeperator(const String& separator, Args&&... args) {
        return joinArgsImpl(separator, std::forward<Args>(args)...);
    }

    /// Base case for formatString – returns unchanged format string.
    static String formatString(const String& format) {
        return format;
    }

    /// Recursively formats a string by replacing each "{}" with corresponding argument.
    template<typename T, typename... Args>
    static String formatString(const String& format, T&& value, Args&&... args) {
        return formatStringImpl(0, format, std::forward<T>(value), std::forward<Args>(args)...);
    }
private:
    template<typename T>
    struct always_false : std::false_type {};

    template<typename... Args>
    static String joinArgsImpl(const String& separator, Args&&... args) {
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

    /// Internal implementation of formatString with recursion depth.
    template<typename T, typename... Args>
    static String formatStringImpl(int depth, const String& format, T&& value, Args&&... args) {
        size_t pos = format.find("{}");
        if (pos == String::npos) {
            if (depth == 0) {
                // No placeholders found: fallback to concatenated args.
                return FormatUtils::joinArgs(format, std::forward<T>(value), std::forward<Args>(args)...);
            }
            return format;
        }

        String result = format.substr(0, pos)
            + FormatUtils::toString(std::forward<T>(value))
            + formatStringImpl(1, format.substr(pos + 2), std::forward<Args>(args)...);
        return result;
    }

    /// Base case for formatStringImpl – returns remaining format unchanged.
    static String formatStringImpl(int, const String& format) {
        return format;
    }
};