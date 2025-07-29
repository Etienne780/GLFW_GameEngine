#pragma once
#include <stdexcept>

template<typename T>
requires std::is_arithmetic_v<T>
class Matrix;

class Vector3 {
public:
    float x = 0;
    float y = 0;
    float z = 0;

    Vector3() {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    std::string ToString() const;

    // Normalizes the vector (sets the length to 1)
    void Normalize();
    
    // Returns the magnitude (length) of the vector
    float Magnitude() const;
    
    // Returns the squared magnitude (without square root)
    float SquaredMagnitude() const;

    // Returns the dot of this and the other vector;
    float Dot(const Vector3& other) const;

    Vector3 Cross(const Vector3& other) const;

    // Returns a new vector linearly interpolated between this vector and the given vector
    // using the interpolation factor t (0.0 returns this vector, 1.0 returns the target vector)
    Vector3 Lerp(const Vector3& other, float t) const;

    // Operator overloads for vector operations
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator+=(float scalar);
    Vector3& operator-=(float scalar);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);
    
    // Additional operator overloads for convenient operations
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator+(float scalar) const;
    Vector3 operator-(float scalar) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    float& operator[](int index);
    const float& operator[](int index) const;

    explicit operator Matrix<float>() const;
};