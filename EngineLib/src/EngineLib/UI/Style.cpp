#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/UIManager.h"
#include "EngineLib/UI/Style.h"

namespace EngineCore::UI {

	Style::Style(const std::string& name)
		:m_name(name){
	}

	std::shared_ptr<Style> Style::Create(const std::string& name) {
		auto s = std::make_shared<Style>(name);
		return s;
	}

	std::shared_ptr<Style> Style::Create(const std::string& name, std::shared_ptr<Style> style) {
		auto s = std::make_shared<Style>(name);
		s->Extend(style);
		return s;
	}

	void Style::Extend(std::shared_ptr<Style> style) {
		m_extendedStyles.emplace_back(style);
		style->SubDirtCallbackInter([this]() { SetStyleDirty(); });
		SetStyleDirty();
	}

	void Style::Set(const char* name, const std::string& value) {
		Set(State::Normal, name, value);
	}

	void Style::Set(const std::string& name, const std::string& value) {
		Set(State::Normal, name.c_str(), value);
	}

	void Style::Set(State state, const char* name, const std::string& value) {
		m_attributes[state][name] = value;
		SetStyleDirty();
	}

	void Style::Remove(const std::string& name) {
		Remove(State::Normal, name);
	}

	void Style::Remove(State state, const std::string& name) {
		auto outerIt = m_attributes.find(state);
		if (outerIt != m_attributes.end()) {
			outerIt->second.erase(name);

			if (outerIt->second.empty()) {
				m_attributes.erase(outerIt);
			}
			SetStyleDirty();
		}
	}

	void Style::Clear() {
		m_attributes.clear();

	}

	void Style::Clear(UI::State state) {
		auto it = m_attributes.find(state);
		if (it != m_attributes.end())
			it->second.clear();

		SetStyleDirty();
	}

	std::string Style::Get(const char* name) const {
		return Get(State::Normal, name);
	}

	std::string Style::Get(State state, const char* name) const {
		if (m_styleDirty) {
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
		if (m_styleDirty) {
			GenerateCachedStyle();
		}
		return m_cachedStyle->m_attributes;
	}

	const std::unordered_map<std::string, std::string>& Style::GetAllState(State state) const {
		if (m_styleDirty) {
			GenerateCachedStyle();
		}
		return m_cachedStyle->m_attributes[state];
	}

	const std::unordered_map<State, std::unordered_map<std::string, std::string>>& Style::GetAllLocal() const {
		return m_attributes;
	}

	const std::unordered_map<std::string, std::string>& Style::GetAllStateLocal(State state) const {
		return m_attributes[state];
	}

	const std::vector<std::shared_ptr<Style>> Style::GetAllExtendedStyles() const {
		return m_extendedStyles;
	}

	Style::SubscriberID Style::SubDirtCallback(StyleDirtyCallback callback) {
		m_dirtyCallback.push_back({ ++m_dirtyCallbackID, callback });
		return m_dirtyCallbackID;
	}

	Style::SubscriberID Style::SubDirtCallbackInter(StyleDirtyCallback callback) {
		m_dirtyCallbackInter.push_back({ ++m_dirtyCallbackIDInter, callback });
		return m_dirtyCallbackIDInter;
	}

	void Style::UnsubDirtyCallback(SubscriberID id) {
		auto it = std::find_if(m_dirtyCallback.begin(), m_dirtyCallback.end(),
			[id](const Subscriber<StyleDirtyCallback>& sub) { return id == sub.id; }
		);

		if (it != m_dirtyCallback.end()) {
			m_dirtyCallback.erase(it);
		}
	}

	void Style::SetStyleDirty() {
		m_styleDirty = true;
		CallDirty();
	}

	void Style::GenerateCachedStyle() const {
		m_cachedStyle = std::make_unique<Style>();

		// applys extended styles 
		for (const auto& extStyle : m_extendedStyles) {
			const auto& extAll = extStyle->GetAll();
			for (const auto& [state, attMap] : extAll) {
				auto& targetMap = m_cachedStyle->m_attributes[state];
				for (const auto& [name, value] : attMap) {
					targetMap[name] = value;
				}
			}
		}

		// copys the other stats in to the cached style 
		for (const auto& [state, attMap] : m_attributes) {
			auto& targetMap = m_cachedStyle->m_attributes[state];
			for (const auto& [name, value] : attMap) {
				targetMap[name] = value;
			}
		}

		m_styleDirty = false;
	}

	void Style::CallDirty() {
		if (m_dirtyCallbackInter.empty() && m_dirtyCallback.empty())
			return;

#ifndef NDEBUG
		if (UIManager::GetDebug())
			Log::Debug("Style: called Dirt Callbacks with '{}' Inter and '{}' normal", m_dirtyCallbackInter.size(), m_dirtyCallback.size());
#endif

		if (m_dirtyCallbackInter.size() > 10 && m_dirtyCallback.size() > 10) {
			std::vector<Subscriber<StyleDirtyCallback>> callbacks;
			callbacks.reserve(m_dirtyCallbackInter.size() + m_dirtyCallback.size());
			callbacks.insert(callbacks.end(), m_dirtyCallbackInter.begin(), m_dirtyCallbackInter.end());
			callbacks.insert(callbacks.end(), m_dirtyCallback.begin(), m_dirtyCallback.end());

			for (const auto& sub : callbacks) {
				if (sub.callback)
					sub.callback();
			}
		}
		else {
			for (const auto& sub : m_dirtyCallbackInter) {
				if (sub.callback)
					sub.callback();
			}

			for (const auto& sub : m_dirtyCallback) {
				if (sub.callback)
					sub.callback();
			}
		}
	}

}