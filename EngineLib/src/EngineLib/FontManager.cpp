#include <CoreLib/File.h>
#include <CoreLib/Log.h>

#include "EngineLib/FontManager.h"

namespace EngineCore {

	int FontManager::Init() {
		m_init = true;

		if (FT_Init_FreeType(&m_ft)) {
			Log::Error("FontManager: could not init FreeType Library"); 
			return ENGINE_FAILURE;
		}

		return ENGINE_SUCCESS;
	}
	
	void FontManager::Shutdown() {
		m_fonts.clear();
		FT_Done_FreeType(m_ft);
		m_init = false;
	}

	FontID FontManager::LoadFont(const std::string& path, bool useAbsolutDir) {
#ifndef NDEBUG
		if (!m_init) {
			Log::Error("FontManager: Failed to load font, FontManager was not initiated");
			return FontID(ENGINE_INVALID_ID);
		}
#endif 
		FontAsset font(m_ft, path, useAbsolutDir);

		int id = m_idManager.GetNewUniqueIdentifier();
		if (id == ENGINE_INVALID_ID) {
			Log::Error("FontManager: No free font id was found");
		}
		else {
			m_fonts.emplace(id, font);
		}

		return FontID(id);
	}

}