#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>

using String = std::string;

class ConversionUtils {
public:
    static int HexToIntegral(const std::string& hex);

    template<typename T>
    static String IntegralToHex(T num, bool withPrefix) {
        static_assert(std::is_integral<T>::value, "IntegralToHex requires integral types");

        std::ostringstream stream;
        stream << std::hex << std::uppercase << static_cast<uint64_t>(num);

        return withPrefix ? "0x" + stream.str() : stream.str();
    }
};