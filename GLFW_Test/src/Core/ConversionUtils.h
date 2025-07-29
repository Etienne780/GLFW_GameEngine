#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>
#include <bitset>
#include <limits>

using String = std::string;

/**
 * @brief Provides utility functions for common data format conversions.
 */
class ConversionUtils {
public:
    /**
     * @brief Converts a hexadecimal string to an integer.
     *
     * Supports optional "0x" or "0X" prefix.
     *
     * @param hex The hex string to convert.
     * @return The corresponding integer value.
     */
    static int HexToIntegral(const std::string& hex);

    /**
     * @brief Converts an integral value to a hexadecimal string.
     *
     * @tparam T An integral type (e.g., int, uint32_t).
     * @param num The value to convert.
     * @param withPrefix If true, includes the "0x" prefix.
     * @return A string representing the value in hexadecimal format.
     */
    template<typename T>
    static String IntegralToHex(T num, bool withPrefix) {
        static_assert(std::is_integral<T>::value, "IntegralToHex requires integral types");

        std::ostringstream stream;
        stream << std::hex << std::uppercase << static_cast<uint64_t>(num);

        return withPrefix ? "0x" + stream.str() : stream.str();
    }

    /**
     * @brief Converts an integral value to a binary string.
     *
     * @tparam T An integral type (e.g., int, uint32_t).
     * @param num The value to convert.
     * @param withPrefix If true, includes the "0b" prefix.
     * @return A string representing the value in binary format.
     */
    template<typename T>
    static String IntegralToBinary(T num, bool withPrefix) {
        static_assert(std::is_integral<T>::value, "IntegralToBinary requires integral types");

        constexpr size_t bits = sizeof(T) * 8;
        std::bitset<bits> binary(static_cast<uint64_t>(num));

        // Remove leading zeroes
        String binStr = binary.to_string();
        size_t firstOne = binStr.find('1');
        if (firstOne != String::npos) {
            binStr = binStr.substr(firstOne);
        }
        else {
            binStr = "0";
        }

        return withPrefix ? "0b" + binStr : binStr;
    }
};