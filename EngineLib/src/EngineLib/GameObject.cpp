#include "EngineLib\GameObjectManager.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\Component.h"

namespace EngineCore {

	GameObject::GameObject(const std::string& name)
		: m_name(name) {
	}

	GameObject* GameObject::Create(const std::string& name) {
		GameObject* go = new GameObject(name);
		GameObjectManager::AddGameObject(go);
		return go;
	}

	std::string GameObject::GetName() const {
		return m_name;
	}
}
