#include "Matrix.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "..\FormatUtils.h"

Matrix::Matrix() {
}

Matrix::Matrix(int rows, int cols)
    : m_rows(rows), m_cols(cols), m_data(rows* cols, 0.0f) {
}

Matrix::Matrix(int rows, int cols, const float* values)
    : m_rows(rows), m_cols(cols), m_data(values, values + (rows * cols)) {
}

Matrix::Matrix(std::initializer_list<std::initializer_list<float>> values) {
    m_rows = static_cast<int>(values.size());
    m_cols = static_cast<int>(values.begin()->size());
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

float* Matrix::GetData() {
    return m_data.data(); 
}

const float* Matrix::GetData() const {
    return m_data.data();
}

void Matrix::SetData(float value) {
    std::fill(m_data.begin(), m_data.end(), value);
}

#pragma region to_conversion
std::vector<float> Matrix::ToColMajorData() const {
    std::vector<float> result;
    result.reserve(m_rows * m_cols);
    const float* a = GetData();

    for (int col = 0; col < m_cols; ++col)
        for (int row = 0; row < m_rows; ++row)
            result.push_back(a[row * m_cols + col]);

    return result;
}

const float* Matrix::ToOpenGLData() const {
    m_cachedColMajorData = ToColMajorData();
    return m_cachedColMajorData.data();
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
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw std::runtime_error("Matrix dimensions do not match for addition.");
    }

    float* a = GetData();
    const float* b = other.GetData();

    for (int i = 0; i < m_rows; ++i)
        for (int j = 0; j < m_cols; ++j)
            a[i * m_cols + j] += b[i * m_cols + j];
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
#ifndef NDEBUG
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw std::runtime_error("Matrix dimensions do not match for subtraction.");
    }
#endif

    float* a = GetData();
    const float* b = other.GetData();

    for (int i = 0; i < m_rows; ++i)
        for (int j = 0; j < m_cols; ++j)
            a[i * m_cols + j] -= b[i * m_cols + j];
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    #ifndef NDEBUG
    if (m_cols != other.m_rows) {
        throw std::runtime_error("Matrix dimensions invalid for multiplication.");
    }
    #endif

    Matrix result(m_rows, other.m_cols);
    const float* a = GetData();
    const float* b = other.GetData();
    float* r = result.GetData();

    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < other.m_cols; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < m_cols; ++k) {
                sum += a[i * m_cols + k] * b[k * other.m_cols + j];
            }
            r[i * other.m_cols + j] = sum;
        }
    }

    *this = std::move(result);
    return *this;
}

Matrix& Matrix::operator+=(float scalar) {
    float* a = GetData();
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            a[i * m_cols + j] += scalar;
    return *this;
}

Matrix& Matrix::operator-=(float scalar) {
    float* a = GetData();
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            a[i * m_cols + j] -= scalar;
    return *this;
}

Matrix& Matrix::operator*=(float scalar) {
    float* a = GetData();
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            a[i * m_cols + j] *= scalar;
    return *this;
}

Matrix& Matrix::operator/=(float scalar) {
#ifndef NDEBUG
    if (scalar == 0)
        throw std::runtime_error("Matrix division by zero is not allowed");
#endif
    float* a = GetData();
    for (int i = 0; i < GetRowCount(); ++i)
        for (int j = 0; j < GetColCount(); ++j)
            a[i * m_cols + j] /= scalar;
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
    if (m_cols != other.m_rows) 
        throw std::runtime_error("Matrix multiplication not allowed");

    Matrix result(m_rows, other.m_cols);
    const float* a = GetData();
    const float* b = other.GetData();
    float* r = result.GetData();

    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < other.m_cols; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < m_cols; ++k) {
                sum += a[i * m_cols + k] * b[k * other.m_cols + j];
            }
            r[i * other.m_cols + j] = sum;
        }
    }

    return result;
}

Matrix Matrix::operator*(const Vector2& other) const {
    if (m_cols != 2)
        throw std::runtime_error("Matrix column count must match Vector2 size (2)");

    Matrix result(m_rows, 1);
    const float* a = GetData();
    float* r = result.GetData();

    for (int i = 0; i < m_rows; ++i)
        r[i] = a[i * m_cols + 0] * other.x + a[i * m_cols + 1] * other.y;

    return result;
}

Matrix Matrix::operator*(const Vector3& other) const {
    if (m_cols != 3)
        throw std::runtime_error("Matrix column count must match Vector3 size (3)");

    Matrix result(m_rows, 1);
    const float* a = GetData();
    float* r = result.GetData();

    for (int i = 0; i < m_rows; ++i)
        r[i] = a[i * m_cols + 0] * other.x
        + a[i * m_cols + 1] * other.y
        + a[i * m_cols + 2] * other.z;

    return result;
}

Matrix Matrix::operator*(const Vector4& other) const {
    if (m_cols != 4)
        throw std::runtime_error("Matrix column count must match Vector4 size (4)");

    Matrix result(m_rows, 1);
    const float* a = GetData();
    float* r = result.GetData();

    for (int i = 0; i < m_rows; ++i)
        r[i] = a[i * m_cols + 0] * other.x
        + a[i * m_cols + 1] * other.y
        + a[i * m_cols + 2] * other.z
        + a[i * m_cols + 3] * other.w;

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
    const float* a = matrix.GetData();
    float* r = result.GetData();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;

#ifndef NDEBUG
            if (a[index] == 0.0f) {
                std::ostringstream oss;
                oss << "Division by zero at (" << i << "," << j << ")";
                throw std::runtime_error(oss.str());
            }
#endif

            r[index] = scalar / a[index];
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

    Matrix ScaleNonUniform(const Vector3& scalar) {
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

    Matrix Translation(const Vector3& translation) {
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

    Matrix RotationXYZ(const Vector3& radians) {
        return RotationXYZ(radians.x, radians.y, radians.z);
    }

    Matrix Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        Matrix result = Matrix(4, 4);
        float* a = result.GetData();

        a[0 * 4 + 0] = 2.0f / (right - left);
        a[1 * 4 + 1] = 2.0f / (top - bottom);
        a[2 * 4 + 2] = -2.0f / (zFar - zNear);
        a[3 * 4 + 3] = 1.0f;

        a[0 * 4 + 3] = -(right + left) / (right - left);
        a[1 * 4 + 3] = -(top + bottom) / (top - bottom);
        a[2 * 4 + 3] = -(zFar + zNear) / (zFar - zNear);
        return result;
    }

    Matrix Perspective(float fovy, float aspect, float zNear, float zFar)
    {
        Matrix result = Matrix(4, 4);
        float* a = result.GetData();

        float const tanHalfFovy = tan(fovy / 2.0f);
        a[0 * 4 + 0] = 1.0f / (aspect * tanHalfFovy);
        a[1 * 4 + 1] = 1.0f / (tanHalfFovy);
        a[2 * 4 + 2] = -(zFar + zNear) / (zFar - zNear);
        a[2 * 4 + 3] = -(2.0f * zFar * zNear) / (zFar - zNear); 
        a[3 * 4 + 2] = -1.0f;
        return result;
    }

    void Identity(Matrix& out) {
        #ifndef NDEBUG
        if (out.GetRowCount() != 4 || out.GetColCount() != 4) {
            throw std::runtime_error("Identity: Matrix must be 4x4");
        }
        #endif

        out.SetData(0);
        float* o = out.GetData();
        o[0] = 1;
        o[1 * 4 + 1] = 1;
        o[2 * 4 + 2] = 1;
        o[3 * 4 + 3] = 1;
    }

    void ScaleNonUniform(Matrix& out, float x, float y, float z) {
        #ifndef NDEBUG
        if (out.GetRowCount() != 4 || out.GetColCount() != 4) {
            throw std::runtime_error("ScaleNonUniform: Matrix must be 4x4");
        }
        #endif
        float* o = out.GetData();
        o[0] = x;
        o[1 * 4 + 1] = y;
        o[2 * 4 + 2] = z;
    }

    void ScaleNonUniform(Matrix& out, const Vector3& scalar) {
        ScaleNonUniform(out, scalar.x, scalar.y, scalar.z);
    }


    void ScaleUniform(Matrix& out, float scalar) {
        ScaleNonUniform(out, scalar, scalar, scalar);
    }

    void Translation(Matrix& out, float x, float y, float z) {
        #ifndef NDEBUG
        if (out.GetRowCount() != 4 || out.GetColCount() != 4) {
            throw std::runtime_error("Translation: Matrix must be 4x4");
        }
        #endif
        float* o = out.GetData();
        o[0 * 4 + 3] = x;
        o[1 * 4 + 3] = y;
        o[2 * 4 + 3] = z;
    }

    void Translation(Matrix& out, const Vector3& translation) {
        Translation(out, translation.x, translation.y, translation.z);
    }

    void RotationX(Matrix& out, float radians) {
        #ifndef NDEBUG
        if (out.GetRowCount() != 4 || out.GetColCount() != 4) {
            throw std::runtime_error("RotationX: Matrix must be 4x4");
        }
        #endif
        float c = std::cos(radians), s = std::sin(radians);
        float* o = out.GetData();
        o[0] = 1;
        o[1 * 4 + 1] = c;
        o[1 * 4 + 2] = -s;
        o[2 * 4 + 1] = s;
        o[2 * 4 + 2] = c;
        o[3 * 4 + 3] = 1;
    }

    void RotationY(Matrix& out, float radians) {
        #ifndef NDEBUG
        if (out.GetRowCount() != 4 || out.GetColCount() != 4) {
            throw std::runtime_error("RotationY: Matrix must be 4x4");
        }
        #endif
        float c = std::cos(radians), s = std::sin(radians);
        float* o = out.GetData();
        o[0] = c;
        o[2] = s;
        o[1 * 4 + 1] = 1;
        o[2 * 4 + 0] = -s;
        o[2 * 4 + 2] = c;
        o[3 * 4 + 3] = 1;
    }

    void RotationZ(Matrix& out, float radians) {
        #ifndef NDEBUG
        if (out.GetRowCount() != 4 || out.GetColCount() != 4) {
            throw std::runtime_error("RotationZ: Matrix must be 4x4");
        }
        #endif
        float c = std::cos(radians), s = std::sin(radians);
        float* o = out.GetData();
        o[0] = c;
        o[1] = -s;
        o[1 * 4 + 0] = s;
        o[1 * 4 + 1] = c;
        o[2 * 4 + 2] = 1;
        o[3 * 4 + 3] = 1;
    }

    void RotationXYZ(Matrix& out, float rx, float ry, float rz) {
        #ifndef NDEBUG
        if (out.GetRowCount() != 4 || out.GetColCount() != 4) {
            throw std::runtime_error("RotationXYZ: Matrix must be 4x4");
        }
        #endif

        Matrix rotX(4, 4), rotY(4, 4), rotZ(4, 4);
        RotationX(rotX, rx);
        RotationY(rotY, ry);
        RotationZ(rotZ, rz);
        out = rotZ * rotY * rotX;
    }

    void RotationXYZ(Matrix& out, const Vector3& radians) {
        RotationXYZ(out, radians.x, radians.y, radians.z);
    }
}