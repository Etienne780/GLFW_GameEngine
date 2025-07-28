#include "Vector4.h"

std::string Vector4::toString() const {
    return FormatUtils::formatString("[{}, {}, {}, {}]", x, y, z, w);
}

void Vector4::normalize() {
    float len = magnitude();
    if (len > 0) {
        *this /= len;
    }
}

float Vector4::magnitude() const {
    return std::sqrt(squaredMagnitude());
}

float Vector4::squaredMagnitude() const {
    return x * x + y * y + z * z + w * w;
}

Vector4 Vector4::lerp(const Vector4& vec, float t) const {
    return Vector4(
        MathUtil::Lerp(x, vec.x, t),
        MathUtil::Lerp(y, vec.y, t),
        MathUtil::Lerp(z, vec.z, t),
        MathUtil::Lerp(w, vec.w, t)
    );
}

Vector4& Vector4::operator+=(const Vector4& other) {
    x += other.x; y += other.y;
    z += other.z; w += other.w;
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& other) {
    x -= other.x; y -= other.y;
    z -= other.z; w -= other.w;
    return *this;
}

Vector4& Vector4::operator*=(float scalar) {
    x *= scalar; y *= scalar;
    z *= scalar; w *= scalar;
    return *this;
}

Vector4& Vector4::operator/=(float scalar) {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not possible");
    }
    x /= scalar; y /= scalar;
    z /= scalar; w /= scalar;
    return *this;
}

Vector4 Vector4::operator+(const Vector4& other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

// Subtracts two vectors and returns the result
Vector4 Vector4::operator-(const Vector4& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

// Scales the vector with a scalar and returns the result
Vector4 Vector4::operator*(float scalar) const {
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

// Divides the vector by a scalar and returns the result
Vector4 Vector4::operator/(float scalar) const {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not allowed");
    }
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}