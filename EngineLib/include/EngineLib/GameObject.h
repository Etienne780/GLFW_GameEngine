#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include "Components/Transform_C.h"
#include "EngineTypes.h"

namespace EngineCore {

	class ComponentBase;

	class GameObject {
	friend class GameObjectManager;
	public:
		~GameObject();

		static std::string GetHierarchyString();
		static GameObject* Create(const std::string& name);
		static bool Delete(GameObject* gameObjectPtr);
		static bool Delete(unsigned int id);
		static bool Delete(const std::string& name);

		template<typename C, typename... Args>
		C* AddComponent(Args&&... args);

		template<typename C>
		GameObject* RemoveComponent();

		template<typename C>
		C* GetComponent() const;

		template<typename C>
		bool TryGetComponent(C*& outComponent) const;

		template<typename C>
		bool HasComponent() const;
		bool HasParent() const;

		Component::Transform * GetTransform();
		std::string GetName() const;
		unsigned int GetID() const;
		GameObject* GetParent() const;
		const std::vector<GameObject*>& GetChildren() const;
		std::string GetComponentListString() const;
		std::string GetComponentListString(bool moreDetail) const;

		GameObject* SetName(const std::string& name);
		GameObject* SetParent(GameObject* parentPtr);
		GameObject* Detach();

	private:
		GameObject(unsigned int id, const std::string& name);

		static GameObjectManager* m_gameObjectManager;

		unsigned int m_id = ENGINE_INVALID_ID;
		std::string m_name;
		GameObject* m_parentObjPtr = nullptr;
		std::vector<GameObject*> m_childObjPtrs;
		Component::Transform m_transform;

		std::vector<std::unique_ptr<ComponentBase>> m_components;

		void RemoveChild(GameObject* child);
		void Draw();
	};

}

#include "GameObject.inl"