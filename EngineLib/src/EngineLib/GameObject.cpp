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

	GameObject* GameObject::Create(const std::string& name) {
		#ifndef NDEBUG
		if (GameObjectManager::IsNameUnique(name)) {
			Log::Warn("GameObject: name is not unique");
		}
		#endif
		GameObject* go = new GameObject(name);
		go->m_id = GameObjectManager::GetNewUniqueIdentifier();
		GameObjectManager::AddGameObject(go);
		return go;
	}

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
			auto& siblings = m_parentObjPtr->m_childObjPtrs;
			siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
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
}
