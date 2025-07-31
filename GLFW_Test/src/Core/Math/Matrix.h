#pragma once
#include <string>
#include <vector>

typedef std::string String;

class Vector2;
class Vector3;
class Vector4;

class Matrix {
public:
    // Constructs a matrix with given row and column dimensions, initialized with zeros
    Matrix(int rows, int cols);

    Matrix(int rows, int cols, const float* values);
    // Constructs a matrix using an initializer list (e.g., {{1, 2}, {3, 4}})
    Matrix(std::initializer_list<std::initializer_list<float>> values);

    // Returns the number of rows in the matrix
    int GetRowCount() const;

    // Returns the number of columns in the matrix
    int GetColCount() const;

    const float* Data() const;

    #pragma region to_conversion

    std::vector<float> ToOpenGLData() const;

    // Converts the matrix to a human-readable string representation
    String ToString() const;
    // Converts this matrix to a Vector2 if the dimensions match
    Vector2 ToVector2() const;
    // Converts this matrix to a Vector3 if the dimensions match
    Vector3 ToVector3() const;
    // Converts this matrix to a Vector4 if the dimensions match
    Vector4 ToVector4() const;

    #pragma endregion

    // Accesses or modifies an element at the specified row and column
    float& operator()(int row, int col);

    // Returns the value at the specified row and column
    const float& operator()(int row, int col) const;

    #pragma region operation=

    // Adds another matrix element-wise
    Matrix& operator+=(const Matrix& other);
    // Subtracts another matrix element-wise
    Matrix& operator-=(const Matrix& other);
    // Adds a scalar to all matrix elements
    Matrix& operator+=(float scalar);
    // Subtracts a scalar from all matrix elements
    Matrix& operator-=(float scalar);
    // Multiplies all elements by a scalar
    Matrix& operator*=(float scalar);
    // Divides all elements by a scalar
    Matrix& operator/=(float scalar);

    #pragma endregion

    #pragma region operation

    // Matrix addition
    Matrix operator+(const Matrix& other) const;
    // Matrix subtraction
    Matrix operator-(const Matrix& other) const;
    // Matrix-scalar addition
    Matrix operator+(float scalar) const;
    // Matrix-scalar subtraction
    Matrix operator-(float scalar) const;
    // Matrix multiplication with another matrix
    Matrix operator*(const Matrix& other) const;
    // Matrix-vector multiplication (Vector2)
    Matrix operator*(const Vector2& other) const;
    // Matrix-vector multiplication (Vector3)
    Matrix operator*(const Vector3& other) const;
    // Matrix-vector multiplication (Vector4)
    Matrix operator*(const Vector4& other) const;
    // Matrix-scalar multiplication
    Matrix operator*(float scalar) const;
    // Matrix-scalar division
    Matrix operator/(float scalar) const;

    #pragma endregion

private:
    int m_rows = 0;
    int m_cols = 0;
    std::vector<float> m_data;

    Matrix();
    
    // row-major layout
    int ToIndex(int row, int col) const;
};

#pragma region non_member_operations

Matrix operator+(float scalar, const Matrix& matrix);
Matrix operator-(float scalar, const Matrix& matrix);
Matrix operator*(float scalar, const Matrix& matrix);
Matrix operator/(float scalar, const Matrix& matrix);

#pragma endregion

namespace GLTransform {

    Matrix Identity();

    Matrix ScaleNonUniform(float x, float y, float z);
    Matrix ScaleNonUniform(Vector3 scalar);

    Matrix ScaleUniform(float scalar);

    Matrix Translation(float x, float y, float z);
    Matrix Translation(Vector3 translation);

    Matrix RotationX(float radians);
    Matrix RotationY(float radians);
    Matrix RotationZ(float radians);
    Matrix RotationXYZ(float rx, float ry, float rz);
    Matrix RotationXYZ(Vector3 radians);

    Matrix Combine(const Matrix& last);

    /**
     * @brief Combines multiple matrices by multiplying them in the given order (left to right).
     * @tparam T Type of matrix elements (e.g., float)
     * @param first The first matrix in the multiplication chain
     * @param rest Remaining matrices to multiply
     * @return Combined transformation matrix (first * rest...)
     */
    template<typename... Matrices>
    Matrix Combine(const Matrix& first, const Matrices&... rest) {
        return first * Combine(rest...);
    }
}