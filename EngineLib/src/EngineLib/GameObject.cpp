#include "EngineLib/RenderLayer.h"
#include "EngineLib/GameObjectManager.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/ComponentBase.h"

namespace EngineCore {

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
		if (id == ENGINE_INVALID_ID) {
			Log::Warn("GameObject: cant create GameObject, no free ids left");
			return nullptr;
		}
		auto go = std::shared_ptr<GameObject>(new GameObject(id, name));
		m_gameObjectManager->AddGameObject(go);// add gameobject to list
		go->InitComponents();// inits the transform comp (gets the go from the list as a ref)
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

	std::shared_ptr<GameObject> GameObject::Get(GameObjectID id) {
#ifndef NDEBUG
		auto go = m_gameObjectManager->GetGameObject(id.value);
		if (!go)
			Log::Warn("GameObject: no GameObject with ID '{}' found!", id.value);
		return go;
#endif
		return m_gameObjectManager->GetGameObject(id.value);
	}

	std::shared_ptr<GameObject> GameObject::Get(const std::string& name) {
#ifndef NDEBUG
		auto go = m_gameObjectManager->GetGameObject(name);
		if (!go)
			Log::Warn("GameObject: no GameObject with name '{}' found!", name);
		return go;
#endif
		return m_gameObjectManager->GetGameObject(name);
	}

	std::vector<std::shared_ptr<ComponentBase>> GameObject::GetAllComponents() const {
		std::vector<std::shared_ptr<ComponentBase>> comps;
		comps.reserve(m_components.size() + 1);

		comps.push_back(m_transform);
		for (auto& comp : m_components) {
			comps.push_back(comp);
		}

		return comps;
	}

	bool GameObject::HasParent() const {
		return (m_parentObjPtr != nullptr);
	}

	GameObject* GameObject::Disable(bool value) {
		if (IsDead("Cant Disable")) {
			return this;
		}

		m_isDisabled = value;

		// sets the childs also to disabled
		if (!m_childObjPtrs.empty()) {
			for (auto& childs : m_childObjPtrs) {
				childs->Disable(value);
			}
		}

		return this;
	}

	bool GameObject::IsDisabled() const {
		return m_isDisabled;
	}

	bool GameObject::IsPersistent() const {
		return m_isPersistent;
	}

	bool GameObject::IsAlive() const {
		return m_alive;
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

	GameObjectID GameObject::GetID() const {
		if (IsDead("Cant get id")) {
			return GameObjectID(ENGINE_INVALID_ID);
		}
		return m_id;
	}

	const GameObjectID* GameObject::GetIDPtr() const {
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

	unsigned int GameObject::GetRenderLayer() {
		if (IsDead("Cant get render-layer")) {
			return -1;
		}
		return m_renderLayerIndex;
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

	GameObject* GameObject::SetRenderLayer(unsigned int renderLayerIndex) {
		if (IsDead("Cant set render-layer")) {
			return this;
		}

		if (renderLayerIndex >= RenderLayer::GetLayerCount()) {
			Log::Error("GameObject: Cant set renderLayerIndex");
			return this;
		}

		m_renderLayerIndex = renderLayerIndex;
		return this;
	}

	GameObject* GameObject::SetRenderLayerInChildren(unsigned int renderLayerIndex) {
		if (IsDead("Cant set render-layer in children")) {
			return this;
		}

		// sets the renderLayer of childs 
		if (!m_childObjPtrs.empty()) {
			for (auto& childs : m_childObjPtrs) {
				childs->SetRenderLayer(renderLayerIndex);
			}
		}
		return this;
	}

	GameObject* GameObject::SetRenderLayerAll(unsigned int renderLayerIndex) {
		if (IsDead("Cant set render-layer all")) {
			return this;
		}

		SetRenderLayer(renderLayerIndex);
		SetRenderLayerInChildren(renderLayerIndex);
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
				childs->SetPersistent(value);
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

	void GameObject::SortComponents() {
		std::sort(m_components.begin(), m_components.end(), 
		[](const std::shared_ptr<ComponentBase>& a, const std::shared_ptr<ComponentBase>& b) {
			return a->m_executionOrder < b->m_executionOrder;
		});
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

		for (auto& comp : m_components) {
			if (comp->IsDrawable()) {
				comp->CSubmitDrawCall();
			}
		}
	}

	bool GameObject::IsDead(const std::string& msg) const {
		if (!m_alive) {
			Log::Warn("GameObject: {}, GameObject '{}({})' was deleted!", msg, m_name, m_id.value);
			return true;
		}
		return false;
	}
}
