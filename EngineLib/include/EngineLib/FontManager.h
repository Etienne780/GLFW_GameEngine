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
	public:
		static int Init();
		static void Shutdown();

		static FontID LoadFont(const std::string& path, bool useAbsolutDir = false);
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
	};

}