#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <CoreLib/File.h>
#include <CoreLib/Log.h>

#include "EngineLib/FontAsset.h"

namespace EngineCore {

	FontAsset::FontAsset(const FT_Library& lib, const std::string& path, bool useAbsolutDir = false) {
        std::string fullPath;
        if (useAbsolutDir) {
            fullPath = path;
        }
        else {
            fullPath = File::GetExecutableDir() + "\\" + path;
        }
        const char* newPath = fullPath.c_str();

		if (!File::Exists(newPath)) {
			Log::Error("FontManager: Failed to load font, font not found");
			Log::Print(Log::levelError, "             {}", newPath);

			Log::Error("FontManager: Not implementd, should load a default font here or somthing like that. (i am lazy)");
		}

        FT_Error error = FT_New_Face(lib, newPath, 0, &m_face);
        if (error) {
            Log::Error("FontManager: Failed to load font '{}', FreeType error {}", newPath, error);
            m_face = nullptr;
        }

        m_path = newPath;
	}

	FontAsset::~FontAsset() {
        if (m_face) {
            FT_Done_Face(m_face);
            m_face = nullptr;
        }
	}

    void FontAsset::SetNumberOfMaxAtlases(size_t maxAtlases) {
        m_maxAtlases = maxAtlases;
    }

    const FontAsset::Glyph& FontAsset::GetGlyph(char c, int pixelSize) {
        auto it = m_atlases.find(pixelSize);
        if (it == m_atlases.end()) {
            BuildAtlas(pixelSize);
            return GetGlyph(c, pixelSize);
        }

        it->second.lastUsedFrame = ++m_accessCounter;
        auto glyphIt = it->second.glyphs.find(c);
        if (glyphIt == it->second.glyphs.end()) {
            Log::Warn("FontAsset: Glyph '{}' not found in atlas (size {})", c, pixelSize);
            Log::Print(Log::levelWarning, "                      {}", m_path);
            static Glyph dummy{};
            return dummy;
        }
        return glyphIt->second;
    }

    unsigned int FontAsset::GetAtlasTextureID(int pixelSize) {
        auto it = m_atlases.find(pixelSize);
        if (it == m_atlases.end()) {
            BuildAtlas(pixelSize);

            it = m_atlases.find(pixelSize);
            if (it == m_atlases.end()) {
                Log::Error("FontAsset: Failed to build atlas for pixelSize {}", pixelSize);
                return ENGINE_INVALID_ID;
            }
        }

        it->second.lastUsedFrame = ++m_accessCounter;
        return it->second.glTextureID;
    }

    void FontAsset::DeleteFontAtlas(int pixelSize) {
        auto it = m_atlases.find(pixelSize);
        if (it != m_atlases.end()) {
            glDeleteTextures(1, &it->second.glTextureID);
            m_atlases.erase(it);
            return;
        }
        Log::Warn("FontAsset: Could not delete font atlas (size {}), font was not found", pixelSize);
    }

    void FontAsset::DeleteAllAtlases() {
        for (auto& [size, atlas] : m_atlases) {
            if (atlas.glTextureID != ENGINE_INVALID_ID) {
                glDeleteTextures(1, &atlas.glTextureID);
                atlas.glTextureID = ENGINE_INVALID_ID;
            }
        }
        m_atlases.clear();
    }

    void FontAsset::BuildAtlas(int pixelSize) {
        auto it = m_atlases.find(pixelSize);
        if (it != m_atlases.end()) {
            Log::Warn("FontAsset: font pixelSize already build!");
            return;
        }

        FT_Error error = FT_Set_Pixel_Sizes(m_face, 0, pixelSize);
        if (error) {
            Log::Error("FT_Set_Pixel_Sizes failed with error code {}", error);
            return;
        }

        // ASCII 32–128
        const int firstChar = 32;
        const int lastChar = 128;

        int atlasWidth = 0;
        int atlasHeight = 0;

        for (int c = firstChar; c < lastChar; ++c) {
            if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
                Log::Error("FontAsset: Failed to load Glyph '{}'", c);
                continue;
            }
            atlasWidth += m_face->glyph->bitmap.width;
            atlasHeight = std::max(atlasHeight, (int)m_face->glyph->bitmap.rows);
        }

        std::vector<unsigned char> atlasBuffer(atlasWidth * atlasHeight, 0);

        int xOffset = 0;
        Atlas atlas;

        for (int c = firstChar; c < lastChar; ++c) {
            if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) continue;

            FT_Bitmap& bmp = m_face->glyph->bitmap;

            // Copy bitmap into atlas buffer
            for (unsigned int row = 0; row < bmp.rows; ++row) {
                for (unsigned int col = 0; col < bmp.width; ++col) {
                    int x = xOffset + col;
                    int y = row;
                    size_t dstIndex = static_cast<size_t>(y) * static_cast<size_t>(atlasWidth) + static_cast<size_t>(x);
                    size_t srcIndex = static_cast<size_t>(row) * static_cast<size_t>(bmp.pitch) + static_cast<size_t>(col);

                    atlasBuffer[dstIndex] = bmp.buffer[srcIndex];
                }
            }

            Glyph glyph;
            glyph.size = Vector2((float)bmp.width, (float)bmp.rows);
            glyph.bearing = Vector2((float)m_face->glyph->bitmap_left, (float)m_face->glyph->bitmap_top);
            glyph.advance = static_cast<unsigned int>(m_face->glyph->advance.x >> 6);

            // Normierte UVs
            glyph.uvMin = Vector2((float)xOffset / atlasWidth, 0.0f);
            glyph.uvMax = Vector2((float)(xOffset + bmp.width) / atlasWidth,
                (float)bmp.rows / atlasHeight);

            atlas.glyphs.emplace((char)c, glyph);

            xOffset += bmp.width;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        unsigned int texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RED,
            atlasWidth,
            atlasHeight,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            atlasBuffer.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        atlas.lastUsedFrame = ++m_accessCounter;
        atlas.glTextureID = texID;
        m_atlases[pixelSize] = atlas;

        EnforceAtlasLimit();
    }

    void FontAsset::EnforceAtlasLimit() {
        while (m_atlases.size() > m_maxAtlases) {
            auto lru = std::min_element(
                m_atlases.begin(),
                m_atlases.end(),
                [](const auto& a, const auto& b) {
                    return a.second.lastUsedFrame < b.second.lastUsedFrame;
                }
            );

            if (lru == m_atlases.end())
                break;

            Log::Debug("FontAsset: Deleted least recently used atlas (size {})", lru->first);
            Log::Print(Log::levelDebug, "                    {}", m_path);

            glDeleteTextures(1, &lru->second.glTextureID);
            m_atlases.erase(lru);
        }
    }

}