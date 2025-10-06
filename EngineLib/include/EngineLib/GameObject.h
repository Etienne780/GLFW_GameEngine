#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include "AllComponents.h"
#include "EngineTypes.h"

namespace EngineCore {

	class ComponentBase;
	class GameObjectManager;

	class GameObject : public std::enable_shared_from_this<GameObject> {
	friend class ComponentBase;
	friend class GameObjectManager;
	public:
		~GameObject();

		static std::string GetHierarchyString();
		static size_t GetGameObjectCount();
		static std::shared_ptr<GameObject> Create(const std::string& name);
		static bool Delete(std::shared_ptr<GameObject> gameObjectPtr);
		static bool Delete(GameObjectID id);
		static bool Delete(const std::string& name);
		/*
		* @brief Deletes all GameObjects except persistent ones.
		*/
		static void ClearAll();
		/*
		* @brief Deletes all GameObjects including persistent ones.
		*/
		static void DeleteAll();
		static std::shared_ptr<GameObject> Get(GameObjectID id);
		static std::shared_ptr<GameObject> Get(const std::string& name);
		static std::shared_ptr<Component::Camera> GetMainCamera();
		static void SetMainCamera(std::shared_ptr<Component::Camera> camera);

		template<typename C, typename... Args>
		std::shared_ptr<C> AddComponent(Args&&... args);

		template<typename C>
		GameObject* RemoveComponent();

		template<typename C>
		std::shared_ptr<C> GetComponent() const;

		std::vector<std::shared_ptr<ComponentBase>> GetAllComponents() const;

		template<typename C>
		bool TryGetComponent(C*& outComponent) const;

		template<typename C>
		bool HasComponent() const;
		bool HasParent() const;

		GameObject* Disable(bool value);
		bool IsDisabled() const;
		bool IsPersistent() const;
		bool IsAlive() const;

		std::shared_ptr<Component::Transform> GetTransform();
		std::string GetName() const;
		/*
		* @brief Returns the current ID of this GameObject.
		*        Note: For persistent GameObjects, the ID may change during runtime.
		* @return The current ID of the GameObject.
		*/
		GameObjectID GetID() const;
		/*
		* @brief Returns a pointer to the internal ID of this GameObject.
		*        Useful for persistent GameObjects, where the ID may be updated dynamically.
		*        Do NOT modify the ID through this pointer, as it may cause internal inconsistencies.
		* @return Pointer to the internal ID.
		*/
		const GameObjectID* GetIDPtr() const;
		std::shared_ptr<GameObject> GetParent() const;
		RenderLayerID GetRenderLayer();
		const std::vector<std::shared_ptr<GameObject>>& GetChildren() const;
		std::string GetComponentListString() const;
		std::string GetComponentListString(bool moreDetail) const;

		GameObject* SetName(const std::string& name);

		// Is Slower than with ID
		GameObject* SetRenderLayer(const std::string& renderLayerName);
		GameObject* SetRenderLayerInChildren(const std::string& renderLayerName);
		GameObject* SetRenderLayerAll(const std::string& renderLayerName);

		// Is Faster than with Layer name
		GameObject* SetRenderLayer(RenderLayerID renderLayerID);
		GameObject* SetRenderLayerInChildren(RenderLayerID renderLayerID);
		GameObject* SetRenderLayerAll(RenderLayerID renderLayerID);

		GameObject* SetParent(std::shared_ptr<GameObject> parentPtr);
		GameObject* SetPersistent(bool value);
		GameObject* Detach();					

	private:
		GameObject(unsigned int id, const std::string& name);
		void InitComponents();

		static inline GameObjectManager* m_gameObjectManager = nullptr;

		GameObjectID m_id = GameObjectID(ENGINE_INVALID_ID);
		RenderLayerID m_renderLayerID = RenderLayerID(0);
		bool m_alive = true;
		bool m_isPersistent = false;
		bool m_isDisabled = false;
		std::string m_name;
		std::shared_ptr<GameObject> m_parentObjPtr = nullptr;
		std::vector<std::shared_ptr<GameObject>> m_childObjPtrs;
		std::shared_ptr<Component::Transform> m_transform = nullptr;
		bool m_hasCamera = false;

		std::vector<std::shared_ptr<ComponentBase>> m_components;

		/**
		* @brief calls the start and update methods of the components
		*/
		void Update();

		void UpdateComponentIDs();
		void RegisterCamera(std::weak_ptr<Component::Camera> camera);
		void UnregisterCamera(std::weak_ptr<Component::Camera> camera);
		void UnregisterCameraFromManager();
		/*
		* @brief sorts all the components of this gameobject after the ExecutionOrder enum
		*/
		void SortComponents();
		/**
		* @brief marks this GameObject as dead, so that all references know 'this' is deleted an will not be shown
		*/
		void Unalive();
		/**
		* @brief sets all the components alive variabel to false
		*/
		void UnaliveComponents();
		void RemoveChild(std::shared_ptr<GameObject> child);
		void SubmitDrawCall();
		/**
		* @brief checks if the gameobject is Dead
		* @param msg if the func is true writes warn(GameObject: 'msg', GameObject was deleted!)
		* @return returns true when the gameobject is dead
		*/
		bool IsDead(const std::string& msg) const;
	};

}

#include "GameObject.inl"