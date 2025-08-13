#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <CoreLib\Log.h>
#include <CoreLib\Math.h>
#include <CoreLib\stb_image.h>

#include "EngineLib\Texture2D.h"

namespace EngineCore {

	unsigned char* Texture2D::m_missingTexture = nullptr;

	Texture2D::Texture2D() {
		m_format = GL_RGB;

		m_wrappingX = GL_REPEAT;
		m_wrappingY = GL_REPEAT;

		m_filterMin = GL_LINEAR;
		m_filterMag = GL_LINEAR;
	}

	Texture2D::Texture2D(const char* path) {
		m_format = GL_RGB;

		m_wrappingX = GL_REPEAT;
		m_wrappingY = GL_REPEAT;

		m_filterMin = GL_LINEAR;
		m_filterMag = GL_LINEAR;

		m_path = path;
	}

	Texture2D::Texture2D(const unsigned char* data, int width, int height, int nrChannels) {
		m_format = GL_RGB;

		m_wrappingX = GL_REPEAT;
		m_wrappingY = GL_REPEAT;

		m_filterMin = GL_LINEAR;
		m_filterMag = GL_LINEAR;

		m_imageData = new unsigned char[width * height * nrChannels];
		std::memcpy(m_imageData, data, width * height * nrChannels);

		m_width = width;
		m_height = height;
		m_nrChannels = nrChannels;
	}

	Texture2D::~Texture2D() {
		DeleteGL();
		if (m_imageData != nullptr) {
			delete[] m_imageData;
			m_imageData = nullptr;
		}
	}

	void Texture2D::Cleanup() {
		if (m_missingTexture != nullptr) {
			delete[] m_missingTexture;
			m_missingTexture = nullptr;
		}
	}

	void Texture2D::Create(const char* path) {
		m_exists = true;
		m_path = path;

		// create texture
		glGenTextures(1, &m_opengGLID);
		glBindTexture(GL_TEXTURE_2D, m_opengGLID);
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
				LoadTextureFallback();
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

			LoadTextureFallback();
		}
		stbi_image_free(imageData);
	}

	void Texture2D::Create(unsigned char* data, int width, int height, int nrChannels) {
		m_exists = true;

		m_imageData = new unsigned char[width * height * nrChannels];
		std::memcpy(m_imageData, data, width * height * nrChannels);

		// create texture
		glGenTextures(1, &m_opengGLID);
		glBindTexture(GL_TEXTURE_2D, m_opengGLID);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrappingX);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrappingY);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMag);

		m_width = width;
		m_height = height;
		m_nrChannels = nrChannels;

		if (data) {
			if (m_nrChannels == 1)
				m_format = GL_RED;
			else if (m_nrChannels == 3)
				m_format = GL_RGB;
			else if (m_nrChannels == 4)
				m_format = GL_RGBA;
			else {
				Log::Error("Texture2D: Unsupported number of channels: {}!", m_nrChannels);
				LoadTextureFallback();
				return;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
			if (m_createMipmaps) {
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		else {
			Log::Error("Texture2D: Failed to Create texture, no data was provided!");
			LoadTextureFallback();
		}
	}

	void Texture2D::CreateGL() {
		if (m_opengGLID != ENGINE_INVALID_ID) return;

		if (m_path.empty()) {
			Log::Error("Texture2D: Texture could not be created, there was no path set");
			return;
		}

		if (m_path.empty())
			Create(m_imageData, m_width, m_height, m_nrChannels);
		else
			Create(m_path.c_str());
	}

	void Texture2D::DeleteGL() {
		if (!m_exists) return;

		if (m_opengGLID != ENGINE_INVALID_ID) {
			glDeleteTextures(1, &m_opengGLID);
			m_opengGLID = ENGINE_INVALID_ID;
		}

		m_exists = false;
	}

	void Texture2D::LoadTextureFallback() {
		if (m_missingTexture == nullptr) {
			m_missingTexture = GenerateFallbackTexture();
		}

		m_width = 32;
		m_height = 32;
		m_format = GL_RGB;

		glBindTexture(GL_TEXTURE_2D, m_opengGLID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_missingTexture);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	unsigned char* Texture2D::GenerateFallbackTexture() {
		const unsigned char colorA[3] = { 0x00, 0x00, 0x00 };
		const unsigned char colorB[3] = { 0xfb, 0x3e, 0xf9 };

		const int height = 32;
		const int width = 32;

		const int size = sizeof(unsigned char) * 3;

		m_missingTexture = new unsigned char[height * width * size];

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int offset = (y * width + x) * size;

				if ((x < width / 2 && y < height / 2) || (x >= width / 2 && y >= height / 2)) {
					m_missingTexture[offset] = colorA[0];
					m_missingTexture[offset + 1] = colorA[1];
					m_missingTexture[offset + 2] = colorA[2];
				}
				else {
					m_missingTexture[offset] = colorB[0];
					m_missingTexture[offset + 1] = colorB[1];
					m_missingTexture[offset + 2] = colorB[2];
				}
			}
		}

		return m_missingTexture;
	}

	void Texture2D::Bind(unsigned int unit) {
		if (!m_exists || m_opengGLID == ENGINE_INVALID_ID) {
			CreateGL();
		}
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_opengGLID);
	}

	void Texture2D::Unbind(unsigned int unit) {
		glActiveTexture(GL_TEXTURE0 + unit);

		GLint boundTexture = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

		if (static_cast<GLint>(m_opengGLID) != boundTexture)
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

	unsigned int Texture2D::GetID() const {
		return m_opengGLID;
	}

	std::string Texture2D::GetPath() const {
		return m_path;
	}

	int Texture2D::GetWidth() const {
		return m_width;
	}

	int Texture2D::GetHeight() const {
		return m_height;
	}

	Vector2 Texture2D::GetSize() const {
		return Vector2(static_cast<float>(m_width), static_cast<float>(m_height));
	}

	int Texture2D::GetNrChannels() const {
		return m_nrChannels;
	}

}