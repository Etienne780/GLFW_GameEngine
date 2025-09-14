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
	friend class ResourceManager;
	public:
		static std::vector<TextQuad> BuildTextQuads(const std::string& text, FontID fontID, Vector2 startPos, int pixelSize, float scale);

	private:
		FontManager();

		inline static bool m_init = false;
		inline static FT_Library m_ftLib = nullptr;

		inline static unsigned int m_textVAO = ENGINE_INVALID_ID;
		inline static unsigned int m_textVBO = ENGINE_INVALID_ID;
		inline static unsigned int m_textEBO = ENGINE_INVALID_ID;

		static int Init();
		static void Shutdown();

		static FT_Library& GetFTLib();
		static void InitTextRenderer();
		static void ShutdownTextRenderer();
		static void DrawQuad(const TextQuad& quad);
	};

}