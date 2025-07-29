#pragma once
#include <stdexcept>

template<typename T>
requires std::is_arithmetic_v<T>
class Matrix;

class Vector2 {
public:
    static const Vector2 up;
    static const Vector2 down;
    static const Vector2 left;
    static const Vector2 right;
    static const Vector2 one;

    float x = 0;
    float y = 0;

    Vector2() {}
    Vector2(float x, float y) : x(x), y(y) {}
    
    std::string ToString() const;

    // Normalizes the vector (sets the length to 1)
    void Normalize();
    
    // Returns the magnitude (length) of the vector
    float Magnitude() const;
    
    // Returns the squared magnitude (without square root)
    float SquaredMagnitude() const;

    // Returns the dot of this and the other vector;
    float Dot(const Vector2& other) const;
    // Returns the cross of this and the other vector;
    float Cross(const Vector2& other) const;

    // Returns the dot of a and b;
    static float Dot(const Vector2& a, const Vector2& b);
    // Returns the cross of a and b;
    static float Cross(const Vector2& a, const Vector2& b);

    // Returns a new vector linearly interpolated between a and b
    // using the interpolation factor t (0.0 returns a, 1.0 returns b)
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

    static float Distance(const Vector2& a, const Vector2& b);

    // Operator overloads for vector operations
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator+=(float scalar);
    Vector2& operator-=(float scalar);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);
    
    // Additional operator overloads for convenient operations
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator+(float scalar) const;
    Vector2 operator-(float scalar) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;

    float& operator[](int index);
    const float& operator[](int index) const;

    explicit operator Matrix<float>() const;
};

#pragma region non_member_operations

Vector2 operator+(float scalar, const Vector2& other);
Vector2 operator-(float scalar, const Vector2& other);
Vector2 operator*(float scalar, const Vector2& other);
Vector2 operator/(float scalar, const Vector2& other);

#pragma endregion