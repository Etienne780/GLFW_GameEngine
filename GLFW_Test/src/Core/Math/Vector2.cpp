#include "Vector2.h"

#include <cmath>

#include "Matrix.h"
#include "MathUtils.h"

#include "..\FormatUtils.h"


std::string Vector2::ToString() const {
    return FormatUtils::formatString("[{}, {}]", x, y);
}

void Vector2::Normalize() {
    float len = Magnitude();
    if (len > 0) {
        *this /= len;
    }
}

float Vector2::Magnitude() const {
    return std::sqrt(SquaredMagnitude());
}

float Vector2::SquaredMagnitude() const {
    return (x * x + y * y);
}

float Vector2::Dot(const Vector2& other) const {
    return (x * other.x) + (y * other.y);
}

float Vector2::Cross(const Vector2& other) const {
    return x * other.y - y * other.x;
}

Vector2 Vector2::Lerp(const Vector2& vec, float t) const {
    return Vector2(
        MathUtil::Lerp(x, vec.x, t), 
        MathUtil::Lerp(y, vec.y, t) 
    );
}

Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2& Vector2::operator+=(float scalar) {
    x += scalar;
    y += scalar;
    return *this;
}

Vector2& Vector2::operator-=(float scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
}

Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar) {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not possible");
    }
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator+(float scalar) const {
    return Vector2(x + scalar, y + scalar);
}

Vector2 Vector2::operator-(float scalar) const {
    return Vector2(x - scalar, y - scalar);
}

Vector2 Vector2::operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar) const {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not possible");
    }
    return Vector2(x / scalar, y / scalar);
}

float& Vector2::operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    throw std::out_of_range("Vector2 index out of range");
}

const float& Vector2::operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    throw std::out_of_range("Vector2 index out of range");
}

Vector2::operator Matrix<float>() const {
    return Matrix<float>({
        {x},
        {y}
    });
}