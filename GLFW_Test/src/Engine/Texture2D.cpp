#include "Texture2D.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "..\Core\Log.h"
#include "..\Core\Math.h"
#include "..\Core\stb_image.h"

Texture2D::Texture2D() {
	m_format = GL_RGB;

	m_wrappingX = GL_REPEAT;
	m_wrappingY = GL_REPEAT;

	m_filterMin = GL_LINEAR;
	m_filterMag = GL_LINEAR;
}

Texture2D::~Texture2D() {
	if (m_ID != -1) {
		glDeleteTextures(1, &m_ID);
	}
}

void Texture2D::Create(const char* path) {
	m_path = path;

	// create texture
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrappingX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrappingY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMag);
	// load and generate the texture
	unsigned char* imageData = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);

	if (imageData) {
		if (m_nrChannels == 1)
			m_format = GL_RED;
		else if (m_nrChannels == 3)
			m_format = GL_RGB;
		else if (m_nrChannels == 4)
			m_format = GL_RGBA;
		else {
			Log::Error("Texture2D: Unsupported number of channels: {}!", m_nrChannels);
			stbi_image_free(imageData);
			return;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, imageData);
		if (m_createMipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	else {
		Log::Error("Texture2D: Failed to load texture!");
		Log::Error(path);
	}
	stbi_image_free(imageData);
}

void Texture2D::Bind(unsigned int unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Unbind(unsigned int unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);

	GLint boundTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

	if (static_cast<GLint>(m_ID) != boundTexture)
		return;

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SetWrapping(unsigned int wrappingMode) {
	m_wrappingX = wrappingMode;
	m_wrappingY = wrappingMode;
}

void Texture2D::SetWrappingX(unsigned int wrappingMode) {
	m_wrappingX = wrappingMode;
}

void Texture2D::SetWrappingY(unsigned int wrappingMode) {
	m_wrappingY = wrappingMode;
}

void Texture2D::SetFilter(unsigned int filterMode) {
	m_filterMin = filterMode;
	m_filterMag = filterMode;
}

void Texture2D::SetFilterMin(unsigned int filterMode) {
	m_filterMin = filterMode;
}

void Texture2D::SetFilterMag(unsigned int filterMode) {
	m_filterMag = filterMode;
}

void Texture2D::SetGenerateMipmaps(bool enable) {
	m_createMipmaps = enable;
}

int Texture2D::GetID() const  {
	return m_ID;
}

String Texture2D::GetPath() const  {
	return m_path;
}

int Texture2D::GetWidth() const  {
	return m_width;
}

int Texture2D::GetHeight() const  {
	return m_height;
}

Vector2 Texture2D::GetSize() const  {
	return Vector2(static_cast<float>(m_width), static_cast<float>(m_height));
}

int Texture2D::GetNrChannels() const  {
	return m_nrChannels;
}
