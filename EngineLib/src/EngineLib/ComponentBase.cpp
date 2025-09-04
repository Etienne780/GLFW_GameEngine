#include "CoreLib/FormatUtils.h"
#include "CoreLib/Log.h"

#include "EngineLib/GameObject.h"
#include "EngineLib/ComponentBase.h"

namespace EngineCore {

	ComponentBase::ComponentBase(const std::string& name, unsigned int goID)
		: m_name(name), m_gameObjectID(goID) {
	}

	ComponentBase::ComponentBase(const std::string& name, ExecutionOrder executionOrder, unsigned int goID)
		: m_name(name), m_executionOrder(executionOrder), m_gameObjectID(goID) {
	}

	void ComponentBase::CUpdate() {
		UpdateAlwaysImpl();
		if (m_isDisabled) 
			return;
		UpdateImpl();
	}

	void ComponentBase::CSubmitDrawCall() {
		if (m_isDisabled)
			return;
		SubmitDrawCall();
	}

	void ComponentBase::Disable(bool value) {
		if (!CanDisalbe()) 
			return;
		m_isDisabled = value;
	}

	bool ComponentBase::IsDisable() const {
		return m_isDisabled;
	}

	std::shared_ptr<GameObject> ComponentBase::GetGameObject() const {
		if (IsDead("Cant get GameObject")) {
			return nullptr;
		}

		if(!m_gameObject)
			m_gameObject = GameObject::Get(m_gameObjectID);
		return m_gameObject;
	}

	unsigned int ComponentBase::GetGameObjectID() const {
		if (IsDead("Cant get GameObjectID")) {
			return ENGINE_INVALID_ID;
		}

		return m_gameObjectID;
	}

	const unsigned int* ComponentBase::GetGameObjectIDPtr() const {
		if (IsDead("Cant get GameObjectID")) {
			return nullptr;
		}

		return GetGameObject()->GetIDPtr();
	}

	std::string ComponentBase::GetName() const {
		if (IsDead("Cant get name")) {
			return "INVALID";
		}
		return m_name.empty() ? "Unknown" : m_name;
	}

	std::string ComponentBase::GetComponentString() const {
		if (IsDead("Cant get Component string")) {
			return "INVALID";
		}
		std::string output;
		GetComponentString("", output, false);
		return output;
	}

	std::string ComponentBase::GetComponentString(bool moreDetail) const {
		if (IsDead("Cant get Component string")) {
			return "INVALID";
		}
		std::string output;
		GetComponentString("", output, moreDetail);
		return output;
	}

	void ComponentBase::GetComponentString(const std::string& prefix, std::string& outStr, bool moreDetail) const {
		if (IsDead("Cant get Component string")) {
			return;
		}
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
