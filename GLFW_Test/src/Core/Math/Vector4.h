#pragma once
#include <stdexcept>

class Matrix;

/**
 * @class Vector4
 * @brief Represents a 4D vector with common vector operations.
 */
class Vector4 {
public:
    static const Vector4 one; ///< Vector with all components set to one (1, 1, 1, 1)

    float x = 0; ///< X component of the vector
    float y = 0; ///< Y component of the vector
    float z = 0; ///< Z component of the vector
    float w = 0; ///< W component of the vector

    Vector4() {} ///< Default constructor initializes to (0, 0, 0, 0)
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {} ///< Constructs vector with given components

    /**
     * @brief Converts the vector to a string representation.
     * @return String in the format "(x, y, z, w)".
     */
    std::string ToString() const;
    /**
    * @brief Converts this vector to a 4x1 column matrix.
    * @return A Matrix with 4 rows and 1 column representing this vector.
    */
    Matrix ToMatrix4x1() const;
    /**
    * @brief Converts this vector to a 1x4 row matrix.
    * @return A Matrix with 1 row and 4 columns representing this vector.
    */
    Matrix ToMatrix1x4() const;

    /**
    * @brief Sets each component of a vector
    *
    * @param x component of the vector
    * @param y component of the vector
    * @param z component of the vector
    * @param w component of the vector
    *
    * @return this vector
    */
    Vector4& Set(float x, float y, float z, float w);

    /**
    * @brief Normalizes the vector to have length 1.
    * Modifies the vector in place.
    */
    void Normalize();

    /**
    * @brief Normalizes a vector to have length 1.
    * @param vec the vector that gets Normalized.
    * @return Normalized vector
    */
    static Vector4 Normalize(const Vector4& vec);

    /**
    * @brief Calculates the magnitude (length) of the vector.
    * @return Length as a float.
    */
    float Magnitude() const;

    /**
    * @brief Calculates the squared magnitude of the vector.
    * Avoids costly square root operation.
    * @return Squared length as a float.
    */
    float SquaredMagnitude() const;

    /**
    * @brief Computes the dot product with another vector.
    * @param other The other vector.
    * @return Scalar dot product result.
    */
    float Dot(const Vector4& other) const;

    /**
    * @brief Static dot product between two vectors.
    * @param a First vector.
    * @param b Second vector.
    * @return Scalar dot product result.
    */
    static float Dot(const Vector4& a, const Vector4& b);

    /**
    * @brief Linearly interpolates between two vectors.
    * @param a Start vector.
    * @param b End vector.
    * @param t Interpolation factor in [0, 1].
    * @return Interpolated vector.
    */
    static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);

    /**
    * @brief Computes the Euclidean distance between two vectors.
    * @param a First vector.
    * @param b Second vector.
    * @return Distance as a float.
    */
    static float Distance(const Vector4& a, const Vector4& b);

    // Compound assignment operators modifying this vector

    Vector4& operator+=(const Vector4& other); ///< Add another vector
    Vector4& operator-=(const Vector4& other); ///< Subtract another vector
    Vector4& operator+=(float scalar);         ///< Add scalar to all components
    Vector4& operator-=(float scalar);         ///< Subtract scalar from all components
    Vector4& operator*=(float scalar);         ///< Multiply all components by scalar
    Vector4& operator/=(float scalar);         ///< Divide all components by scalar

    // Binary operators returning new vectors

    Vector4 operator+(const Vector4& other) const; ///< Vector addition
    Vector4 operator-(const Vector4& other) const; ///< Vector subtraction
    Vector4 operator+(float scalar) const;         ///< Add scalar to vector
    Vector4 operator-(float scalar) const;         ///< Subtract scalar from vector
    Vector4 operator*(float scalar) const;         ///< Multiply vector by scalar
    Vector4 operator/(float scalar) const;         ///< Divide vector by scalar

    /**
     * @brief Index operator for accessing vector components.
     * @param index Component index (0 = x, 1 = y, 2 = z, 3 = w).
     * @return Reference to the component.
     * @throws std::out_of_range if index is invalid.
     */
    float& operator[](int index);

    /**
     * @brief Const index operator for accessing components.
     * @param index Component index.
     * @return Const reference to component.
     * @throws std::out_of_range if index is invalid.
     */
    const float& operator[](int index) const;
};

#pragma region non_member_operations

// Non-member operators to allow scalar op vector with scalar first

Vector4 operator+(float scalar, const Vector4& other);
Vector4 operator-(float scalar, const Vector4& other);
Vector4 operator*(float scalar, const Vector4& other);
Vector4 operator/(float scalar, const Vector4& other);

#pragma endregion