#pragma once
#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <CoreLib/Math/Vector2.h>

#include "EngineTypes.h"
#include "IDManager.h"
#include "FontAsset.h"

namespace EngineCore {

	// Constant to improve code readability when passing 'useAbsolutDir'.
	inline const bool FONT_ABSOLUTDIR = true;

	class FontManager {
	friend class Renderer;
	public:
		static int Init();
		static void Shutdown();

		static FontID LoadFont(const std::string& path, bool useAbsolutDir = false);
		/*
		* @brief IMPORTANT the data ptr has to be alive as long as this FontAsset exist
		*/
		static FontID LoadFontMemory(const FT_Byte* data, FT_Long size);
		static std::shared_ptr<FontAsset> GetFont(FontID id);
		static std::vector<TextQuad> BuildTextQuads(const std::string& text, FontID fontID, Vector2 startPos, int pixelSize, float scale);
		
		static const FontAsset::Glyph& GetGlyph(FontID id, char c, int pixelSize);
		static unsigned int GetAtlasTextureID(FontID id, int pixelSize);

	private:
		FontManager();

		inline static bool m_init = false;
		inline static FT_Library m_ftLib = nullptr;
		inline static IDManager m_idManager;

		inline static std::unordered_map<FontID, std::shared_ptr<FontAsset>> m_fonts;

		inline static unsigned int m_textVAO = ENGINE_INVALID_ID;
		inline static unsigned int m_textVBO = ENGINE_INVALID_ID;
		inline static unsigned int m_textEBO = ENGINE_INVALID_ID;

		static void InitTextRenderer();
		static void ShutdownTextRenderer();
		static void DrawQuad(const TextQuad& quad);
	};

}