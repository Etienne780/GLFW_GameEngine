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
		/*
		* @brief creats a FontAsset from a file path
		*/
		FontAsset(const FT_Library& lib, const std::string& path, bool useAbsolutDir);
		/*
		* @brief Creats a FontAsset from raw memory
		*/
		FontAsset(const FT_Library& lib, const FT_Byte* data, FT_Long size);
		~FontAsset();

		FontAsset(const FontAsset&) = delete;
		FontAsset& operator=(const FontAsset&) = delete;

		FontAsset(FontAsset&&) = delete;
		FontAsset& operator=(FontAsset&&) = delete;

		struct Glyph {
			Vector2 size;       // Size of glyph in pixels
			Vector2 bearing;    // Offset from baseline
			unsigned int advance = 0;
			Vector2 uvMin;      // Bottom-left corner in atlas
			Vector2 uvMax;      // Top-right corner in atlas
		};

		void SetNumberOfMaxAtlases(size_t maxAtlases);

		const Glyph& GetGlyph(char c, int pixelSize);
		unsigned int GetAtlasTextureID(int pixelSize);

		void BuildAtlas(int pixelSize);

		void DeleteFontAtlas(int pixelSize);
		void DeleteAllAtlases();
	private:
		// says if the FontAsset was created from memory
		bool m_isFromMemory = false;
		std::string m_path;
		FT_Face m_face;
		size_t m_maxAtlases = 8;

		struct Atlas {
			unsigned int glTextureID = ENGINE_INVALID_ID;
			std::unordered_map<char, Glyph> glyphs;
			mutable uint64_t lastUsedFrame = 0; // gets updated whenever atlas is accessed
		};
		
		std::unordered_map<int, Atlas> m_atlases;
		uint64_t m_accessCounter = 0;

		void EnforceAtlasLimit();
	};

}