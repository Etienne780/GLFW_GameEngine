#pragma once
#include <string>

typedef std::string String;

class Matrix;
class Vector2;
class Vector3;
class Vector4;

namespace EngineCore {

    /**
     * @brief A utility class for loading, compiling, linking, and using GLSL shader programs.
     *
     * This class provides helper functions to load shader source files, compile them,
     * create a shader program, and set uniform values easily.
     */
    class Shader {
        friend class Application;
    public:
        /**
         * @brief Default constructor. Does not initialize any shader program.
         */
        Shader();

        /**
         * @brief Constructs a shader program by loading, compiling, and linking vertex and fragment shaders from file.
         *
         * @param vertexPath Path to the vertex shader source file.
         * @param fragmentPath Path to the fragment shader source file.
         */
        Shader(const char* vertexPath, const char* fragmentPath);

        unsigned int GetID();

        /**
        * @brief Gets the location in the current shader programm
        * @param name Name of the uniform variable in the GLSL shader.
        */
        int GetUniformLocation(const String& name) const;

        bool IsActive();

        // --- Uniform setters ---

        /**
         * @brief Sets a boolean uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param value Boolean value to set.
         */
        void SetBool(const String& name, bool value) const;

        /**
         * @brief Sets an integer uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param value Integer value to set.
         */
        void SetInt(const String& name, int value) const;

        /**
         * @brief Sets a float uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param value Float value to set.
         */
        void SetFloat(const String& name, float value) const;

        /**
         * @brief Sets a vec2 uniform variable (2D vector) in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param value Vector2 value to set.
         */
        void SetVector2(const String& name, const Vector2& value) const;

        /**
         * @brief Sets a vec3 uniform variable (3D vector) in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param value Vector3 value to set.
         */
        void SetVector3(const String& name, const Vector3& value) const;

        /**
         * @brief Sets a vec4 uniform variable (4D vector) in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param value Vector4 value to set.
         */
        void SetVector4(const String& name, const Vector4& value) const;

        /**
         * @brief Sets a 2x2 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 4 float values in column-major order.
         */
        void SetMatrix2(const String& name, const float* data) const;

        /**
         * @brief Sets a 3x3 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 9 float values in column-major order.
         */
        void SetMatrix3(const String& name, const float* data) const;

        /**
         * @brief Sets a 4x4 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 16 float values in column-major order.
         */
        void SetMatrix4(const String& name, const float* data) const;

        /**
         * @brief Sets a 2x3 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 6 float values in column-major order.
         */
        void SetMatrix2x3(const String& name, const float* data) const;

        /**
         * @brief Sets a 3x2 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 6 float values in column-major order.
         */
        void SetMatrix3x2(const String& name, const float* data) const;

        /**
         * @brief Sets a 2x4 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 8 float values in column-major order.
         */
        void SetMatrix2x4(const String& name, const float* data) const;

        /**
         * @brief Sets a 4x2 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 8 float values in column-major order.
         */
        void SetMatrix4x2(const String& name, const float* data) const;

        /**
         * @brief Sets a 3x4 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 12 float values in column-major order.
         */
        void SetMatrix3x4(const String& name, const float* data) const;

        /**
         * @brief Sets a 4x3 matrix uniform variable in the shader.
         * @param name Name of the uniform variable in the GLSL shader.
         * @param data Pointer to 12 float values in column-major order.
         */
        void SetMatrix4x3(const String& name, const float* data) const;

    private:
        /// OpenGL shader program ID
        unsigned int m_ID = -1;
        bool m_IsActive = false;

        /**
        * @brief Activates the shader program for use in the current OpenGL context.
        */
        void Bind();

        /**
        * @brief Deactivates the shader program in the current OpenGL context.
        */
        void Unbind();

        /**
        * @brief Deletes the shader program. Should be called during cleanup.
        */
        void Delete();

        /**
        * @brief Checks if a value can be set. if not print warnings
        *
        * @param funcName is the name of the set func
        * @param paramName is the name of the param
        */
        bool CanSetValue(const String& funcName, const String& paramName) const;
    };

}
