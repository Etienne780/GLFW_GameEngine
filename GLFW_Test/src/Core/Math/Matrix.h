#pragma once
#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "..\FormatUtils.h"

typedef std::string String;

// Forward declaration of identity and transformation matrices
Matrix<float> matrix_gl_identity();
Matrix<float> matrix_gl_scale_non_uniform(float x, float y, float z);
Matrix<float> matrix_gl_scale_non_uniform(Vector3 scalar);
Matrix<float> matrix_gl_scale_uniform(float scalar);
Matrix<float> matrix_gl_translation(float x, float y, float z);
Matrix<float> matrix_gl_translation(Vector3 translation);

template<typename T>
    requires std::is_arithmetic_v<T>
class Matrix {
public:
    // Constructs a matrix with given row and column dimensions, initialized with zeros
    Matrix(int row, int column)
        : m_matrix(row, std::vector<T>(column, T(0))) {
    }

    // Constructs a matrix using a nested vector of values
    Matrix(const std::vector<std::vector<T>>& values)
        : m_matrix(values) {
    }

    // Constructs a matrix using an initializer list (e.g., {{1, 2}, {3, 4}})
    Matrix(std::initializer_list<std::initializer_list<T>> values) {
        for (const auto& row : values) {
            m_matrix.emplace_back(row);
        }
    }

    // Returns the number of rows in the matrix
    int GetRowCount() const { return static_cast<int>(m_matrix.size()); }

    // Returns the number of columns in the matrix
    int GetColCount() const { return static_cast<int>(m_matrix.empty() ? 0 : m_matrix[0].size()); }

    // Converts the matrix to a human-readable string representation
    String ToString() const {
        String result;
        for (const auto& row : m_matrix) {
            for (const auto& val : row) {
                result += FormatUtils::toString(val) + " ";
            }
            result += "\n";
        }
        return result;
    }

    // Accesses or modifies an element at the specified row and column
    T& operator()(int row, int column) {
        if (row < 0 || column < 0 || row >= GetRowCount() || column >= GetColCount()) {
            throw std::runtime_error("Matrix index out of bounds");
        }
        return m_matrix[row][column];
    }

    // Returns the value at the specified row and column
    const T& operator()(int row, int column) const {
        if (row < 0 || column < 0 || row >= GetRowCount() || column >= GetColCount()) {
            throw std::runtime_error("Matrix index out of bounds");
        }
        return m_matrix[row][column];
    }

    // Adds another matrix element-wise
    Matrix& operator+=(const Matrix& other) {
        if (GetRowCount() != other.GetRowCount() || GetColCount() != other.GetColCount()) {
            throw std::runtime_error("Matrix dimensions do not match for addition.");
        }
        for (int i = 0; i < GetRowCount(); ++i)
            for (int j = 0; j < GetColCount(); ++j)
                m_matrix[i][j] += other.m_matrix[i][j];
        return *this;
    }

    // Subtracts another matrix element-wise
    Matrix& operator-=(const Matrix& other) {
        if (GetRowCount() != other.GetRowCount() || GetColCount() != other.GetColCount()) {
            throw std::runtime_error("Matrix dimensions do not match for subtraction.");
        }
        for (int i = 0; i < GetRowCount(); ++i)
            for (int j = 0; j < GetColCount(); ++j)
                m_matrix[i][j] -= other.m_matrix[i][j];
        return *this;
    }

    // Adds a scalar to all matrix elements
    Matrix& operator+=(T scalar) {
        for (int i = 0; i < GetRowCount(); ++i)
            for (int j = 0; j < GetColCount(); ++j)
                m_matrix[i][j] += scalar;
        return *this;
    }

    // Subtracts a scalar from all matrix elements
    Matrix& operator-=(T scalar) {
        for (int i = 0; i < GetRowCount(); ++i)
            for (int j = 0; j < GetColCount(); ++j)
                m_matrix[i][j] -= scalar;
        return *this;
    }

    // Multiplies all elements by a scalar
    Matrix& operator*=(T scalar) {
        for (int i = 0; i < GetRowCount(); ++i)
            for (int j = 0; j < GetColCount(); ++j)
                m_matrix[i][j] *= scalar;
        return *this;
    }

    // Divides all elements by a scalar
    Matrix& operator/=(T scalar) {
        if (scalar == 0)
            throw std::runtime_error("Matrix division by zero is not allowed");

        for (int i = 0; i < GetRowCount(); ++i)
            for (int j = 0; j < GetColCount(); ++j)
                m_matrix[i][j] /= scalar;
        return *this;
    }

    // Matrix addition
    Matrix operator+(const Matrix& other) const {
        Matrix result = *this;
        result += other;
        return result;
    }

    // Matrix subtraction
    Matrix operator-(const Matrix& other) const {
        Matrix result = *this;
        result -= other;
        return result;
    }

    // Matrix-scalar addition
    Matrix operator+(T scalar) const {
        Matrix result = *this;
        result += scalar;
        return result;
    }

    // Matrix-scalar subtraction
    Matrix operator-(T scalar) const {
        Matrix result = *this;
        result -= scalar;
        return result;
    }

    // Matrix multiplication with another matrix
    Matrix operator*(const Matrix& other) const {
        if (GetColCount() != other.GetRowCount()) {
            throw std::runtime_error("Matrix multiplication not allowed");
        }

        Matrix result(GetRowCount(), other.GetColCount());

        for (int i = 0; i < GetRowCount(); ++i)
            for (int j = 0; j < other.GetColCount(); ++j)
                for (int k = 0; k < GetColCount(); ++k)
                    result(i, j) += (*this)(i, k) * other(k, j);

        return result;
    }

    // Matrix-vector multiplication (Vector2)
    Matrix<T> operator*(const Vector2& other) const {
        if (GetColCount() != 2)
            throw std::runtime_error("Matrix column count must match Vector2 size (2)");

        Matrix<T> result(GetRowCount(), 1);

        for (int i = 0; i < GetRowCount(); ++i)
            result(i, 0) = (*this)(i, 0) * other[0] + (*this)(i, 1) * other[1];

        return result;
    }

    // Matrix-vector multiplication (Vector3)
    Matrix<T> operator*(const Vector3& other) const {
        if (GetColCount() != 3)
            throw std::runtime_error("Matrix column count must match Vector3 size (3)");

        Matrix<T> result(GetRowCount(), 1);

        for (int i = 0; i < GetRowCount(); ++i)
            result(i, 0) = (*this)(i, 0) * other[0]
            + (*this)(i, 1) * other[1]
            + (*this)(i, 2) * other[2];

        return result;
    }

    // Matrix-vector multiplication (Vector4)
    Matrix<T> operator*(const Vector4& other) const {
        if (GetColCount() != 4)
            throw std::runtime_error("Matrix column count must match Vector4 size (4)");

        Matrix<T> result(GetRowCount(), 1);

        for (int i = 0; i < GetRowCount(); ++i)
            result(i, 0) = (*this)(i, 0) * other[0]
            + (*this)(i, 1) * other[1]
            + (*this)(i, 2) * other[2]
            + (*this)(i, 3) * other[3];

        return result;
    }

    // Matrix-scalar multiplication
    Matrix operator*(T scalar) const {
        Matrix result = *this;
        result *= scalar;
        return result;
    }

    // Matrix-scalar division
    Matrix operator/(T scalar) const {
        Matrix result = *this;
        result /= scalar;
        return result;
    }

#pragma region explicit_casting

    // Converts this matrix to a Vector2 if the dimensions match
    explicit operator Vector2() const {
        if (!((GetRowCount() == 2 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 2))) {
            throw std::runtime_error("Matrix cannot be converted to Vector2 due to incompatible dimensions");
        }
        return GetColCount() == 1 ?
            Vector2((*this)(0, 0), (*this)(1, 0)) :
            Vector2((*this)(0, 0), (*this)(0, 1));
    }

    // Converts this matrix to a Vector3 if the dimensions match
    explicit operator Vector3() const {
        if (!((GetRowCount() == 3 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 3))) {
            throw std::runtime_error("Matrix cannot be converted to Vector3 due to incompatible dimensions");
        }
        return GetColCount() == 1 ?
            Vector3((*this)(0, 0), (*this)(1, 0), (*this)(2, 0)) :
            Vector3((*this)(0, 0), (*this)(0, 1), (*this)(0, 2));
    }

    // Converts this matrix to a Vector4 if the dimensions match
    explicit operator Vector4() const {
        if (!((GetRowCount() == 4 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 4))) {
            throw std::runtime_error("Matrix cannot be converted to Vector4 due to incompatible dimensions");
        }
        return GetColCount() == 1 ?
            Vector4((*this)(0, 0), (*this)(1, 0), (*this)(2, 0), (*this)(3, 0)) :
            Vector4((*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(0, 3));
    }

#pragma endregion

private:
    std::vector<std::vector<T>> m_matrix;
};

#pragma region non_member_operations

template<typename T>
Matrix<T> operator+(T scalar, const Matrix<T>& matrix) {
    return matrix + scalar;
}

template<typename T>
Matrix<T> operator-(T scalar, const Matrix<T>& matrix) {
    int rows = matrix.GetRowCount();
    int cols = matrix.GetColCount();

    Matrix<T> result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result(i, j) = scalar - matrix(i, j);
        }
    }

    return result;
}

template<typename T>
Matrix<T> operator*(T scalar, const Matrix<T>& matrix) {
    return matrix * scalar;
}

template<typename T>
Matrix<T> operator/(T scalar, const Matrix<T>& matrix) {
    int rows = matrix.GetRowCount();
    int cols = matrix.GetColCount();

    Matrix<T> result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            T val = matrix(i, j);
            if (val == 0) {
                std::ostringstream oss;
                oss << "Division by zero at (" << i << "," << j << ")";
                throw std::runtime_error(oss.str());
            }
            result(i, j) = scalar / val;
        }
    }

    return result;
}

#pragma endregion