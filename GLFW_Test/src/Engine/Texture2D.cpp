#include "Texture2D.h"

Texture2D::Texture2D() {
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
		GLenum format;
		if (m_nrChannels == 1)
			format = GL_RED;
		else if (m_nrChannels == 3)
			format = GL_RGB;
		else if (m_nrChannels == 4)
			format = GL_RGBA;
		else {
			Log::Error("Texture2D: Unsupported number of channels: {}!", m_nrChannels);
			stbi_image_free(imageData);
			return;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		Log::Error("Texture2D: Failed to load texture!");
		Log::Error(path);
	}
	stbi_image_free(imageData);
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

int Texture2D::GetID() {
	return m_ID;
}

String Texture2D::GetPath() {
	return m_path;
}

int Texture2D::GetWidth() {
	return m_width;
}

int Texture2D::GetHeight() {
	return m_height;
}

Vector2 Texture2D::GetSize() {
	return Vector2(m_width, m_height);
}

int Texture2D::GetNrChannels() {
	return m_nrChannels;
}
