#include "Vector4.h"
#include <stdexcept>
#include <cmath>

#include "Matrix.h"
#include "MathUtils.h"

#include "..\FormatUtils.h"

const Vector4 Vector4::one(1, 1, 1, 1);
const Vector4 Vector4::zero(0, 0, 0, 0);

std::string Vector4::ToString() const {
    return FormatUtils::formatString("[{}, {}, {}, {}]", x, y, z, w);
}
Matrix Vector4::ToMatrix4x1() const {
    float data[4] = { x, y, z, w };
    return Matrix(4, 1, data);
}

Matrix Vector4::ToMatrix1x4() const {
    float data[4] = { x, y, z, w };
    return Matrix(1, 4, data);
}

Vector4& Vector4::Set(float _x, float _y, float _z, float _w) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
    return *this;
}

void Vector4::Normalize() {
    float len = Magnitude();
    if (len > 0) {
        *this /= len;
    }
}

static Vector4 Normalize(const Vector4& vec) {
    float len = vec.Magnitude();
    if (len > 0) {
        return vec / len;
    }
    return vec;
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

float Vector4::Dot(const Vector4& a, const Vector4& b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float t) {
    return Vector4(
        MathUtil::Lerp(a.x, b.x, t),
        MathUtil::Lerp(a.y, b.y, t),
        MathUtil::Lerp(a.z, b.z, t),
        MathUtil::Lerp(a.w, b.w, t)
    );
}

float Vector4::Distance(const Vector4& a, const Vector4& b) {
    return (a - b).Magnitude();
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

Vector4 operator+(float scalar, const Vector4& other) {
    return other + scalar;
}

Vector4 operator-(float scalar, const Vector4& other) {
    return Vector4(scalar - other.x, scalar - other.y, scalar - other.z, scalar - other.w);
}

Vector4 operator*(float scalar, const Vector4& other) {
    return other * scalar;
}

Vector4 operator/(float scalar, const Vector4& other) {
    if (other.x == 0 || other.y == 0) {
        std::ostringstream oss;
        oss << "Division by zero (" << other.x << ", " << other.y << ", " << other.z << ", " << other.w << ")";
        throw std::runtime_error(oss.str());
    }
    return Vector4(scalar / other.x, scalar / other.y, scalar / other.z, scalar / other.w);
}