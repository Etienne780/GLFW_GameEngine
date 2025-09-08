#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <CoreLib/File.h>
#include <CoreLib/Log.h>

#include "EngineLib/FontAsset.h"

namespace EngineCore {

	FontAsset::FontAsset(const FT_Library& lib, const std::string& path, bool useAbsolutDir = false) {
		const char* newPath = (useAbsolutDir) ? path.c_str() : (File::GetExecutableDir() + "\\" + path).c_str();
		if (!File::Exists(newPath)) {
			Log::Error("FontManager: Failed to load font, font not found");
			Log::Print(Log::levelError, "             {}", newPath);

			Log::Error("FontManager: Not implementd, should load a default font here or somthing like that. (i am lazy)");
		}

		if (FT_New_Face(lib, newPath, 0, &m_face)) {
			Log::Error("FontManager: Failed to load font");
		}

        m_path = newPath;
	}

	FontAsset::~FontAsset() {
        if (m_face) {
            FT_Done_Face(m_face);
            m_face = nullptr;
        }
	}

	void FontAsset::LoadBaseGlyphs() {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
                Log::Error("FontAsset: Failed to load Glyph '{}', of font {}", c, m_path);
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                m_face->glyph->bitmap.width,
                m_face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                m_face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Glyph character = {
                texture,
                Vector2(static_cast<float>(m_face->glyph->bitmap.width), static_cast<float>(m_face->glyph->bitmap.rows)),
                Vector2(static_cast<float>(m_face->glyph->bitmap_left), static_cast<float>(m_face->glyph->bitmap_top)),
                static_cast<unsigned int>(m_face->glyph->advance.x)
            };

            m_glyphs.emplace(c, character);
        }
	}

}