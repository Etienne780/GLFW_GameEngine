#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <CoreLib\Log.h>
#include <CoreLib\File.h>
#include <CoreLib\Math.h>
#include <CoreLib\stb_image.h>

#include "EngineLib\Texture2D.h"

namespace EngineCore {

	Texture2D::Texture2D(bool useFallBack) {
		if (!useFallBack) {
			m_wrappingX = GL_REPEAT;
			m_wrappingY = GL_REPEAT;

			m_filterMin = GL_LINEAR;
			m_filterMag = GL_LINEAR;
		}
		else {
			LoadTextureFallback();
		}
	}

	Texture2D::Texture2D(const std::string& path, bool useAbsolutDir) {
		m_wrappingX = GL_REPEAT;
		m_wrappingY = GL_REPEAT;

		m_filterMin = GL_LINEAR;
		m_filterMag = GL_LINEAR;

		if(useAbsolutDir)
			m_path = path;
		else
			m_path = File::GetExecutableDir() + path;

		CreateGL();
	}

	Texture2D::Texture2D(const unsigned char* data, int width, int height, int nrChannels) {
		if (data && width > 0 && height > 0 && nrChannels > 0) {
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
		else {
			LoadTextureFallback();
		}
	}

	Texture2D::~Texture2D() {
		DeleteGL();
		if (m_imageData != nullptr) {
			delete[] m_imageData;
			m_imageData = nullptr;
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
			GLenum dataFormat, internalFormat;
			if (m_nrChannels == 1) {
				dataFormat = GL_RED;
				internalFormat = GL_R8;
			}
			else if (m_nrChannels == 3) {
				dataFormat = GL_RGB;
				internalFormat = GL_RGB8;
			}
			else if (m_nrChannels == 4) {
				dataFormat = GL_RGBA;
				internalFormat = GL_RGBA8;
			}
			else {
				Log::Error("Texture2D: Unsupported number of channels: {}!", m_nrChannels);
				LoadTextureFallback();
				stbi_image_free(imageData);
				m_exists = false;
				return;
			}

			int rowBytes = m_width * m_nrChannels;
			int alignment = 4;
			if (rowBytes % 4 != 0) alignment = 1;
			glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
				m_width, m_height, 0,
				dataFormat, GL_UNSIGNED_BYTE, imageData);

			if (m_createMipmaps) {
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		else {
			Log::Error("Texture2D: Failed to load texture!");
			Log::Error(path);

			LoadTextureFallback();
			m_exists = false;
		}
		stbi_image_free(imageData);
	}

	void Texture2D::Create(unsigned char* data, int width, int height, int nrChannels) {
		m_exists = true;

		if (nrChannels < 1 || nrChannels > 4) {
			Log::Error("Texture2D: Invalid number of channels ({}). Expected 1-4.", nrChannels);
			return;
		}

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
			GLenum dataFormat, internalFormat;
			if (m_nrChannels == 1) {
				dataFormat = GL_RED;
				internalFormat = GL_R8;
			}
			else if (m_nrChannels == 3) {
				dataFormat = GL_RGB;
				internalFormat = GL_RGB8;
			}
			else if (m_nrChannels == 4) {
				dataFormat = GL_RGBA;
				internalFormat = GL_RGBA8;
			}
			else {
				Log::Error("Texture2D: Unsupported number of channels: {}!", m_nrChannels);
				LoadTextureFallback();
				m_exists = false;
				return;
			}

			int rowBytes = m_width * m_nrChannels;
			int alignment = 4;
			if (rowBytes % 4 != 0) alignment = 1;
			glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
				m_width, m_height, 0,
				dataFormat, GL_UNSIGNED_BYTE, data);

			if (m_createMipmaps) {
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		else {
			Log::Error("Texture2D: Failed to Create texture, no data was provided!");
			LoadTextureFallback();
			m_exists = false;
		}
	}

	void Texture2D::CreateGL() {
		if (m_exists) return;

		if (!m_path.empty()) {
			if (File::Exists(m_path)) {
				Create(m_path.c_str());
				return;
			}
			Log::Error("Texture2D: Could not load texture. File '{}' dose not exist", m_path);
		}
	
		Create(m_imageData, m_width, m_height, m_nrChannels);
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
		m_imageData = GenerateFallbackTexture();

		m_width = 32;
		m_height = 32;
		m_nrChannels = 3;

		m_wrappingX = GL_REPEAT;
		m_wrappingY = GL_REPEAT;

		m_filterMin = GL_NEAREST;
		m_filterMag = GL_NEAREST;
		m_createMipmaps = true;
		m_path = "";
		
		CreateGL();
	}

	unsigned char* Texture2D::GenerateFallbackTexture() {
		const unsigned char colorA[3] = { 0x00, 0x00, 0x00 };
		const unsigned char colorB[3] = { 0xfb, 0x3e, 0xf9 };

		const int height = 32;
		const int width = 32;

		const int size = sizeof(unsigned char) * 3;

		unsigned char* data = new unsigned char[height * width * size];

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int offset = (y * width + x) * size;

				if ((x < width / 2 && y < height / 2) || (x >= width / 2 && y >= height / 2)) {
					data[offset] = colorA[0];
					data[offset + 1] = colorA[1];
					data[offset + 2] = colorA[2];
				}
				else {
					data[offset] = colorB[0];
					data[offset + 1] = colorB[1];
					data[offset + 2] = colorB[2];
				}
			}
		}

		return data;
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

	#pragma region Set

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

	#pragma endregion

	#pragma region Get

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

	#pragma endregion

}