#include "ColorUtils.h"
#include <cmath>

Vector3 ColorUtils::HSVtoRGB(float h, float s, float v) {
    float c = v * s;
    float x = c * (1.0f - std::fabs(fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float r = 0, g = 0, b = 0;
    if (0 <= h && h < 60) { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else if (h < 360) { r = c; g = 0; b = x; }

    return Vector3(r + m, g + m, b + m);
}

Vector3 ColorUtils::RGBtoHSV(float r, float g, float b) {
    float max = std::fmax(r, std::fmax(g, b));
    float min = std::fmin(r, std::fmin(g, b));
    float delta = max - min;

    float h = 0;
    if (delta != 0) {
        if (max == r) {
            h = 60.0f * fmod(((g - b) / delta), 6.0f);
        }
        else if (max == g) {
            h = 60.0f * (((b - r) / delta) + 2.0f);
        }
        else {
            h = 60.0f * (((r - g) / delta) + 4.0f);
        }
    }
    if (h < 0) h += 360.0f;

    float s = max == 0 ? 0 : delta / max;
    float v = max;

    return Vector3(h, s, v);
}

Vector3 ColorUtils::LerpRGB(const Vector3& a, const Vector3& b, float t) {
    return Vector3(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    );
}

Vector3 ColorUtils::ClampColor(const Vector3& color) {
    float r = std::fmax(0.0f, std::fmin(1.0f, color.x));
    float g = std::fmax(0.0f, std::fmin(1.0f, color.y));
    float b = std::fmax(0.0f, std::fmin(1.0f, color.z));
    return Vector3(r, g, b);
}