#include "Vector3.h"

#include <stdexcept>
#include <cmath>

#include "Matrix.h"
#include "MathUtils.h"

#include "..\FormatUtils.h"

const Vector3 Vector3::forward(0, 0, 1);
const Vector3 Vector3::back(0, 0, -1);
const Vector3 Vector3::up(0, 1, 0);
const Vector3 Vector3::down(0, -1, 0);
const Vector3 Vector3::left(-1, 0, 0);
const Vector3 Vector3::right(1, 0, 0);
const Vector3 Vector3::one(1, 1, 1);
const Vector3 Vector3::zero(0, 0, 0);

std::string Vector3::ToString() const {
    return FormatUtils::formatString("[{}, {}, {}]", x, y, z);
}

Matrix Vector3::ToMatrix3x1() const {
    float data[3] = { x, y, z };
    return Matrix(3, 1, data);
}

Matrix Vector3::ToMatrix1x3() const {
    float data[3] = { x, y, z };
    return Matrix(1, 3, data);
}

Vector3& Vector3::Set(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
    return *this;
}

void Vector3::Normalize() {
    float len = Magnitude();
    if (len > 0) {
        *this /= len;
    }
}

Vector3 Vector3::Normalize(const Vector3& vec) {
    float len = vec.Magnitude();
    if (len > 0) {
        return vec / len;
    }
    return vec;
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

float Vector3::Dot(const Vector3& a, const Vector3& b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
    return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t) {
    return Vector3(
        MathUtil::Lerp(a.x, b.x, t),
        MathUtil::Lerp(a.y, b.y, t),
        MathUtil::Lerp(a.z, b.z, t)
    );
}

float Vector3::Distance(const Vector3& a, const Vector3& b) {
    return (a - b).Magnitude();
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

Vector3 operator+(float scalar, const Vector3& other) {
    return other + scalar;
}

Vector3 operator-(float scalar, const Vector3& other) {
    return Vector3(scalar - other.x, scalar - other.y, scalar - other.z);
}

Vector3 operator*(float scalar, const Vector3& other) {
    return other * scalar;
}

Vector3 operator/(float scalar, const Vector3& other) {
    if (other.x == 0 || other.y == 0) {
        std::ostringstream oss;
        oss << "Division by zero (" << other.x << ", " << other.y << ", " << other.z << ")";
        throw std::runtime_error(oss.str());
    }
    return Vector3(scalar / other.x, scalar / other.y, scalar / other.z);
}