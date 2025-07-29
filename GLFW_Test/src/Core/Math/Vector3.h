#pragma once
#include <stdexcept>

template<typename T>
requires std::is_arithmetic_v<T>
class Matrix;

class Vector3 {
public:
    static const Vector3 forward;
    static const Vector3 back;
    static const Vector3 up;
    static const Vector3 down;
    static const Vector3 left;
    static const Vector3 right;
    static const Vector3 one;

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
    // Returns the cross of this and the other vector;
    Vector3 Cross(const Vector3& other) const;

    // Returns the dot of a and b;
    static float Dot(const Vector3& a, const Vector3& b);
    // Returns the cross of a and b;
    static Vector3 Cross(const Vector3& a, const Vector3& b);

    // Returns a new vector linearly interpolated between a and the b
    // using the interpolation factor t (0.0 returns a, 1.0 returns b)
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

    static float Distance(const Vector3& a, const Vector3& b);

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

#pragma region non_member_operations

Vector3 operator+(float scalar, const Vector3& other);
Vector3 operator-(float scalar, const Vector3& other);
Vector3 operator*(float scalar, const Vector3& other);
Vector3 operator/(float scalar, const Vector3& other);

#pragma endregion