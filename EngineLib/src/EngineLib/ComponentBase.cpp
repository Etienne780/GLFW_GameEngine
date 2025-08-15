#include "CoreLib\FormatUtils.h"
#include "CoreLib\Log.h"

#include "EngineLib\ComponentBase.h"

namespace EngineCore {

	ComponentBase::ComponentBase(const std::string& name, GameObject* gameObject)
		: m_name(name), m_gameObject(gameObject) {
	}

	GameObject* ComponentBase::GetGameObject() const {
		return m_gameObject;
	}

	std::string ComponentBase::GetName() const {
		return m_name.empty() ? "Unknown" : m_name;
	}

	std::string ComponentBase::GetComponentString() const {
		std::string output;
		GetComponentString("", output, false);
		return output;
	}

	std::string ComponentBase::GetComponentString(bool moreDetail) const {
		std::string output;
		GetComponentString("", output, moreDetail);
		return output;
	}

	void ComponentBase::GetComponentString(const std::string& prefix, std::string& outStr, bool moreDetail) const {
		if (prefix.empty()) {
			outStr.append(FormatUtils::formatString("{}\n", GetName()));
		}
		else {
			outStr.append(FormatUtils::formatString("{}{}\n", prefix, GetName()));
		}
	}

	bool ComponentBase::IsDead(const std::string& msg) const {
		if (!m_alive) {
			Log::Warn("{}: {}, Component was deleted!", m_name, msg);
			return true;
		}
		return false;
	}

}
