#include "Vector2.h"

std::string Vector2::toString() const {
    return FormatUtils::formatString("[{}, {}]", x, y);
}

void Vector2::normalize() {
    float len = magnitude();
    if (len > 0) {
        *this /= len;
    }
}

float Vector2::magnitude() const {
    return std::sqrt(squaredMagnitude());
}

float Vector2::squaredMagnitude() const {
    return (x * x + y * y);
}

float Vector2::dot(const Vector2& other) const {
    return (x * other.x) + (y * other.y);
}

float Vector2::cross(const Vector2& other) const {
    return x * other.y - y * other.x;
}

Vector2 Vector2::lerp(const Vector2& vec, float t) const {
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