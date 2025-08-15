#include "EngineLib\GameObjectManager.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\ComponentBase.h"

namespace EngineCore {

	GameObjectManager* GameObject::m_gameObjectManager = nullptr;
	
	GameObject::GameObject(unsigned int id, const std::string& name)
		: m_id(id), m_name(name) {
		m_transform = std::make_shared<Component::Transform>(this);
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

	std::shared_ptr<GameObject> GameObject::Create(const std::string& name) {
		#ifndef NDEBUG
		if (!m_gameObjectManager->IsNameUnique(name)) {
			Log::Warn("GameObject: Name '{}' is not unique", name);
		}
		#endif
		unsigned int id = m_gameObjectManager->GetNewUniqueIdentifier();
		auto go = std::shared_ptr<GameObject>(new GameObject(id, name));
		m_gameObjectManager->AddGameObject(go);
		return go;
	}

	bool GameObject::Delete(std::shared_ptr<GameObject> gameObjectPtr) {
		return m_gameObjectManager->DeleteGameObject(gameObjectPtr);
	}

	bool GameObject::Delete(unsigned int id) {
		return m_gameObjectManager->DeleteGameObject(id);
	}

	bool GameObject::Delete(const std::string& name) {
		return m_gameObjectManager->DeleteGameObject(name);
	}

	std::weak_ptr<Component::Camera>  GameObject::GetMainCamera() {
		return m_gameObjectManager->GetMainCamera();
	}

	void GameObject::SetMainCamera(std::shared_ptr<Component::Camera> camera) {
		m_gameObjectManager->SetMainCamera(camera);
	}

	#pragma endregion

	std::shared_ptr<GameObject> GameObject::Get(unsigned int id) {
		return m_gameObjectManager->GetGameObject(id);
	}

	std::shared_ptr<GameObject> GameObject::Get(const std::string& name) {
		return m_gameObjectManager->GetGameObject(name);
	}

	bool GameObject::HasParent() const {
		return (m_parentObjPtr != nullptr);
	}

	#pragma region Get

	std::shared_ptr<Component::Transform> GameObject::GetTransform() {
		if (IsDead("Cant get Transform")) {
			return nullptr;
		}
		return m_transform;
	}

	std::string GameObject::GetName() const {
		if (IsDead("Cant get name")) {
			return "INVALID";
		}
		return m_name;
	}

	unsigned int GameObject::GetID() const {
		if (IsDead("Cant get id")) {
			return ENGINE_INVALID_ID;
		}
		return m_id;
	}

	std::shared_ptr<GameObject> GameObject::GetParent() const {
		if (IsDead("Cant get parent")) {
			return nullptr;
		}
		return m_parentObjPtr;
	}

	std::vector<std::shared_ptr<GameObject>> GameObject::GetChildren() const {
		if (IsDead("Cant get children")) {
			return {};
		}
		return m_childObjPtrs;
	}

	std::string GameObject::GetComponentListString() const {
		if (IsDead("Cant get Component list string")) {
			return "INVALID";
		}
		return GetComponentListString(false);
	}

	std::string GameObject::GetComponentListString(bool moreDetail) const {
		if (IsDead("Cant get Component list string")) {
			return "INVALID";
		}
		std::string listString;
		listString.append(FormatUtils::formatString("{}:\n", m_name));

		m_transform->GetComponentString("  - ", listString, moreDetail);
		for (const auto& comp : m_components) {
			if (comp) {
				comp->GetComponentString("  - ", listString, moreDetail);
			}
		}

		return listString;
	}

	#pragma endregion

	GameObject* GameObject::SetName(const std::string& name) {
		if (IsDead("Cant set name")) {
			return this;
		}
		m_name = name;
		return this;
	}

	GameObject* GameObject::SetParent(std::shared_ptr<GameObject> parentPtr) {
		if (IsDead("Cant set parent")) {
			return this;
		}
		if (m_parentObjPtr == parentPtr)
			return this;

		if (this == parentPtr.get()) {
			Log::Warn("GameObject: Could not set parent to itself");
			return nullptr;
		}
		
		// remove self form parents child list
		if (m_parentObjPtr) {
			m_parentObjPtr->RemoveChild(shared_from_this());
		}

		m_parentObjPtr = parentPtr;
		// add selft to parents child list
		if (m_parentObjPtr) {
			m_parentObjPtr->m_childObjPtrs.push_back(shared_from_this());
		}
		return this;
	}

	GameObject* GameObject::Detach() {
		if (IsDead("Cant detach from parent")) {
			return this;
		}
		SetParent(nullptr);
		return this;
	}

	void GameObject::RegisterCamera(std::weak_ptr<Component::Camera> camera) {
		m_gameObjectManager->AddCamera(camera);
	}

	void GameObject::UnregisterCamera(std::weak_ptr<Component::Camera> camera) {
		m_gameObjectManager->RemoveCamera(camera);
	}

	void GameObject::UnregisterCameraFromManager() {
		if (!m_hasCamera) return;
		auto c = GetComponent<Component::Camera>();
		m_gameObjectManager->RemoveCamera(c);
	}

	void GameObject::UnaliveComponents() {
		for (auto& comp : m_components) {
			comp->m_alive = false;
		}
	}

	void GameObject::RemoveChild(std::shared_ptr<GameObject> child) {
		auto& children = m_childObjPtrs;
		children.erase(std::remove_if(children.begin(), children.end(),
			[this](const std::shared_ptr<GameObject>& child) {
				return child.get() == this;
			}),
			children.end());
	}

	void GameObject::Draw() {
		for (auto& comp : m_components) {
			if (comp->IsDrawable()) {
				comp->Draw();
			}
		}
	}

	bool GameObject::IsDead(const std::string& msg) const {
		if (!m_alive) {
			Log::Warn("GameObject: {}, GameObject '{}({})' was deleted!", msg, m_name, m_id);
			return true;
		}
		return false;
	}
}
