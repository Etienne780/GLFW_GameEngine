#pragma once
#include <stdexcept>

template<typename T>
requires std::is_arithmetic_v<T>
class Matrix;

class Vector4 {
public:
    static const Vector4 one;

    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;

    Vector4() {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    
    std::string ToString() const;

    // Normalizes the vector (sets the length to 1)
    void Normalize();
    
    // Returns the magnitude (length) of the vector
    float Magnitude() const;
    
    // Returns the squared magnitude (without square root)
    float SquaredMagnitude() const;

    // Returns the dot of this and the other vector;
    float Dot(const Vector4& other) const;

    // Returns the dot of this and the other vector;
    static float Dot(const Vector4& a, const Vector4& b);

    // Returns a new vector linearly interpolated between a and the b
    // using the interpolation factor t (0.0 returns a, 1.0 returns b)
    static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);

    static float Distance(const Vector4& a, const Vector4& b);

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

    float& operator[](int index);
    const float& operator[](int index) const;

    explicit operator Matrix<float>() const;
};

#pragma region non_member_operations

Vector4 operator+(float scalar, const Vector4& other);
Vector4 operator-(float scalar, const Vector4& other);
Vector4 operator*(float scalar, const Vector4& other);
Vector4 operator/(float scalar, const Vector4& other);

#pragma endregion