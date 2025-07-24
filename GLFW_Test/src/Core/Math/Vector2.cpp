#include "Vector2.h"

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

Vector2 Vector2::lerp(const Vector2& vec, float t) const {
    return Vector2(
        MathUtil::Lerp(x, vec.x, t), 
        MathUtil::Lerp(y, vec.y, t) 
    );
}

// Adds another vector to the current vector
Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// Subtracts another vector from the current vector
Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

// Scales the vector by a scalar
Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

// Divides the vector by a scalar
Vector2& Vector2::operator/=(float scalar) {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not possible");
    }
    x /= scalar;
    y /= scalar;
    return *this;
}

// Adds two vectors and returns the result
Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

// Subtracts two vectors and returns the result
Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

// Scales the vector by a scalar and returns the result
Vector2 Vector2::operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

// Divides the vector by a scalar and returns the result
Vector2 Vector2::operator/(float scalar) const {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not possible");
    }
    return Vector2(x / scalar, y / scalar);
}