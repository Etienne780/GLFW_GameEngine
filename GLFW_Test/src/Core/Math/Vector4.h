#pragma once
#include <stdexcept>
#include <cmath>

#include "MathUtils.h"
#include "..\FormatUtils.h"

class Vector4 {
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;

    Vector4() {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    
    std::string toString() const;

    // Normalizes the vector (sets the length to 1)
    void normalize();
    
    // Returns the magnitude (length) of the vector
    float magnitude() const;
    
    // Returns the squared magnitude (without square root)
    float squaredMagnitude() const;

    // Returns the dot of this and the other vector;
    float dot(const Vector4& other) const;

    // Returns a new vector linearly interpolated between this vector and the given vector
    // using the interpolation factor t (0.0 returns this vector, 1.0 returns the target vector)
    Vector4 lerp(const Vector4& vec, float t) const;

    // Operator overloads for vector operations
    Vector4& operator+=(const Vector4& other);
    Vector4& operator-=(const Vector4& other);

    Vector4& operator+=(float scalar);
    Vector4& operator-=(float scalar);

    Vector4& operator*=(float scalar);
    Vector4& operator/=(float scalar);
    
    // Additional operator overloads for convenient operations
    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;

    Vector4 operator+(float scalar) const;
    Vector4 operator-(float scalar) const;

    Vector4 operator*(float scalar) const;
    Vector4 operator/(float scalar) const;
};