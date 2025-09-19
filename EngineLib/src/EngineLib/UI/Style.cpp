#include "EngineLib/UI/Style.h"

namespace EngineCore::UI {

	void Style::Extend(std::shared_ptr<Style> style) {
		m_styles.emplace_back(style);
	}

	void Style::Set(const char* name, std::any value) {
		m_attributes[name] = std::move(value);
	}

	const std::unordered_map<const char*, std::any>& Style::GetAll() const {
		return m_attributes;
	}

	void Style::GenerateCachedStyle() {
		m_cachedStyle = std::make_unique<Style>();
		for (auto& style : m_styles) {
			const auto& att = style->GetAll();
			for (auto& [name, value] : att) {
				m_cachedStyle->Set(name, value);
			}
		}

		for (auto& [name, value] : m_attributes) {
			m_cachedStyle->Set(name, value);
		}
		
	}

}