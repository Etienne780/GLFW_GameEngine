#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include "Components/Transform_C.h"

namespace EngineCore {

	class Component;

	class GameObject {
	friend class GameObjectManager;
	public:
		~GameObject();

		static GameObject* Create(const std::string& name = "");
		static bool Delete(GameObject* gameObjectPtr);
		static bool Delete(unsigned int id);
		static bool Delete(const std::string& name);

		template<typename C, typename... Args>
		C* AddComponent(Args&&... args);

		template<typename C>
		C* GetComponent() const;

		template<typename C>
		bool TryGetComponent(C*& outComponent) const;

		template<typename C>
		bool HasComponent() const;
		bool HasParent() const;

		std::string GetName() const;
		unsigned int GetID() const;
		GameObject* GetParent() const;

		void SetParent(GameObject* parentPtr);
		void Detach();

	private:
		GameObject(const std::string& name);

		unsigned int m_id = -1;
		std::string m_name;
		GameObject* m_parentObjPtr = nullptr;
		std::vector<GameObject*> m_childObjPtrs;

		Transform m_transform;
		std::vector<std::unique_ptr<Component>> m_components;

		void RemoveChild(GameObject* child);
	};

}

#include "GameObject.inl"