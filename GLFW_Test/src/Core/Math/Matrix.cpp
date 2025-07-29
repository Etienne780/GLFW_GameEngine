#include "Matrix.h"

namespace GLTransform {

    Matrix<float> Identity() {
        return Matrix<float>({
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 }
        });
    };

    Matrix<float> ScaleNonUniform(float x, float y, float z) {
        return Matrix<float>({
            { x, 0, 0, 0 },
            { 0, y, 0, 0 },
            { 0, 0, z, 0 },
            { 0, 0, 0, 1 }
        });
    };

    Matrix<float> ScaleNonUniform(Vector3 scalar) {
        return ScaleNonUniform(scalar.x, scalar.y, scalar.z);
    };

    Matrix<float> ScaleUniform(float scalar) {
        return ScaleNonUniform(scalar, scalar, scalar);
    };

    Matrix<float> Translation(float x, float y, float z) {
        return Matrix<float>({
            { 1, 0, 0, x },
            { 0, 1, 0, y },
            { 0, 0, 1, z },
            { 0, 0, 0, 1 }
        });
    };

    Matrix<float> Translation(Vector3 translation) {
        return Translation(translation.x, translation.y, translation.z);
    };

    Matrix<float> RotationX(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);

        return Matrix<float>({
            { 1, 0,  0, 0 },
            { 0, c, -s, 0 },
            { 0, s,  c, 0 },
            { 0, 0,  0, 1 }
        });
    };

    Matrix<float> RotationY(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);

        return Matrix<float>({
            {  c, 0, s, 0 },
            {  0, 1, 0, 0 },
            { -s, 0, c, 0 },
            {  0, 0, 0, 1 }
        });
    };

    Matrix<float> RotationZ(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);

        return Matrix<float>({
            { c, -s, 0, 0 },
            { s,  c, 0, 0 },
            { 0,  0, 1, 0 },
            { 0,  0, 0, 1 }
        });
    };

    Matrix<float> RotationXYZ(float rx, float ry, float rz) {
        return RotationZ(rz) * RotationY(ry) * RotationX(rx);
    }

    Matrix<float> RotationXYZ(Vector3 radians) {
        return RotationXYZ(radians.x, radians.y, radians.z);
    }

}