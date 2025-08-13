#include "CoreLib\FormatUtils.h"

#include "EngineLib\Component.h"

namespace EngineCore {

	Component::Component(const std::string& name)
		: m_name(name) {
	}

	GameObject* Component::GetGameObject() const {
		return m_gameObject;
	}

	std::string Component::GetName() const {
		return m_name.empty() ? "Unknown" : m_name;
	}

	std::string Component::GetComponentString() const {
		std::string output;
		GetComponentString("", output, false);
		return output;
	}

	std::string Component::GetComponentString(bool moreDetail) const {
		std::string output;
		GetComponentString("", output, moreDetail);
		return output;
	}

	void Component::GetComponentString(const std::string& prefix, std::string& outStr, bool moreDetail) const {
		if (prefix.empty()) {
			outStr.append(FormatUtils::formatString("{}\n", GetName()));
		}
		else {
			outStr.append(FormatUtils::formatString("{}{}\n", prefix, GetName()));
		}
	}
}
