#include <glad\glad.h>
#include <CoreLib/File.h>
#include <CoreLib/Log.h>

#include "EngineLib/ResourceManager.h"
#include "EngineLib/FontManager.h"

namespace EngineCore {

	int FontManager::Init() {
		if (m_init)
			return ENGINE_SUCCESS;

		m_init = true;

		FT_Error err = FT_Init_FreeType(&m_ftLib);
		if (err) {
			Log::Error("Failed to init FreeType: {}", err);
			return ENGINE_FAILURE;
		}

		InitTextRenderer();
		Log::Info("Engine::FreeType: Initialized FreeType successfully");
		return ENGINE_SUCCESS;
	}
	
	void FontManager::Shutdown() {
		if (!m_init)
			return;

		FT_Done_FreeType(m_ftLib);
		ShutdownTextRenderer();
		m_init = false;
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
		auto fontAsset = ResourceManager::GetInstance()->GetFontAsset(fontID);
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

	FT_Library& FontManager::GetFTLib() {
		return m_ftLib;
	}

	void FontManager::InitTextRenderer() {
		glGenVertexArrays(1, &m_textVAO);
		glGenBuffers(1, &m_textVBO);
		glGenBuffers(1, &m_textEBO);

		glBindVertexArray(m_textVAO);

		// VBO für 4 Vertices
		glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TextVertex) * 4, nullptr, GL_DYNAMIC_DRAW);

		// EBO für Quad-Indizes
		unsigned int indices[6] = { 0,1,2,2,3,0 };
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_textEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Vertex-Attribs: position (vec3) + uv (vec2)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, uv));

		glBindVertexArray(0);
	}

	void FontManager::ShutdownTextRenderer() {
		if (m_textVAO != ENGINE_INVALID_ID) {
			glDeleteTextures(1, &m_textVAO);
			m_textVAO = ENGINE_INVALID_ID;
		}
		if (m_textVBO != ENGINE_INVALID_ID) {
			glDeleteTextures(1, &m_textVBO);
			m_textVBO = ENGINE_INVALID_ID;
		}
		if (m_textEBO != ENGINE_INVALID_ID) {
			glDeleteTextures(1, &m_textEBO);
			m_textEBO = ENGINE_INVALID_ID;
		}
	}

	void FontManager::DrawQuad(const TextQuad& quad) {
		glBindVertexArray(m_textVAO);

		// Quad-Daten hochladen (dynamisch)
		glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad.vertices), quad.vertices);

		// Zeichnen
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	
}