#include "EngineLib/UI/Style.h"

namespace EngineCore::UI {

	void Style::Extend(const Style& style) {
		const auto att = style.GetAll();
		for (const auto& [name, value] : att) {
			Set(name.c_str(), value);
		}
	}

	void Style::Set(const char* name, std::any value) {
		m_attributes[name] = std::move(value);
	}

	const std::unordered_map<std::string, std::any>& Style::GetAll() const {
		return m_attributes;
	}

}