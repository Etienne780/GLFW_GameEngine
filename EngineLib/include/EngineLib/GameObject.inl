#pragma once
#include "CoreLib\Log.h"
#include "ComponentBase.h"

namespace EngineCore {

	template<typename C, typename... Args>
	std::shared_ptr<C> GameObject::AddComponent(Args&&... args) {
		static_assert(std::is_base_of<ComponentBase, C>::value, "C must derive from ComponentBase");
		if (IsDead("Cant add component")) return nullptr;

		if constexpr (std::is_same_v<C, Component::Transform>) {
			Log::Warn("Transform is already part of the GameObject.");
			return nullptr;
		}
		
		if (HasComponent<C>()) {
			Log::Warn("Component already exists.");
			return nullptr;
		}

		auto comp = std::make_shared<C>(m_id, std::forward<Args>(args)...);
		if (auto cam = std::dynamic_pointer_cast<Component::Camera>(comp)) {
			m_hasCamera = true;
			RegisterCamera(cam);
		}

		m_components.emplace_back(comp);
		SortComponents();
		return comp;
	}

	template<typename C>
	GameObject* GameObject::RemoveComponent() {
		static_assert(std::is_base_of<ComponentBase, C>::value, "C must derive from ComponentBase");
		if (IsDead("Cant remove component")) {
			return this;
		}

		// Cant remove transform component
		if constexpr (std::is_same<C, Component::Transform>::value) {
			#ifndef NDEBUG
			Log::Warn("GameObject: Cannot remove Transform via RemoveComponent!");
			#endif
			return this;
		}

		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			if (it->get()->IsType<C>()) {
				// if component is a camera give it to the GameObjectManager
				if (auto cameraPtr = std::dynamic_pointer_cast<Component::Camera>(*it)) {
					m_hasCamera = false;
					UnregisterCamera(std::weak_ptr<Component::Camera>(cameraPtr));
				}
				it->get()->m_alive = false;
				m_components.erase(it);
				break;
			}
		}
		return this;
	}

	template<typename C>
	std::shared_ptr<C> GameObject::GetComponent() const {
		static_assert(std::is_base_of<ComponentBase, C>::value, "C must derive from ComponentBase");
		if (IsDead("Cant get component")) {
			return nullptr;
		}

		if constexpr (std::is_same<C, Component::Transform>::value) {
			return m_transform;
		}

		for (const auto& comp : m_components) {
			if (auto casted = dynamic_cast<C*>(comp.get())) {
				return std::static_pointer_cast<C>(comp);
			}
		}

		return nullptr;
	}

	template<typename C>
	bool GameObject::TryGetComponent(C*& outComponent) const {
		if (IsDead("Cant try get component")) {
			return false;
		}
		outComponent = (*this).GetComponent<C>();
		return outComponent != nullptr;
	}

	template<typename C>
	bool GameObject::HasComponent() const {
		if (IsDead("Cant check if GO has component")) {
			return false;
		}
		return (*this).GetComponent<C>() != nullptr;
	}

}
