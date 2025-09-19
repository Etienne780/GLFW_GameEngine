#include "EngineLib/UI/Style.h"

namespace EngineCore::UI {

	std::shared_ptr<Style> Style::Create() {
		return std::make_shared<Style>();
	}

	std::shared_ptr<Style> Style::Create(std::shared_ptr<Style> style) {
		auto s = std::make_shared<Style>();
		s->Extend(style);
		return s;
	}

	void Style::Extend(std::shared_ptr<Style> style) {
		m_styles.emplace_back(style);
		m_newStyleAdded = true;
	}

	void Style::Set(const char* name, const StyleValue& value) {
		m_attributes[name] = std::move(value);
		m_newStyleAdded = true;
	}

	const std::unordered_map<std::string, StyleValue>& Style::GetAll() const {
		if (m_newStyleAdded) {
			m_newStyleAdded = false;
			GenerateCachedStyle();
		}
		return m_cachedStyle->m_attributes;
	}


	void Style::GenerateCachedStyle() const {
		m_cachedStyle = std::make_unique<Style>();
		// applys all the extended styls in the right order
		for (auto& style : m_styles) {
			const auto& att = style->GetAll();
			for (auto& [name, value] : att) {
				m_cachedStyle->Set(name.c_str(), value);
			}
		}
		// applys the att of this style last
		for (auto& [name, value] : m_attributes) {
			m_cachedStyle->Set(name.c_str(), value);
		}
		
	}

}