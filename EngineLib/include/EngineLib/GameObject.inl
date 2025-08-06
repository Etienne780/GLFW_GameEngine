#pragma once
#include "Component.h"
#include "CoreLib\Log.h"

namespace EngineCore {

	template<typename C, typename... Args>
	C* GameObject::AddComponent(Args&&... args) {
		static_assert(std::is_base_of<Component, C>::value, "C must derive from Component");

		// Cant add transform component. is already a member of GO
		if constexpr (std::is_same<C, Transform>::value) {
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
		auto comp = std::make_unique<C>(std::forward<Args>(args)...);
		comp->m_owner = this;
		C* ptr = comp.get();
		m_components.emplace_back(std::move(comp));
		return ptr;
	}

	template<typename C>
	C* GameObject::GetComponent() const {
		static_assert(std::is_base_of<Component, C>::value, "C must derive from Component");

		if constexpr (std::is_same<C, Transform>::value) {
			return const_cast<Transform*>(&m_transform);
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
