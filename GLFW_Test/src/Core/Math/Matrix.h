#pragma once
#include <string>
#include <vector>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "..\FormatUtils.h"

typedef std::string String;

/*
* Matrix create template
auto rotationMatrix = [](float angle) -> Matrix<float> {
    float c = std::cos(angle);
    float s = std::sin(angle);

    return Matrix<float>({
        { c, -s, 0 },
        { s,  c, 0 },
        { 0,  0, 1 }
    });
};
*/

template<typename T>
requires std::is_arithmetic_v<T>
class Matrix {
public:
    Matrix(int row, int column)
        : m_matrix(row, std::vector<T>(column, T(0))) {
    }

    Matrix(const std::vector<std::vector<T>>& values)
        : m_matrix(values) {
    }

    Matrix(std::initializer_list<std::initializer_list<T>> values) {
        for (const auto& row : values) {
            m_matrix.emplace_back(row);
        }
    }

    int GetRowCount() const { return static_cast<int>(m_matrix.size()); }
    int GetColCount() const { return static_cast<int>(m_matrix.empty() ? 0 : m_matrix[0].size()); }

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

    T& operator()(int row, int column) {
        if (row < 0 || column < 0 || row >= GetRowCount() || column >= GetColCount()) {
            throw std::runtime_error("Matrix index out of bounds");
        }
        return m_matrix[row][column];
    }

    const T& operator()(int row, int column) const {
        if (row < 0 || column < 0 || row >= GetRowCount() || column >= GetColCount()) {
            throw std::runtime_error("Matrix index out of bounds");
        }
        return m_matrix[row][column];
    }

    Matrix& operator+=(const Matrix& other) {
        if (GetRowCount() != other.GetRowCount() ||
            GetColCount() != other.GetColCount()) {
            throw std::runtime_error("Matrix dimensions do not match for addition.");
        }

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                m_matrix[i][j] += other.m_matrix[i][j];
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        if (GetRowCount() != other.GetRowCount() ||
            GetColCount() != other.GetColCount()) {
            throw std::runtime_error("Matrix dimensions do not match for subtraction.");
        }

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                m_matrix[i][j] -= other.m_matrix[i][j];
            }
        }
        return *this;
    }

    Matrix& operator+=(T scalar) {
        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                m_matrix[i][j] += scalar;
            }
        }
        return *this;
    }

    Matrix& operator-=(T scalar) {
        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                m_matrix[i][j] -= scalar;
            }
        }
        return *this;
    }

    Matrix& operator*=(T scalar) {
        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                m_matrix[i][j] *= scalar;
            }
        }
        return *this;
    }

    Matrix& operator/=(T scalar) {
        if (scalar == 0) {
            throw std::runtime_error("Matrix division by zero is not allowed");
        }

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                m_matrix[i][j] /= scalar;
            }
        }
        return *this;
    }

    Matrix operator+(const Matrix& other) const {
        if (GetRowCount() != other.GetRowCount() ||
            GetColCount() != other.GetColCount()) {
            throw std::runtime_error("Matrix dimensions do not match for addition.");
        }

        Matrix result(static_cast<int>(m_matrix.size()), static_cast<int>(m_matrix[0].size()));

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                result(i, j) = m_matrix[i][j] + other.m_matrix[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (GetRowCount() != other.GetRowCount() ||
            GetColCount() != other.GetColCount()) {
            throw std::runtime_error("Matrix dimensions do not match for subtraction.");
        }

        Matrix result(static_cast<int>(m_matrix.size()), static_cast<int>(m_matrix[0].size()));

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                result(i, j) = m_matrix[i][j] - other.m_matrix[i][j];
            }
        }
        return result;
    }

    Matrix operator+(T scalar) const {
        Matrix result(GetRowCount(), GetColCount());

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                result(i, j) = m_matrix[i][j] + scalar;
            }
        }
        return result;
    }

    Matrix operator-(T scalar) const {
        Matrix result(GetRowCount(), GetColCount());

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                result(i, j) = m_matrix[i][j] - scalar;
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (GetColCount() != other.GetRowCount()) {
            throw std::runtime_error("Matrix multiplication not allowed");
        }
        Matrix result(GetRowCount(), other.GetColCount());

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < other.GetColCount(); ++j) {
                for (int k = 0; k < GetColCount(); ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

    Matrix<T> operator*(const Vector2& other) const {
        if (GetColCount() != 2) {
            throw std::runtime_error("Matrix column count must match Vector2 size (2)");
        }

        Matrix<T> result(GetRowCount(), 1);

        for (int i = 0; i < GetRowCount(); ++i) {
            result(i, 0) = (*this)(i, 0) * other[0] + (*this)(i, 1) * other[1];
        }

        return result;
    }

    Matrix<T> operator*(const Vector3& other) const {
        if (GetColCount() != 3) {
            throw std::runtime_error("Matrix column count must match Vector3 size (3)");
        }

        Matrix<T> result(GetRowCount(), 1);

        for (int i = 0; i < GetRowCount(); ++i) {
            result(i, 0) = (*this)(i, 0) * other[0]
                + (*this)(i, 1) * other[1]
                + (*this)(i, 2) * other[2];
        }

        return result;
    }

    Matrix<T> operator*(const Vector4& other) const {
        if (GetColCount() != 4) {
            throw std::runtime_error("Matrix column count must match Vector4 size (4)");
        }

        Matrix<T> result(GetRowCount(), 1);

        for (int i = 0; i < GetRowCount(); ++i) {
            result(i, 0) = (*this)(i, 0) * other[0]
                + (*this)(i, 1) * other[1]
                + (*this)(i, 2) * other[2]
                + (*this)(i, 3) * other[3];
        }

        return result;
    }

    Matrix operator*(T scalar) const {
        Matrix result(GetRowCount(), GetColCount());

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                result(i, j) = m_matrix[i][j] * scalar;
            }
        }
        return result;
    }

    Matrix operator/(T scalar) const {
        if (scalar == 0) {
            throw std::runtime_error("Matrix division by zero is not allowed");
        }
        Matrix result(GetRowCount(), GetColCount());

        for (int i = 0; i < GetRowCount(); ++i) {
            for (int j = 0; j < GetColCount(); ++j) {
                result(i, j) = m_matrix[i][j] / scalar;
            }
        }
        return result;
    }

    explicit operator Vector2() const {
        if (!((GetRowCount() == 2 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 2))) {
            throw std::runtime_error("Matrix cannot be converted to Vector2 due to incompatible dimensions");
        }

        if (GetColCount() == 1) {
            return Vector2((*this)(0, 0), (*this)(1, 0));
        }
        else {
            return Vector2((*this)(0, 0), (*this)(0, 1));
        }
    }

    explicit operator Vector3() const {
        if (!((GetRowCount() == 3 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 3))) {
            throw std::runtime_error("Matrix cannot be converted to Vector3 due to incompatible dimensions");
        }

        if (GetColCount() == 1) {
            return Vector3((*this)(0, 0), (*this)(1, 0), (*this)(2, 0));
        }
        else {
            return Vector3((*this)(0, 0), (*this)(0, 1), (*this)(0, 2));
        }
    }

    explicit operator Vector4() const {
        if (!((GetRowCount() == 4 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 4))) {
            throw std::runtime_error("Matrix cannot be converted to Vector4 due to incompatible dimensions");
        }

        if (GetColCount() == 1) {
            return Vector4((*this)(0, 0), (*this)(1, 0), (*this)(2, 0), (*this)(3, 0));
        }
        else {
            return Vector4((*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(0, 3));
        }
    }

private:
    std::vector<std::vector<T>> m_matrix;
};

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