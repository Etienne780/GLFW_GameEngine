#pragma once
#include <string>
#include "ComponentTypeID.h"
#include "EngineTypes.h"

#define COMPONENT_TYPE_DEFINITION(T) \
    static ComponentTypeID StaticTypeID() { return GetComponentTypeID<T>(); } \
    ComponentTypeID GetTypeID() const override { return StaticTypeID(); }

namespace EngineCore {

	class GameObject;

	class ComponentBase {
		friend class GameObject;
	public:
		virtual ~ComponentBase() = default;

		virtual bool CanDisalbe() const { return true; }
		virtual bool IsDrawable() const { return false; }

		void Disable(bool value);
		bool IsDisable() const;

		std::string GetComponentString() const;
		std::string GetComponentString(bool moreDetail) const;
		virtual void GetComponentString(const std::string& prefix, std::string& outStr, bool moreDetail) const;

		std::string GetName() const;
		std::shared_ptr<GameObject> GetGameObject() const;

		virtual ComponentTypeID GetTypeID() const { return ENGINE_INVALID_ID; }

		template<typename T>
		bool IsType() const {
			return GetTypeID() == GetComponentTypeID<T>();
		}

	protected:
		ComponentBase(const std::string& name, unsigned int goID);

		bool m_alive = true;
		bool m_isDisabled = false;
		std::string m_name;
		unsigned int m_gameObjectID = ENGINE_INVALID_ID;
		mutable std::shared_ptr<GameObject> m_gameObject = nullptr;
		/**
		* @brief checks if the gameobject is Dead
		* @param msg if the func is true writes warn(GameObject: 'msg', GameObject was deleted!)
		* @return returns true when the gameobject is dead
		*/
		bool IsDead(const std::string& msg) const;
		void CUpdate();
		virtual void UpdateImpl() {}
		virtual void SubmitDrawCall() {}
	};

}