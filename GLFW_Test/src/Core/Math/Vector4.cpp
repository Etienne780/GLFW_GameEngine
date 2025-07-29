#include "Vector4.h"
#include <stdexcept>
#include <cmath>

#include "Matrix.h"
#include "MathUtils.h"

#include "..\FormatUtils.h"

std::string Vector4::ToString() const {
    return FormatUtils::formatString("[{}, {}, {}, {}]", x, y, z, w);
}

void Vector4::Normalize() {
    float len = Magnitude();
    if (len > 0) {
        *this /= len;
    }
}

float Vector4::Magnitude() const {
    return std::sqrt(SquaredMagnitude());
}

float Vector4::SquaredMagnitude() const {
    return x * x + y * y + z * z + w * w;
}

float Vector4::Dot(const Vector4& other) const {
    return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

Vector4 Vector4::Lerp(const Vector4& vec, float t) const {
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

Vector4& Vector4::operator+=(float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    w += scalar;
    return *this;
}

Vector4& Vector4::operator-=(float scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    w -= scalar;
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

Vector4 Vector4::operator-(const Vector4& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator+(float scalar) const {
    return Vector4(x + scalar, y + scalar, z + scalar, w + scalar);
}

Vector4 Vector4::operator-(float scalar) const {
    return Vector4(x - scalar, y - scalar, z - scalar, w - scalar);
}

Vector4 Vector4::operator*(float scalar) const {
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4 Vector4::operator/(float scalar) const {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not allowed");
    }
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

float& Vector4::operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    if (index == 3) return w;
    throw std::out_of_range("Vector4 index out of range");
}

const float& Vector4::operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    if (index == 3) return w;
    throw std::out_of_range("Vector4 index out of range");
}

Vector4::operator Matrix<float>() const {
    return Matrix<float>({
        {x},
        {y},
        {z},
        {w}
    });
}