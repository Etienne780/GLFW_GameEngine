#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "..\Core\Log.h"
#include "..\Core\Math.h"
#include "..\Core\stb_image.h"

typedef std::string String;

class Texture2D {
public:
	/**
	* @brief Default constructor. Does not initialize any Textures.
	*/
	Texture2D();
	~Texture2D();

	/**
	* @brief Creates the texture
	*/
	void Create(const char* path);

	void Bind(unsigned int unit = 0) const;
	void Unbind(unsigned int unit = 0) const;

	void SetWrapping(unsigned int wrappingMode);
	void SetWrappingX(unsigned int wrappingMode);
	void SetWrappingY(unsigned int wrappingMode);

	void SetFilter(unsigned int filterMode);
	void SetFilterMin(unsigned int filterMode);
	void SetFilterMag(unsigned int filterMode);

	void SetGenerateMipmaps(bool enable);

	int GetID() const;
	String GetPath() const;
	int GetWidth() const;
	int GetHeight() const;
	Vector2 GetSize() const;
	int GetNrChannels() const;

private:
	unsigned int m_ID = -1;
	String m_path = "";
	int m_width = 0;
	int m_height = 0;
	int m_nrChannels = -1;
	GLenum m_format = GL_RGB;

	unsigned int m_wrappingX = GL_REPEAT;
	unsigned int m_wrappingY = GL_REPEAT;
	// filter is used when the texture gets downscaled
	unsigned int m_filterMin = GL_LINEAR;
	// filter is used when the texture gets upscaled
	unsigned int m_filterMag = GL_LINEAR;

	bool m_createMipmaps = true;
};