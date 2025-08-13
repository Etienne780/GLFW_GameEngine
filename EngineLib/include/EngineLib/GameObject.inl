#pragma once
#include "ComponentBase.h"
#include "CoreLib\Log.h"

namespace EngineCore {

	template<typename C, typename... Args>
	C* GameObject::AddComponent(Args&&... args) {
		static_assert(std::is_base_of<ComponentBase, C>::value, "C must derive from ComponentBase");

		// Cant add transform component. is already a member of GO
		if constexpr (std::is_same<C, Component::Transform>::value) {
			#ifndef NDEBUG
			Log::Warn("GameObject: Cannot add Transform via AddComponent. It is already part of the GameObject.");
			#endif
			return nullptr;
		}

		// Check if the GO has the componentn already
		for (const auto& comp : m_components) {
			if (dynamic_cast<C*>(comp.get())) {
				#ifndef NDEBUG
				Log::Warn("GameObject: Component '{}' already exists on GameObject.", comp.get()->GetName());
				#endif
				return nullptr;
			}
		}

		// Creates/Adds the component
		auto comp = std::make_unique<C>(this, std::forward<Args>(args)...);
		C* ptr = comp.get();
		m_components.emplace_back(std::move(comp));
		return ptr;
	}

	template<typename C>
	GameObject* GameObject::RemoveComponent() {
		static_assert(std::is_base_of<ComponentBase, C>::value, "C must derive from ComponentBase");

		// Cant remove transform component
		if constexpr (std::is_same<C, Component::Transform>::value) {
			#ifndef NDEBUG
			Log::Warn("GameObject: Cannot remove Transform via RemoveComponent!");
			#endif
			return this;
		}

		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			if (dynamic_cast<C*>(it->get())) {
				m_components.erase(it); // unique_ptr deletes the rest
				break;
			}
		}
		return this;
	}

	template<typename C>
	C* GameObject::GetComponent() const {
		static_assert(std::is_base_of<ComponentBase, C>::value, "C must derive from ComponentBase");

		if constexpr (std::is_same<C, Component::Transform>::value) {
			return const_cast<Component::Transform*>(&m_transform);
		}

		for (const auto& comp : m_components) {
			if (auto casted = dynamic_cast<C*>(comp.get()))
				return casted;
		}
		return nullptr;
	}

	template<typename C>
	bool GameObject::TryGetComponent(C*& outComponent) const {
		outComponent = (*this).GetComponent<C>();
		return outComponent != nullptr;
	}

	template<typename C>
	bool GameObject::HasComponent() const {
		return (*this).GetComponent<C>() != nullptr;
	}

}
