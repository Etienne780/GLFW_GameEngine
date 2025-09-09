#pragma once
#include <string>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <CoreLib/Math/Vector2.h>

#include "EngineTypes.h"

namespace EngineCore {

	class FontAsset {
	public:
		FontAsset(const FT_Library& lib, const std::string& path, bool useAbsolutDir);
		~FontAsset();

		struct Glyph {
			Vector2 size;       // Size of glyph in pixels
			Vector2 bearing;    // Offset from baseline
			unsigned int advance = 0;
			Vector2 uvMin;      // Bottom-left corner in atlas
			Vector2 uvMax;      // Top-right corner in atlas
		};

		const Glyph& GetGlyph(char c, int pixelSize);
		unsigned int GetAtlasTextureID(int pixelSize);

		void BuildAtlas(int pixelSize);

		void DeleteFontAtlas(int pixelSize);
		void DeleteAllAtlases();
	private:	
		std::string m_path;
		FT_Face m_face;

		struct Atlas {
			unsigned int glTextureID = ENGINE_INVALID_ID;
			std::unordered_map<char, Glyph> glyphs;
		};

		std::unordered_map<int, Atlas> m_atlases;
	};

}