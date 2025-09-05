#pragma once
#include <string>
#include "IComponentUIRenderer.h"
#include "ComponentTypeID.h"
#include "EngineTypes.h"

#define COMPONENT_TYPE_DEFINITION(T) \
    static ComponentTypeID StaticTypeID() { return GetComponentTypeID<T>(); } \
    ComponentTypeID GetTypeID() const override { return StaticTypeID(); }

namespace EngineCore {

	class GameObject;

	class ComponentBase {
	friend class GameObject;
	friend class DebuggerWindows;
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
		/*
		* @brief Returns the current ID of the GameObject this Component belongs to.
		*        Note: For persistent GameObjects, the ID may change during runtime.
		* @return The current ID of the associated GameObject.
		*/
		GameObjectID GetGameObjectID() const;

		/*
		* @brief Returns a pointer to the internal ID of the GameObject this Component belongs to.
		*        Useful for persistent GameObjects, where the ID may be updated dynamically.
		*        Do NOT modify the ID through this pointer, as it may cause internal inconsistencies.
		* @return Pointer to the internal GameObject ID.
		*/
		const GameObjectID* GetGameObjectIDPtr() const;


		virtual ComponentTypeID GetTypeID() const { return ENGINE_INVALID_ID; }

		template<typename T>
		bool IsType() const {
			return GetTypeID() == GetComponentTypeID<T>();
		}

	protected:
		// needs to be a normal enum so that it can be easly used as an index(order). Dosent have to be a enum class 
		// because it is only visible in ComponentBase and subclasses
		// components get sorted when added to a GO
		enum ExecutionOrder {
			ScriptComponent = 0,
			BaseComponent,
			PhysicsComponent,
			CollisionComponent
		};

		ComponentBase(const std::string& name, GameObjectID goID);
		ComponentBase(const std::string& name, ExecutionOrder executionOrder, GameObjectID goID);

		bool m_alive = true;
		bool m_isDisabled = false;
		std::string m_name;
		GameObjectID m_gameObjectID = GameObjectID(ENGINE_INVALID_ID);
		mutable std::shared_ptr<GameObject> m_gameObject = nullptr;
		/**
		* @brief checks if the gameobject is Dead
		* @param msg if the func is true writes warn(GameObject: 'msg', GameObject was deleted!)
		* @return returns true when the gameobject is dead
		*/
		bool IsDead(const std::string& msg) const;
		void CUpdate();
		/*
		* @brief Called every frame before Application Update.
		*/
		virtual void UpdateImpl() {}
		/*
		* @brief Called every frame before Application Update and Component update,
		*        even if the Component is disabled.
		*/
		virtual void UpdateAlwaysImpl() {}
		void CSubmitDrawCall();
		virtual void SubmitDrawCall() {}
		virtual void OnInspectorGUIImpl(IUIRenderer& ui) { ui.DrawLabel("No Component UI defined"); }
	
	private:
		ExecutionOrder m_executionOrder = ExecutionOrder::BaseComponent;
	};

}