#include "ConversionUtils.h"
#include "Math.h"

int ConversionUtils::HexToIntegral(const std::string& hex) {
    int result = 0;
    std::stringstream ss;

    if (hex.rfind("0x", 0) == 0 || hex.rfind("0X", 0) == 0)
        ss << std::hex << hex.substr(2);
    else if (hex.rfind("#", 0) == 0)
        ss << std::hex << hex.substr(1);
    else
        ss << std::hex << hex;

    ss >> result;
    return result;
}

float ConversionUtils::ToDegrees(float radians) {
    return radians * (180 / M_PI);
}

float ConversionUtils::ToRadians(float degrees) {
    return degrees * (M_PI / 180);
}