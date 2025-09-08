#pragma once
#include <string>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

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

	private:
		FontManager();

		inline static bool m_init = false;
		inline static FT_Library m_ft;
		inline static IDManager m_idManager;

		inline static std::unordered_map<FontID, FontAsset> m_fonts;
	};

}