#pragma once
#include "Math/Vector3.h"

/**
 * @brief Utility class for color space conversions and operations.
 */
class ColorUtils {
public:
    /**
     * @brief Converts HSV (Hue, Saturation, Value) to RGB color space.
     * @param h Hue angle in degrees [0, 360)
     * @param s Saturation [0, 1]
     * @param v Value (brightness) [0, 1]
     * @return RGB color as Vector3 in [0, 1]
     */
    static Vector3 HSVtoRGB(float h, float s, float v);

    /**
     * @brief Converts RGB to HSV color space.
     * @param r Red component [0, 1]
     * @param g Green component [0, 1]
     * @param b Blue component [0, 1]
     * @return HSV color as Vector3: (H in degrees [0,360), S and V in [0,1])
     */
    static Vector3 RGBtoHSV(float r, float g, float b);

    /**
     * @brief Linearly interpolates between two RGB colors.
     * @param a Start color
     * @param b End color
     * @param t Interpolation factor [0, 1]
     * @return Interpolated RGB color
     */
    static Vector3 LerpRGB(const Vector3& a, const Vector3& b, float t);

    /**
     * @brief Clamps a color to the range [0, 1] per component.
     * @param color Input color
     * @return Clamped color
     */
    static Vector3 ClampColor(const Vector3& color);
};