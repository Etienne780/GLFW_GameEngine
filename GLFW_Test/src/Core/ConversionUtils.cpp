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
    return radians * (180.0f / static_cast<float>(M_PI));
}

float ConversionUtils::ToRadians(float degrees) {
    return degrees * (static_cast<float>(M_PI) / 180.0f);
}

float ConversionUtils::ToDegrees(double radians) {
    return static_cast<float>(radians * (180.0 / M_PI));
}

float ConversionUtils::ToRadians(double degrees) {
    return static_cast<float>(degrees * (M_PI / 180.0));
}

double ConversionUtils::ToDegreesDouble(float radians) {
    return static_cast<double>(radians) * (180.0 / M_PI);
}

double ConversionUtils::ToRadiansDouble(float degrees) {
    return static_cast<double>(degrees) * (M_PI / 180.0);
}

double ConversionUtils::ToDegreesDouble(double radians) {
    return radians * (180.0 / M_PI);
}

double ConversionUtils::ToRadiansDouble(double degrees) {
    return degrees * (M_PI / 180.0);
}
