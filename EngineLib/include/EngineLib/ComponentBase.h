#pragma once
#include <string>
#include "ComponentTypeID.h"

#define COMPONENT_TYPE_DEFINITION(T) \
    static ComponentTypeID StaticTypeID() { return GetComponentTypeID<T>(); } \
    ComponentTypeID GetTypeID() const override { return StaticTypeID(); }

namespace EngineCore {

	class GameObject;

	class ComponentBase {
		friend class GameObject;
	public:
		virtual ~ComponentBase() = default;

		void CUpdate(float deltaTime);
		virtual void UpdateImpl(float deltaTime) {}
		virtual bool IsDrawable() const { return false; }
		virtual void SubmitDrawCall() {}

		std::string GetComponentString() const;
		std::string GetComponentString(bool moreDetail) const;
		virtual void GetComponentString(const std::string& prefix, std::string& outStr, bool moreDetail) const;

		std::string GetName() const;
		GameObject* GetGameObject() const;

		virtual ComponentTypeID GetTypeID() const { return 0; }

		template<typename T>
		bool IsType() const {
			return GetTypeID() == GetComponentTypeID<T>();
		}

	protected:
		ComponentBase(const std::string& name, GameObject* gameObject);

		bool m_alive = true;
		std::string m_name;
		GameObject* m_gameObject = nullptr;
		/**
		* @brief checks if the gameobject is Dead
		* @param msg if the func is true writes warn(GameObject: 'msg', GameObject was deleted!)
		* @return returns true when the gameobject is dead
		*/
		bool IsDead(const std::string& msg) const;
	};

}