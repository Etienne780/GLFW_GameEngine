#include "Vector3.h"

std::string Vector3::toString() const {
    return FormatUtils::formatString("[{}, {}, {}]", x, y, z);
}

void Vector3::normalize() {
    float len = magnitude();
    if (len > 0) {
        *this /= len;
    }
}

float Vector3::magnitude() const {
    return std::sqrt(squaredMagnitude());
}

float Vector3::squaredMagnitude() const {
    return (x * x + y * y + z * z);
}

float Vector3::dot(const Vector3& other) const {
    return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 Vector3::lerp(const Vector3& other, float t) const {
    return Vector3(
        MathUtil::Lerp(x, other.x, t),
        MathUtil::Lerp(y, other.y, t),
        MathUtil::Lerp(z, other.z, t)
    );
}

// Adds another vector to the current vector
Vector3& Vector3::operator+=(const Vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3& Vector3::operator+=(float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
}

Vector3& Vector3::operator-=(float scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar) {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not possible");
    }
    x /= scalar; 
    y /= scalar;
    z /= scalar;
    return *this;
}

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator+(float scalar) const {
    return Vector3(x + scalar, y + scalar, z + scalar);
}

Vector3 Vector3::operator-(float scalar) const {
    return Vector3(x - scalar, y - scalar, z - scalar);
}

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero is not possible");
    }
    return Vector3(x / scalar, y / scalar, z / scalar);
}