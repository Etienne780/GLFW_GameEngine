#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <CoreLib\Log.h>
#include <CoreLib\Math.h>

#include "EngineLib\Shader.h"

namespace EngineCore {

	ShaderID Shader::GetID() {
		return m_ID;
	}

	bool Shader::IsActive() const {
		GLint currentProgram = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

		return (static_cast<GLint>(m_ID.value) == currentProgram);
	}

	Shader::Shader() {
	}

	Shader::Shader(const std::string& vertex, const std::string& fragment, bool IsShaderCode) {
		// check if the given strings is code or file paths
		if (!IsShaderCode) {
			// retrieve the vertex/fragment source code from filePath
			std::string vertexCode;
			std::string fragmentCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;
			// ensure ifstream objects can throw exceptions:
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				// open files
				vShaderFile.open(vertex);
				fShaderFile.open(fragment);
				std::stringstream vShaderStream, fShaderStream;
				// read file’s buffer contents into streams
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// close file handlers
				vShaderFile.close();
				fShaderFile.close();
				// convert stream into string
				m_vertexCode = vShaderStream.str();
				m_fragmentCode = fShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				Log::Error("Shader: FILE_NOT_SUCCESFULLY_READ");
				Log::Print(Log::levelError, "Vertex: {}", vertex);
				Log::Print(Log::levelError, "Fragment: {}", fragment);
				return;
			}
		}
		else {
			m_vertexCode = vertex;
			m_fragmentCode = fragment;
		}

		CreateGL();
	}

	Shader::~Shader() {
		DeleteGL();
	}

	void Shader::Bind() {
		if (m_ID.value == ENGINE_INVALID_ID) {
			Log::Warn("Shader: Could not Use Shader. GL ShaderProgram was not created");
			return;
		}
		glUseProgram(m_ID.value);
	}

	void Shader::Unbind() {
		if (!IsActive()) return;
		glUseProgram(0);
	}

	void Shader::CreateGL() {
		if (m_ID.value != ENGINE_INVALID_ID) return;

		if (m_vertexCode.empty() || m_fragmentCode.empty()) {
			Log::Warn("Shader: Could not create Shader. Shader code '{}' was empty!",
				(m_vertexCode.empty()) ? "vertex" : "fragment");
			return;
		}

		const char* vShaderCode = m_vertexCode.c_str();
		const char* fShaderCode = m_fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		// vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			Log::Error("Shader: VERTEX::COMPILATION_FAILED\n{}", infoLog);
		};

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			Log::Error("Shader: FRAGMENT::COMPILATION_FAILED\n{}", infoLog);
		};

		m_ID.value = glCreateProgram();
		glAttachShader(m_ID.value, vertex);
		glAttachShader(m_ID.value, fragment);
		glLinkProgram(m_ID.value);
		// print linking errors if any
		glGetProgramiv(m_ID.value, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ID.value, 512, NULL, infoLog);
			Log::Error("Shader: PROGRAM::LINKING_FAILED\n{}", infoLog);
		}
		// delete shaders; they’re linked into our program and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::DeleteGL() {
		if (m_ID.value == ENGINE_INVALID_ID) return;
		glDeleteProgram(m_ID.value);
		m_ID.value = ENGINE_INVALID_ID;
	}

	bool Shader::CanSetValue(const std::string& funcName, const std::string& paramName) const {
		if (m_ID.value == ENGINE_INVALID_ID) {
			Log::Warn("Shader: Could not {} ({}). Shader was not created", funcName, paramName);
			return false;
		}
		if (!IsActive()) {
			Log::Warn("Shader: Could not {} ({}). Shader is not active", funcName, paramName);
			return false;
		}

		return true;
	}

	void Shader::SetBool(const std::string& name, bool value) const {
		if (!CanSetValue("SetBool", name)) return;

		glUniform1i(glGetUniformLocation(m_ID.value, name.c_str()), (int)value);
	}

	void Shader::SetInt(const std::string& name, int value) const {
		if (!CanSetValue("SetInt", name)) return;

		glUniform1i(glGetUniformLocation(m_ID.value, name.c_str()), value);
	}

	void Shader::SetFloat(const std::string& name, float value) const {
		if (!CanSetValue("SetFloat", name)) return;

		glUniform1f(glGetUniformLocation(m_ID.value, name.c_str()), value);
	}

	void Shader::SetVector2(const std::string& name, const Vector2& value) const {
		if (!CanSetValue("SetVector2", name)) return;

		glUniform2f(glGetUniformLocation(m_ID.value, name.c_str()), value.x, value.y);
	}

	void Shader::SetVector3(const std::string& name, const Vector3& value) const {
		if (!CanSetValue("SetVector3", name)) return;

		glUniform3f(glGetUniformLocation(m_ID.value, name.c_str()), value.x, value.y, value.z);
	}

	void Shader::SetVector4(const std::string& name, const Vector4& value) const {
		if (!CanSetValue("SetVector4", name)) return;

		glUniform4f(glGetUniformLocation(m_ID.value, name.c_str()), value.x, value.y, value.z, value.w);
	}

#pragma region SetMatrix

	void Shader::SetMatrix2(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

	void Shader::SetMatrix3(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

	void Shader::Shader::SetMatrix4(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

	void Shader::SetMatrix2x3(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix2x3fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

	void Shader::SetMatrix3x2(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix3x2fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

	void Shader::SetMatrix2x4(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix2x4fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

	void Shader::SetMatrix4x2(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix4x2fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

	void Shader::SetMatrix3x4(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix3x4fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

	void Shader::SetMatrix4x3(const std::string& name, const float* data) const {
		if (!CanSetValue("SetMatrix", name)) return;

		glUniformMatrix4x3fv(GetUniformLocation(name), 1, GL_FALSE, data);
	}

#pragma endregion

	int Shader::GetUniformLocation(const std::string& name) const {
		int location = glGetUniformLocation(m_ID.value, name.c_str());
		if (location == -1) {
			Log::Warn("Shader: Param {} was not found", name);
			return -1;
		}
		return location;
	}

}