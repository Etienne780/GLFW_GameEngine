#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>
#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower

#include <cmath>

class FormatUtils {
public:
    static std::string trimTrailingZeros(float value) {
        return trimTrailingZeros(static_cast<double>(value));
    }

    static std::string trimTrailingZeros(double value) {
        std::string str = toString(value);

        str.erase(str.find_last_not_of('0') + 1, std::string::npos);

        if (!str.empty() && str.back() == '.') {
            str.pop_back();
        }

        return str;
    }

    static std::string toUpperCase(std::string string) {
        std::transform(string.begin(), string.end(), string.begin(),
            [](unsigned char c) { return std::toupper(c); });

        return string;
    }

    static std::string toLowerCase(std::string string) {
        std::transform(string.begin(), string.end(), string.begin(),
            [](unsigned char c) { return std::tolower(c); });

        return string;
    }

    template<typename T, std::size_t N>
    static std::string arrayToString(const T(&arr)[N]) {
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
    static std::string toString(const char(&arr)[N]) {
        return std::string(arr);
    }

    template<typename T>
    static std::string toString(T value) {
        if constexpr (std::is_same_v<T, bool>) {
            return (value) ? "true" : "false";
        }
        else if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(value);
        }
        else if constexpr (std::is_same_v<T, const char*>) {
            return std::string(value);
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return value;
        }
        else {
            static_assert(always_false<T>::value, "Unsupported type for toString");
        }
    }

    template<typename... Args>
    static std::string joinArgs(Args&&... args) {
        return joinArgsImpl(", ", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static std::string joinArgsSeperator(const std::string& separator, Args&&... args) {
        return joinArgsImpl(separator, std::forward<Args>(args)...);
    }

    /// Base case for formatString – returns unchanged format string.
    static std::string formatString(const std::string& format) {
        return format;
    }

    /// Recursively formats a string by replacing each "{}" with corresponding argument.
    template<typename T, typename... Args>
    static std::string formatString(const std::string& format, T&& value, Args&&... args) {
        return formatStringImpl(0, format, std::forward<T>(value), std::forward<Args>(args)...);
    }
private:
    FormatUtils();

    template<typename T>
    struct always_false : std::false_type {};

    template<typename... Args>
    static std::string joinArgsImpl(const std::string& separator, Args&&... args) {
        std::vector<std::string> strings;
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
    static std::string formatStringImpl(int depth, const std::string& format, T&& value, Args&&... args) {
        size_t pos = format.find("{}");
        if (pos == std::string::npos) {
            if (depth == 0) {
                // No placeholders found: fallback to concatenated args.
                return FormatUtils::joinArgs(format, std::forward<T>(value), std::forward<Args>(args)...);
            }
            return format;
        }

        std::string result = format.substr(0, pos)
            + FormatUtils::toString(std::forward<T>(value))
            + formatStringImpl(1, format.substr(pos + 2), std::forward<Args>(args)...);
        return result;
    }

    /// Base case for formatStringImpl – returns remaining format unchanged.
    static std::string formatStringImpl(int, const std::string& format) {
        return format;
    }
};