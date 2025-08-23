#include "EngineLib\GameObjectManager.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\ComponentBase.h"

namespace EngineCore {

	GameObjectManager* GameObject::m_gameObjectManager = nullptr;
	
	GameObject::GameObject(unsigned int id, const std::string& name)
		: m_id(id), m_name(name) {
	}

	void GameObject::InitComponents() {
		if (!m_transform) {
			m_transform = std::make_shared<Component::Transform>(m_id);
		}
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

	size_t GameObject::GetGameObjectCount() {
		return m_gameObjectManager->m_gameObjects.size();
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
		go->InitComponents();
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

	void GameObject::ClearAll() {
		m_gameObjectManager->CleareAllGameObjects();
	}

	void GameObject::DeleteAll() {
		m_gameObjectManager->DeleteAllGameObjects();
	}

	std::shared_ptr<Component::Camera>  GameObject::GetMainCamera() {
		return m_gameObjectManager->GetMainCamera().lock();
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

	void GameObject::Disable(bool value) {
		m_isDisabled = value;
	}

	bool GameObject::IsDisable() const {
		return m_isDisabled;
	}

	bool GameObject::IsPersistent() const {
		return m_isPersistent;
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

	const unsigned int* GameObject::GetIDPtr() const {
		if (IsDead("Cant get id ptr")) {
			return nullptr;
		}
		return &m_id;
	}

	std::shared_ptr<GameObject> GameObject::GetParent() const {
		if (IsDead("Cant get parent")) {
			return nullptr;
		}
		return m_parentObjPtr;
	}

	const std::vector<std::shared_ptr<GameObject>>& GameObject::GetChildren() const {
		if (IsDead("Cant get children")) {
			static const std::vector<std::shared_ptr<GameObject>> empty;
			return empty;
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
			// sets child persistent if parent is persistent
			m_isPersistent = m_parentObjPtr->IsPersistent();
		}
		return this;
	}

	GameObject* GameObject::SetPersistent(bool value) {
		if (IsDead("Cant set persistent")) {
			return this;
		}

		m_isPersistent = value;
		// sets the childs also to persistent
		if (!m_childObjPtrs.empty()) {
			for (auto& childs : m_childObjPtrs) {
				childs->m_isPersistent = value;
			}
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

	void GameObject::Update() {
		if (m_isDisabled)
			return;

		for (auto& comp : m_components) {
			comp->CUpdate();
		}
	}

	void GameObject::UpdateComponentIDs() {
		m_transform->m_gameObjectID = m_id;
		for (auto& comp : m_components) {
			comp->m_gameObjectID = m_id;
		}

		for (auto& comp : m_drawComponents) {
			comp->m_gameObjectID = m_id;
		}
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

	void GameObject::SubmitDrawCall() {
		if (m_isDisabled)
			return;

		for (auto& comp : m_drawComponents) {
			comp->SubmitDrawCall();
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
