#pragma once
#include <string>
#include <vector>

class Vector2;
class Vector3;
class Vector4;

class Matrix {
public:
    Matrix();
    // Constructs a matrix with the specified number of rows and columns, initialized to zero.
    Matrix(int rows, int cols);

    // Constructs a matrix with the specified dimensions and initializes it with the given raw float array (row-major order).
    Matrix(int rows, int cols, const float* values);

    // Constructs a matrix from a nested initializer list (e.g., {{1, 2}, {3, 4}}).
    Matrix(std::initializer_list<std::initializer_list<float>> values);

    /**
    * @brief Returns the number of rows in the matrix.
    * @return Integer representing the row count.
    */
    int GetRowCount() const;

    /**
    * @brief Returns the number of columns in the matrix.
    * @return Integer representing the column count.
    */
    int GetColCount() const;

    /**
    * @brief Provides mutable access to the raw matrix data (row-major order).
    * @return Pointer to the internal float array representing the matrix data.
    */
    float* GetData();

    /**
    * @brief Provides read-only access to the raw matrix data (row-major order).
    * @return Const pointer to the internal float array representing the matrix data.
    */
    const float* GetData() const;

    Matrix& SetData(float value);

    Matrix& SetDataDirty();

    #pragma region to_conversion

    /**
     * @brief Converts the matrix to a flat float array in column-major order.
     * @return A std::vector<float> containing the matrix elements in column-major layout.
     */
    std::vector<float> ToColMajorData() const;

    /**
     * @brief Converts the matrix to a flat float array suitable for OpenGL.
     *
     * Internally returns the same data as ToColMajorData().data(), since OpenGL expects
     * column-major layout for uniform matrices.
     *
     * @return A const float* containing the matrix elements in column-major layout.
     */
    const float* ToOpenGLData() const;

    /**
    * @brief Converts the matrix to a human-readable string.
    * @return A string representing the matrix content.
    */
    std::string ToString() const;

    /**
    * @brief Converts this matrix to a Vector2 if dimensions are compatible (2x1 or 1x2).
    * @return A Vector2 containing the corresponding matrix elements.
    * @throws std::runtime_error if dimensions are incompatible.
    */
    Vector2 ToVector2() const;

    /**
    * @brief Converts this matrix to a Vector3 if dimensions are compatible (3x1 or 1x3).
    * @return A Vector3 containing the corresponding matrix elements.
    * @throws std::runtime_error if dimensions are incompatible.
    */
    Vector3 ToVector3() const;

    /**
    * @brief Converts this matrix to a Vector4 if dimensions are compatible (4x1 or 1x4).
    * @return A Vector4 containing the corresponding matrix elements.
    * @throws std::runtime_error if dimensions are incompatible.
    */
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
    // Matrix multiplication with another matrix
    Matrix& operator*=(const Matrix& other);
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
    mutable bool m_isDataDirty = true;
    mutable std::vector<float> m_cachedColMajorData;
    
    // row-major layout
    int ToIndex(int row, int col) const;
};

#pragma region non_member_operations

Matrix operator+(float scalar, const Matrix& matrix);
Matrix operator-(float scalar, const Matrix& matrix);
Matrix operator*(float scalar, const Matrix& matrix);
Matrix operator/(float scalar, const Matrix& matrix);

#pragma endregion

/**
* @brief GLTransform provides functions to create and manipulate 4x4 matrices, primarily for OpenGL use.
*/
namespace GLTransform {

    Matrix Identity();

    Matrix ScaleNonUniform(float x, float y, float z);
    Matrix ScaleNonUniform(const Vector3& scalar);

    Matrix ScaleUniform(float scalar);

    Matrix Translation(float x, float y, float z);
    Matrix Translation(const Vector3& translation);

    Matrix RotationX(float radians);
    Matrix RotationY(float radians);
    Matrix RotationZ(float radians);
    Matrix RotationXYZ(float rx, float ry, float rz);
    Matrix RotationXYZ(const Vector3& radians);

    /**
     * @brief Creates an orthographic projection matrix (right-handed coordinate system).
     *
     * Maps a 3D volume defined by the left, right, bottom, top, near, and far planes into normalized device coordinates.
     * This is typically used for 2D rendering or UI systems, where perspective distortion is not desired.
     *
     * @param left   The left plane of the view volume.
     * @param right  The right plane of the view volume.
     * @param bottom The bottom plane of the view volume.
     * @param top    The top plane of the view volume.
     * @param zNear  The near clipping plane distance.
     * @param zFar   The far clipping plane distance.
     * @return       A 4x4 orthographic projection matrix.
     */
    Matrix Orthographic(float left, float right, float bottom, float top, float zNear, float zFar);

    /**
     * @brief Creates a perspective projection matrix (right-handed coordinate system).
     *
     * Simulates a realistic perspective where distant objects appear smaller.
     * This matrix is commonly used for 3D scenes with depth perception.
     *
     * @param fovy    Field of view in the y-direction, in radians.
     * @param aspect  Aspect ratio of the viewport (width / height).
     * @param zNear   The near clipping plane distance (must be > 0).
     * @param zFar    The far clipping plane distance (must be > zNear).
     * @return        A 4x4 perspective projection matrix.
     */
    Matrix Perspective(float fovy, float aspect, float zNear, float zFar);

    Matrix LookAt(const Vector3& position, const Vector3& target, const Vector3& up);

    void Identity(Matrix& out);
    
    void ScaleNonUniform(Matrix& out, float x, float y, float z);
    void ScaleNonUniform(Matrix& out, const Vector3& scalar);
    
    void ScaleUniform(Matrix& out, float scalar);
    
    void Translation(Matrix& out, float x, float y, float z);
    void Translation(Matrix& out, const Vector3& translation);
    
    void RotationX(Matrix& out, float radians);
    void RotationY(Matrix& out, float radians);
    void RotationZ(Matrix& out, float radians);
    void RotationXYZ(Matrix& out, float rx, float ry, float rz);
    void RotationXYZ(Matrix& out, const Vector3& radians);
}