#include "EngineLib/UI/UIStyle.h"

namespace EngineCore {
	
	void UIStyle::Extend(const UIStyle& style) {
		const auto att = style.GetAll();
		for (const auto& [name, value] : att) {
			Set(name.c_str(), value);
		}
	}

	void UIStyle::Set(const char* name, std::any value) {
		m_attributes[name] = std::move(value);
	}

	const std::unordered_map<std::string, std::any>& UIStyle::GetAll() const {
		return m_attributes;
	}

}