#include "EngineLib\GameObjectManager.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\ComponentBase.h"

namespace EngineCore {

	GameObjectManager* GameObject::m_gameObjectManager = nullptr;

	GameObject::GameObject(unsigned int id, const std::string& name)
		: m_id(id), m_name(name), m_transform(this){
	}

	GameObject::~GameObject() {
		m_components.clear();
		m_childObjPtrs.clear();
		m_parentObjPtr = nullptr;
	}

	#pragma region Static

	std::string GameObject::GetHierarchyString() {
		return m_gameObjectManager->GetHierarchy();
	}

	GameObject* GameObject::Create(const std::string& name) {
		#ifndef NDEBUG
		if (!m_gameObjectManager->IsNameUnique(name)) {
			Log::Warn("GameObject: Name '{}' is not unique", name);
		}
		#endif
		auto go = std::unique_ptr<GameObject>(new GameObject(m_gameObjectManager->GetNewUniqueIdentifier(), name));
		GameObject* goPtr = go.get();
		m_gameObjectManager->AddGameObject(std::move(go));
		return goPtr;
	}

	bool GameObject::Delete(GameObject* gameObjectPtr) {
		return m_gameObjectManager->DeleteGameObject(gameObjectPtr);
	}

	bool GameObject::Delete(unsigned int id) {
		return m_gameObjectManager->DeleteGameObject(id);
	}

	bool GameObject::Delete(const std::string& name) {
		return m_gameObjectManager->DeleteGameObject(name);
	}

	#pragma endregion

	bool GameObject::HasParent() const {
		return (m_parentObjPtr != nullptr);
	}

	#pragma region Get

	Component::Transform* GameObject::GetTransform() {
		return &m_transform;
	}

	std::string GameObject::GetName() const {
		return m_name;
	}

	unsigned int GameObject::GetID() const {
		return m_id;
	}

	GameObject* GameObject::GetParent() const {
		return m_parentObjPtr;
	}

	const std::vector<GameObject*>& GameObject::GetChildren() const {
		return m_childObjPtrs;
	}

	std::string GameObject::GetComponentListString() const {
		return GetComponentListString(false);
	}

	std::string GameObject::GetComponentListString(bool moreDetail) const {
		std::string listString;
		listString.append(FormatUtils::formatString("{}:\n", m_name));

		m_transform.GetComponentString("  - ", listString, moreDetail);
		for (const auto& comp : m_components) {
			if (comp) {
				comp->GetComponentString("  - ", listString, moreDetail);
			}
		}

		return listString;
	}

	#pragma endregion

	GameObject* GameObject::SetName(const std::string& name) {
		m_name = name;
		return this;
	}

	GameObject* GameObject::SetParent(GameObject * parentPtr) {
		if (m_parentObjPtr == parentPtr)
			return nullptr;

		if (this == parentPtr) {
			Log::Warn("GameObject: Could not set parent to itself");
			return nullptr;
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
		return this;
	}

	GameObject* GameObject::Detach() {
		return SetParent(nullptr);
	}

	void GameObject::RemoveChild(GameObject* child) {
		auto& children = m_childObjPtrs;
		children.erase(std::remove(children.begin(), children.end(), child), children.end());
	}

	void GameObject::Draw() {
		for (auto& comp : m_components) {
			if (comp->IsDrawable()) {
				comp->Draw();
			}
		}
	}
}
