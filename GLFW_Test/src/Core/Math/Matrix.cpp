#include "Matrix.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "..\FormatUtils.h"

Matrix::Matrix(int rows, int cols)
    : m_rows(rows), m_cols(cols), m_data(rows* cols, 0.0f) {
}

Matrix::Matrix(int rows, int cols, const float* values)
    : m_rows(rows), m_cols(cols), m_data(values, values + (rows * cols)) {
}

Matrix::Matrix(std::initializer_list<std::initializer_list<float>> values) {
    m_rows = static_cast<int>(values.size());
    m_cols = values.begin()->size();
    m_data.reserve(m_rows * m_cols);
    for (const auto& row : values) {
        if (row.size() != static_cast<size_t>(m_cols))
            throw std::runtime_error("All rows must have the same number of elements.");
        m_data.insert(m_data.end(), row.begin(), row.end());
    }
}

int Matrix::GetRowCount() const { 
    return m_rows; 
}

int Matrix::GetColCount() const { 
    return m_cols; 
}

const float* Matrix::Data() const {
    return m_data.data();
}

#pragma region to_conversion

std::vector<float> Matrix::ToOpenGLData() const {
    std::vector<float> result;
    result.reserve(m_rows * m_cols);

    for (int col = 0; col < m_cols; ++col)
        for (int row = 0; row < m_rows; ++row)
            result.push_back((*this)(row, col));

    return result;
}

String Matrix::ToString() const {
    String result;
    for (int i = 0; i < GetRowCount(); ++i) {
        for (int j = 0; j < GetColCount(); ++j) {
            result += FormatUtils::toString((*this)(i, j)) + " ";
        }
        result += "\n";
    }
    return result;
}

Vector2 Matrix::ToVector2() const {
#ifndef NDEBUG
    if (!((GetRowCount() == 2 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 2))) {
        throw std::runtime_error("Matrix cannot be converted to Vector2 due to incompatible dimensions");
    }
#endif
    return GetColCount() == 1 ?
        Vector2((*this)(0, 0), (*this)(1, 0)) :
        Vector2((*this)(0, 0), (*this)(0, 1));
}

Vector3 Matrix::ToVector3() const {
#ifndef NDEBUG
    if (!((GetRowCount() == 3 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 3))) {
        throw std::runtime_error("Matrix cannot be converted to Vector3 due to incompatible dimensions");
    }
#endif
    return GetColCount() == 1 ?
        Vector3((*this)(0, 0), (*this)(1, 0), (*this)(2, 0)) :
        Vector3((*this)(0, 0), (*this)(0, 1), (*this)(0, 2));
}

Vector4 Matrix::ToVector4() const {
#ifndef NDEBUG
    if (!((GetRowCount() == 4 && GetColCount() == 1) || (GetRowCount() == 1 && GetColCount() == 4))) {
        throw std::runtime_error("Matrix cannot be converted to Vector4 due to incompatible dimensions");
    }
#endif
    return GetColCount() == 1 ?
        Vector4((*this)(0, 0), (*this)(1, 0), (*this)(2, 0), (*this)(3, 0)) :
        Vector4((*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(0, 3));
}

int Matrix::ToIndex(int row, int col) const {
#ifndef NDEBUG
    if (row < 0 || col < 0 || row >= m_rows || col >= m_cols) {
        throw std::runtime_error("Matrix index out of bounds");
    }
#endif
    return row * m_cols + col; // row-major layout
}

#pragma endregion

float& Matrix::operator()(int row, int col) {
#ifndef NDEBUG
    if (row < 0 || col < 0 || row >= m_rows || col >= m_cols) {
        throw std::runtime_error("Matrix index out of bounds");
    }
#endif
    return m_data[ToIndex(row, col)];
}

const float& Matrix::operator()(int row, int col) const {
#ifndef NDEBUG
    if (row < 0 || col < 0 || row >= m_rows || col >= m_cols) {
        throw std::runtime_error("Matrix index out of bounds");
    }
#endif
    return m_data[ToIndex(row, col)];
}

#pragma region operation=

Matrix& Matrix::operator+=(const Matrix& other) {
    if (GetRowCount() != other.GetRowCount() || GetColCount() != other.GetColCount()) {
        throw std::runtime_error("Matrix dimensions do not match for addition.");
    }
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            (*this)(i, j) += other(i, j);
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (GetRowCount() != other.GetRowCount() || GetColCount() != other.GetColCount()) {
        throw std::runtime_error("Matrix dimensions do not match for subtraction.");
    }
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            (*this)(i, j) -= other(i, j);
    return *this;
}

Matrix& Matrix::operator+=(float scalar) {
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            (*this)(i, j) += scalar;
    return *this;
}

Matrix& Matrix::operator-=(float scalar) {
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            (*this)(i, j) -= scalar;
    return *this;
}

Matrix& Matrix::operator*=(float scalar) {
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            (*this)(i, j) *= scalar;
    return *this;
}

Matrix& Matrix::operator/=(float scalar) {
    if (scalar == 0)
        throw std::runtime_error("Matrix division by zero is not allowed");

    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            (*this)(i, j) /= scalar;
    return *this;
}

#pragma endregion

#pragma region operation

Matrix Matrix::operator+(const Matrix& other) const {
    Matrix result = *this;
    result += other;
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    Matrix result = *this;
    result -= other;
    return result;
}

Matrix Matrix::operator+(float scalar) const {
    Matrix result = *this;
    result += scalar;
    return result;
}

Matrix  Matrix::operator-(float scalar) const {
    Matrix result = *this;
    result -= scalar;
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (GetColCount() != other.GetRowCount()) {
        throw std::runtime_error("Matrix multiplication not allowed");
    }

    Matrix result(GetRowCount(), other.GetColCount());

    for (int i = 0; i < GetRowCount(); ++i) {
        for (int j = 0; j < other.GetColCount(); ++j) {
            float sum = 0.0f;
            for (int k = 0; k < GetColCount(); ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }

    return result;
}

Matrix Matrix::operator*(const Vector2& other) const {
    if (GetColCount() != 2)
        throw std::runtime_error("Matrix column count must match Vector2 size (2)");

    Matrix result(GetRowCount(), 1);

    for (int i = 0; i < GetRowCount(); ++i)
        result(i, 0) = (*this)(i, 0) * other[0] + (*this)(i, 1) * other[1];

    return result;
}

Matrix Matrix::operator*(const Vector3& other) const {
    if (GetColCount() != 3)
        throw std::runtime_error("Matrix column count must match Vector3 size (3)");

    Matrix result(GetRowCount(), 1);

    for (int i = 0; i < GetRowCount(); ++i)
        result(i, 0) = (*this)(i, 0) * other[0]
        + (*this)(i, 1) * other[1]
        + (*this)(i, 2) * other[2];

    return result;
}

Matrix Matrix::operator*(const Vector4& other) const {
    if (GetColCount() != 4)
        throw std::runtime_error("Matrix column count must match Vector4 size (4)");

    Matrix result(GetRowCount(), 1);

    for (int i = 0; i < GetRowCount(); ++i)
        result(i, 0) = (*this)(i, 0) * other[0]
        + (*this)(i, 1) * other[1]
        + (*this)(i, 2) * other[2]
        + (*this)(i, 3) * other[3];

    return result;
}

Matrix Matrix::operator*(float scalar) const {
    Matrix result = *this;
    result *= scalar;
    return result;
}

Matrix Matrix::operator/(float scalar) const {
    Matrix result = *this;
    result /= scalar;
    return result;
}

#pragma endregion

#pragma region non_member_operations

Matrix operator+(float scalar, const Matrix& matrix) {
    return matrix + scalar;
}

Matrix operator-(float scalar, const Matrix& matrix) {
    int rows = matrix.GetRowCount();
    int cols = matrix.GetColCount();

    Matrix result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result(i, j) = scalar - matrix(i, j);
        }
    }

    return result;
}

Matrix operator*(float scalar, const Matrix& matrix) {
    return matrix * scalar;
}

Matrix operator/(float scalar, const Matrix& matrix) {
    int rows = matrix.GetRowCount();
    int cols = matrix.GetColCount();

    Matrix result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float val = matrix(i, j);
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

namespace GLTransform {

    Matrix Identity() {
        float m4x4[16] = { 
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        return Matrix(4, 4, m4x4);
    }

    Matrix ScaleNonUniform(float x, float y, float z) {
        float data[16] = {
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        };
        return Matrix(4, 4, data);
    }

    Matrix ScaleNonUniform(Vector3 scalar) {
        return ScaleNonUniform(scalar.x, scalar.y, scalar.z);
    }

    Matrix ScaleUniform(float scalar) {
        return ScaleNonUniform(scalar, scalar, scalar);
    }

    Matrix Translation(float x, float y, float z) {
        float data[16] = {
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1
        };
        return Matrix(4, 4, data);
    }

    Matrix Translation(Vector3 translation) {
        return Translation(translation.x, translation.y, translation.z);
    }

    Matrix RotationX(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);
        float data[16] = {
            1, 0,  0, 0,
            0, c, -s, 0,
            0, s,  c, 0,
            0, 0,  0, 1
        };
        return Matrix(4, 4, data);
    }

    Matrix RotationY(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);
        float data[16] = {
             c, 0, s, 0,
             0, 1, 0, 0,
            -s, 0, c, 0,
             0, 0, 0, 1
        };
        return Matrix(4, 4, data);
    }

    Matrix RotationZ(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);
        float data[16] = {
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        };
        return Matrix(4, 4, data);
    }

    Matrix RotationXYZ(float rx, float ry, float rz) {
        return RotationZ(rz) * RotationY(ry) * RotationX(rx);
    }

    Matrix RotationXYZ(Vector3 radians) {
        return RotationXYZ(radians.x, radians.y, radians.z);
    }

    Matrix Combine(const Matrix& last) {
        return last;
    }
}