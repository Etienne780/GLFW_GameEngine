#include <CoreLib/File.h>
#include <CoreLib/Log.h>

#include "EngineLib/FontManager.h"

namespace EngineCore {

	int FontManager::Init() {
		m_init = true;

		FT_Error err = FT_Init_FreeType(&m_ftLib);
		if (err) {
			Log::Error("Failed to init FreeType: {}", err);
			return ENGINE_FAILURE;
		}

		Log::Info("Engine::FreeType: Initialized FreeType successfully");
		return ENGINE_SUCCESS;
	}
	
	void FontManager::Shutdown() {
		m_fonts.clear();
		FT_Done_FreeType(m_ftLib);
		m_init = false;
	}

	FontID FontManager::LoadFont(const std::string& path, bool useAbsolutDir) {
#ifndef NDEBUG
		if (!m_init) {
			Log::Error("FontManager: Failed to load font, FontManager was not initiated");
			return FontID(ENGINE_INVALID_ID);
		}
#endif 
		int id = m_idManager.GetNewUniqueIdentifier();
		if (id == ENGINE_INVALID_ID) {
			Log::Error("FontManager: No free font id was found");
		}
		else {
			m_fonts.emplace(id, std::make_unique<FontAsset>(m_ftLib, path, useAbsolutDir));
		}

		return FontID(id);
	}

	std::shared_ptr<FontAsset> FontManager::GetFont(FontID id) {
#ifndef NDEBUG
		if (!m_init) {
			Log::Error("FontManager: Failed to get font, FontManager was not initiated");
			return nullptr;
		}
#endif 
		auto it = m_fonts.find(id);
		if (it == m_fonts.end()) {
			Log::Error("FontManager: Font({}) not found", id.value);
			return nullptr;
		}
		return it->second;
	}

	std::vector<TextQuad> FontManager::BuildTextQuads(
		const std::string& text,
		FontID fontID,
		Vector2 startPos,
		int pixelSize,
		float scale) {
#ifndef NDEBUG
		if (!m_init) {
			Log::Error("FontManager: Failed to build textQuads, FontManager was not initiated");
			return std::vector<TextQuad>();
		}
#endif 
		auto fontAsset = FontManager::GetFont(fontID);
		if (!fontAsset) {
			Log::Error("FontManager: Invalid fontID");
			return {};
		}

		std::vector<TextQuad> result;
		result.reserve(text.size());
		float x = startPos.x;
		float y = startPos.y;

		for (char c : text) {
			const FontAsset::Glyph& g = fontAsset->GetGlyph(c, pixelSize);

			float xpos = x + g.bearing.x * scale;
			float ypos = y - (g.size.y - g.bearing.y) * scale;

			float w = g.size.x * scale;
			float h = g.size.y * scale;
			float xOffset = 0;
			float yOffset = 0;

			TextQuad q{};

			// Positions
			q.vertices[0].position = { xpos,     ypos + h, 0.0f };
			q.vertices[1].position = { xpos,     ypos,     0.0f };
			q.vertices[2].position = { xpos + w, ypos,     0.0f };
			q.vertices[3].position = { xpos + w, ypos + h, 0.0f };

			// UVs
			q.vertices[0].uv = { g.uvMin.x + xOffset, g.uvMin.y - yOffset };// top left
			q.vertices[1].uv = { g.uvMin.x + xOffset, g.uvMax.y + yOffset };// bottom left
			q.vertices[2].uv = { g.uvMax.x - xOffset, g.uvMax.y + yOffset };// bottom right
			q.vertices[3].uv = { g.uvMax.x - xOffset, g.uvMin.y - yOffset };// top right

			result.push_back(q);

			// Advance cursor
			x += g.advance * scale;
		}

		return result;
	}
	
	const FontAsset::Glyph& FontManager::GetGlyph(FontID id, char c, int pixelSize) {
#ifndef NDEBUG
		if (!m_init) {
			Log::Error("FontManager: Failed to get Glyph, FontManager was not initiated");
			static FontAsset::Glyph dummy{};
			return dummy;
		}
#endif 
		return GetFont(id)->GetGlyph(c, pixelSize);
	}

	unsigned int FontManager::GetAtlasTextureID(FontID id, int pixelSize) {
#ifndef NDEBUG
		if (!m_init) {
			Log::Error("FontManager: Failed to get AtlasTextureID, FontManager was not initiated");
			return ENGINE_INVALID_ID;
		}
#endif 
		return GetFont(id)->GetAtlasTextureID(pixelSize);
	}

}