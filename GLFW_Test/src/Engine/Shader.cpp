#include "Shader.h"

unsigned int Shader::GetID() {
	return m_ID;
}

bool Shader::IsActive() {
	return m_IsActive;
}

Shader::Shader() {
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	String vertexCode;
	String fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file’s buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		Log::Error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		Log::Print(Log::levelError, "Vertex: {}", vertexPath);
		Log::Print(Log::levelError, "Fragment: {}", fragmentPath);
		return;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

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
		Log::Error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
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
		Log::Error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog);
	};

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);
	// print linking errors if any
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		Log::Error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", infoLog);
	}
	// delete shaders; they’re linked into our program and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Bind() {
	if (m_ID == -1) {
		Log::Warn("Could not Use Shader. Shader was not initialized");
		return;
	}
	m_IsActive = true;
	glUseProgram(m_ID);
}

void Shader::Unbind() {
	if (!m_IsActive) return;

	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	if (static_cast<GLint>(m_ID) != currentProgram)
		return;

	m_IsActive = false;
	glUseProgram(0);
}

void Shader::Delete() {
	if (m_ID == -1) {
		Log::Warn("Could not Delete Shader. Shader was not initialized");
		return;
	}
	m_IsActive = false;
	glDeleteProgram(m_ID);
}

void Shader::SetBool(const String& name, bool value) const {
	if (m_ID == -1) {
		Log::Warn("Could not SetBool Shader. Shader was not initialized");
		return;
	}
	if (!m_IsActive) {
		Log::Warn("Could not SetBool Shader. Shader is not active");
		return;
	}
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void Shader::SetInt(const String& name, int value) const {
	if (m_ID == -1) {
		Log::Warn("Could not SetInt Shader. Shader was not initialized");
		return;
	}
	if (!m_IsActive) {
		Log::Warn("Could not SetInt Shader. Shader is not active");
		return;
	}
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const String& name, float value) const {
	if (m_ID == -1) {
		Log::Warn("Could not SetFloat Shader. Shader was not initialized");
		return;
	}
	if (!m_IsActive) {
		Log::Warn("Could not SetFloat Shader. Shader is not active");
		return;
	}
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetVector2(const String& name, Vector2 value) const {
	if (m_ID == -1) {
		Log::Warn("Could not SetVector2 Shader. Shader was not initialized");
		return;
	}
	if (!m_IsActive) {
		Log::Warn("Could not SetVector2 Shader. Shader is not active");
		return;
	}
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y);
}

void Shader::SetVector3(const String& name, Vector3 value) const {
	if (m_ID == -1) {
		Log::Warn("Could not SetVector3 Shader. Shader was not initialized");
		return;
	}
	if (!m_IsActive) {
		Log::Warn("Could not SetVector3 Shader. Shader is not active");
		return;
	}
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVector4(const String& name, Vector4 value) const {
	if (m_ID == -1) {
		Log::Warn("Could not SetVector4 Shader. Shader was not initialized");
		return;
	}
	if (!m_IsActive) {
		Log::Warn("Could not SetVector4 Shader. Shader is not active");
		return;
	}
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
}