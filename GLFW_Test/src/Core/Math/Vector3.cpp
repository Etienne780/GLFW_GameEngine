#include "Vector3.h"

#include <stdexcept>
#include <cmath>

#include "Matrix.h"
#include "MathUtils.h"

#include "..\FormatUtils.h"

std::string Vector3::ToString() const {
    return FormatUtils::formatString("[{}, {}, {}]", x, y, z);
}

void Vector3::Normalize() {
    float len = Magnitude();
    if (len > 0) {
        *this /= len;
    }
}

float Vector3::Magnitude() const {
    return std::sqrt(SquaredMagnitude());
}

float Vector3::SquaredMagnitude() const {
    return (x * x + y * y + z * z);
}

float Vector3::Dot(const Vector3& other) const {
    return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 Vector3::Cross(const Vector3 & other) const {
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

Vector3 Vector3::Lerp(const Vector3& other, float t) const {
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


float& Vector3::operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    throw std::out_of_range("Vector3 index out of range");
}

const float& Vector3::operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    throw std::out_of_range("Vector3 index out of range");
}

Vector3::operator Matrix<float>() const {
    return Matrix<float>({
        {x},
        {y},
        {z}
    });
}