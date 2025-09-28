#include "EngineLib/UI/Style.h"

namespace EngineCore::UI {

	Style::Style(const std::string& name)
		:m_name(name){
	}

	std::shared_ptr<Style> Style::Create(const std::string& name) {
		return std::make_shared<Style>(name);
	}

	std::shared_ptr<Style> Style::Create(const std::string& name, std::shared_ptr<Style> style) {
		auto s = std::make_shared<Style>(name);
		s->Extend(style);
		return s;
	}

	void Style::Extend(std::shared_ptr<Style> style) {
		m_extendedStyles.emplace_back(style);
		m_newStyleAdded = true;
	}

	void Style::Set(const char* name, std::string value) {
		Set(State::Normal, name, value);
	}

	void Style::Set(State state, const char* name, std::string value) {
		m_attributes[state][name] = value;
		m_newStyleAdded = true;
	}

	std::string Style::Get(const char* name) const {
		return Get(State::Normal, name);
	}

	std::string Style::Get(State state, const char* name) const {
		if (m_newStyleAdded) {
			m_newStyleAdded = false;
			GenerateCachedStyle();
		}

		const auto& attState = m_cachedStyle->m_attributes;

		// First try the requested state
		auto itState = attState.find(state);
		if (itState != attState.end()) {
			const auto& attMap = itState->second;
			auto itAttr = attMap.find(name);
			if (itAttr != attMap.end()) {
				return itAttr->second;
			}
		}

		// Fallback: try Normal state
		if (state != State::Normal) {
			auto itNormal = attState.find(State::Normal);
			if (itNormal != attState.end()) {
				const auto& attMap = itNormal->second;
				auto itAttr = attMap.find(name);
				if (itAttr != attMap.end()) {
					return itAttr->second;
				}
			}
		}

		Log::Warn("Style: Could not get attribute '{}' on style '{}'!", name, m_name);
		return "-";
	}

	const std::string& Style::GetName() const {
		return m_name;
	}

	const std::unordered_map<State, std::unordered_map<std::string, std::string>>& Style::GetAll() const {
		if (m_newStyleAdded) {
			m_newStyleAdded = false;
			GenerateCachedStyle();
		}
		return m_cachedStyle->m_attributes;
	}

	const std::unordered_map<std::string, std::string>& Style::GetAllState(State state) const {
		if (m_newStyleAdded) {
			m_newStyleAdded = false;
			GenerateCachedStyle();
		}
		return m_cachedStyle->m_attributes[state];
	}

	void Style::GenerateCachedStyle() const {
		m_cachedStyle = std::make_unique<Style>();
		// applys all the extended styls in the right order
		for (auto& style : m_extendedStyles) {
			const auto& att = style->GetAll();
			for (auto& [state, attMap] : att) {
				for (auto& [name, value] : attMap) {
					m_cachedStyle->Set(state, name.c_str(), value);
				}
			}
		}

		// applys the att of this style last
		for (auto& [state, attMap] : m_attributes) {
			for (auto& [name, value] : attMap) {
				m_cachedStyle->Set(state, name.c_str(), value);
			}
		}
	}

}