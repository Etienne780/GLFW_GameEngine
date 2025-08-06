#include "EngineLib\GameObjectManager.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\Component.h"

namespace EngineCore {

	GameObject::GameObject(const std::string& name)
		: m_name(name) {
	}

	GameObject::~GameObject() {
		m_components.clear();
		m_childObjPtrs.clear();
		m_parentObjPtr = nullptr;
	}

	#pragma region Static

	GameObject* GameObject::Create(const std::string& name) {
		#ifndef NDEBUG
		if (!GameObjectManager::IsNameUnique(name)) {
			Log::Warn("GameObject: name is not unique");
		}
		#endif
		GameObject* go = new GameObject(name);
		go->m_id = GameObjectManager::GetNewUniqueIdentifier();
		GameObjectManager::AddGameObject(go);
		return go;
	}

	bool GameObject::Delete(GameObject* gameObjectPtr) {
		return GameObjectManager::DeleteGameObject(gameObjectPtr);
	}

	bool GameObject::Delete(unsigned int id) {
		return GameObjectManager::DeleteGameObject(id);
	}

	bool GameObject::Delete(const std::string& name) {
		return GameObjectManager::DeleteGameObject(name);
	}

	#pragma endregion

	bool GameObject::HasParent() const {
		return (m_parentObjPtr != nullptr);
	}

	#pragma region Get

	std::string GameObject::GetName() const {
		return m_name;
	}

	unsigned int GameObject::GetID() const {
		return m_id;
	}

	GameObject* GameObject::GetParent() const {
		return m_parentObjPtr;
	}

	#pragma endregion

	void GameObject::SetParent(GameObject * parentPtr) {
		if (m_parentObjPtr == parentPtr)
			return;

		if (this == parentPtr) {
			Log::Warn("GameObject: Could not set parent to itself");
			return;
		}
		
		// remove self form parents child list
		if (m_parentObjPtr) {
			m_parentObjPtr->RemoveChild(this);
		}

		m_parentObjPtr = parentPtr;
		// add selft to parents child list
		if (m_parentObjPtr) {
			m_parentObjPtr->m_childObjPtrs.push_back(this);
		}
	}

	void GameObject::Detach() {
		SetParent(nullptr);
	}

	void GameObject::RemoveChild(GameObject* child) {
		auto& children = m_childObjPtrs;
		children.erase(std::remove(children.begin(), children.end(), child), children.end());
	}

}
