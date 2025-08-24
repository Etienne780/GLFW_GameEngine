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
	friend class GameObjectManager;
	public:
		~GameObject();

		static std::string GetHierarchyString();
		static size_t GetGameObjectCount();
		static std::shared_ptr<GameObject> Create(const std::string& name);
		static bool Delete(std::shared_ptr<GameObject> gameObjectPtr);
		static bool Delete(unsigned int id);
		static bool Delete(const std::string& name);
		/*
		* @brief Deletes all GameObjects except persistent ones.
		*/
		static void ClearAll();
		/*
		* @brief Deletes all GameObjects including persistent ones.
		*/
		static void DeleteAll();
		static std::shared_ptr<GameObject> Get(unsigned int id);
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

		std::shared_ptr<Component::Transform> GetTransform();
		std::string GetName() const;
		/*
		* @brief Returns the current ID of this GameObject.
		*        Note: For persistent GameObjects, the ID may change during runtime.
		* @return The current ID of the GameObject.
		*/
		unsigned int GetID() const;
		/*
		* @brief Returns a pointer to the internal ID of this GameObject.
		*        Useful for persistent GameObjects, where the ID may be updated dynamically.
		*        Do NOT modify the ID through this pointer, as it may cause internal inconsistencies.
		* @return Pointer to the internal ID.
		*/
		const unsigned int* GetIDPtr() const;
		std::shared_ptr<GameObject> GetParent() const;
		const std::vector<std::shared_ptr<GameObject>>& GetChildren() const;
		std::string GetComponentListString() const;
		std::string GetComponentListString(bool moreDetail) const;

		GameObject* SetName(const std::string& name);
		GameObject* SetParent(std::shared_ptr<GameObject> parentPtr);
		GameObject* SetPersistent(bool value);
		GameObject* Detach();

	private:
		GameObject(unsigned int id, const std::string& name);
		void InitComponents();

		static GameObjectManager* m_gameObjectManager;

		unsigned int m_id = ENGINE_INVALID_ID;
		bool m_alive = true;
		bool m_isPersistent = false;
		bool m_isDisabled = false;
		std::string m_name;
		std::shared_ptr<GameObject> m_parentObjPtr = nullptr;
		std::vector<std::shared_ptr<GameObject>> m_childObjPtrs;
		std::shared_ptr<Component::Transform> m_transform = nullptr;
		bool m_hasCamera = false;

		std::vector<std::shared_ptr<ComponentBase>> m_components;
		std::vector<std::shared_ptr<ComponentBase>> m_drawComponents;

		/**
		* @brief calls the start and update methods of the components
		*/
		void Update();

		void UpdateComponentIDs();
		void RegisterCamera(std::weak_ptr<Component::Camera> camera);
		void UnregisterCamera(std::weak_ptr<Component::Camera> camera);
		void UnregisterCameraFromManager();
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