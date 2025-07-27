#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "..\Core\Log.h"
#include "..\Core\Math.h"

typedef std::string String;

/**
 * @brief A utility class for loading, compiling, linking, and using GLSL shader programs.
 *
 * This class provides helper functions to load shader source files, compile them,
 * create a shader program, and set uniform values easily.
 */
class Shader {
public:
    /// OpenGL shader program ID
    unsigned int ID;

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

    /**
     * @brief Activates the shader program for use in the current OpenGL context.
     */
    void Use();

    /**
     * @brief Deletes the shader program. Should be called during cleanup.
     */
    void Delete();

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
    void SetVector2(const String& name, Vector2 value) const;

    /**
     * @brief Sets a vec3 uniform variable (3D vector) in the shader.
     * @param name Name of the uniform variable in the GLSL shader.
     * @param value Vector3 value to set.
     */
    void SetVector3(const String& name, Vector3 value) const;

    /**
     * @brief Sets a vec4 uniform variable (4D vector) in the shader.
     * @param name Name of the uniform variable in the GLSL shader.
     * @param value Vector4 value to set.
     */
    void SetVector4(const String& name, Vector4 value) const;
};
