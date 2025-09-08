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

	private:
		struct Glyph {
			unsigned int m_glTextureID;	// ID handle of the glyph texture
			Vector2	m_size;				// Size of glyph
			Vector2	m_bearing;			// Offset from baseline to left/top of glyph
			unsigned int m_advance;		// Offset to advance to next glyph
		};

		std::string m_path;
		FT_Face m_face;
		std::unordered_map<char, Glyph> m_glyphs;

		void LoadBaseGlyphs();
	};
	
}