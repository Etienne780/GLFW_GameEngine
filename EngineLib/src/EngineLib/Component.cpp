#include "EngineLib\Component.h"

namespace EngineCore {

	Component::Component(const std::string& name)
		: m_name(name) {
	}

	GameObject* Component::GetGameObject() const {
		return m_owner;
	}

	std::string Component::GetName() const {
		return m_name.empty() ? "Unknown" : m_name;
	}

}