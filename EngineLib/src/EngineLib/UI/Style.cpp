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
		Set(State::Normal, name, value);
	}

	void Style::Set(State state, const char* name, const StyleValue& value) {
		m_attributes[state][name] = value;
		m_newStyleAdded = true;
	}

	StyleValue Style::Get(const char* name) const {
		return Get(State::Normal, name);
	}

	StyleValue Style::Get(State state, const char* name) const {
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

		Log::Warn("Style: Could not get attribute '{}'!", name);
		return StyleValue{};
	}

	const std::unordered_map<State, std::unordered_map<std::string, StyleValue>>& Style::GetAll() const {
		if (m_newStyleAdded) {
			m_newStyleAdded = false;
			GenerateCachedStyle();
		}
		return m_cachedStyle->m_attributes;
	}

	const std::unordered_map<std::string, StyleValue>& Style::GetAllState(State state) const {
		if (m_newStyleAdded) {
			m_newStyleAdded = false;
			GenerateCachedStyle();
		}
		return m_cachedStyle->m_attributes[state];
	}

	void Style::GenerateCachedStyle() const {
		m_cachedStyle = std::make_unique<Style>();
		// applys all the extended styls in the right order
		for (auto& style : m_styles) {
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