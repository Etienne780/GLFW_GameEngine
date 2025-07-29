#include "Matrix.h"

#pragma region matrix_templates

Matrix<float> matrix_gl_identity() {
    return Matrix<float>({
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    });
};

Matrix<float> matrix_gl_scale_non_uniform(float x, float y, float z) {
    return Matrix<float>({
        { x, 0, 0, 0 },
        { 0, y, 0, 0 },
        { 0, 0, z, 0 },
        { 0, 0, 0, 1 }
    });
};

Matrix<float> matrix_gl_scale_non_uniform(Vector3 scalar) {
    return matrix_gl_scale_non_uniform(scalar.x, scalar.y, scalar.z);
};

Matrix<float> matrix_gl_scale_uniform(float scalar) {
    return matrix_gl_scale_non_uniform(scalar, scalar, scalar);
};

Matrix<float> matrix_gl_translation(float x, float y, float z) {
    return Matrix<float>({
        { 1, 0, 0, x },
        { 0, 1, 0, y },
        { 0, 0, 1, z },
        { 0, 0, 0, 1 }
    });
};

Matrix<float> matrix_gl_translation(Vector3 translation) {
    return matrix_gl_translation(translation.x, translation.y, translation.z);
};

#pragma endregion