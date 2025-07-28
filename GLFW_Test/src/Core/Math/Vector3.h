#pragma once
#include <stdexcept>
#include <cmath>

#include "MathUtils.h"
#include "..\FormatUtils.h"

class Vector3 {
public:
    float x = 0;
    float y = 0;
    float z = 0;

    Vector3() {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    std::string toString() const;

    // Normalizes the vector (sets the length to 1)
    void normalize();
    
    // Returns the magnitude (length) of the vector
    float magnitude() const;
    
    // Returns the squared magnitude (without square root)
    float squaredMagnitude() const;

    // Returns a new vector linearly interpolated between this vector and the given vector
    // using the interpolation factor t (0.0 returns this vector, 1.0 returns the target vector)
    Vector3 lerp(const Vector3& vec, float t) const;

    // Operator overloads for vector operations
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);
    
    // Additional operator overloads for convenient operations
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;
};