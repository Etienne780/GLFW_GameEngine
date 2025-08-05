#pragma once
#include "Component.h"

namespace EngineCore {

	template<typename C, typename... Args>
	C* GameObject::AddComponent(Args&&... args) {
		static_assert(std::is_base_of<Component, C>::value, "C must derive from Component");

		auto comp = std::make_unique<C>(std::forward<Args>(args)...);
		C* ptr = comp.get();
		m_components.emplace_back(std::move(comp));
		return ptr;
	}

	template<typename C>
	C* GameObject::GetComponent() const {
		for (const auto& comp : m_components) {
			if (auto casted = dynamic_cast<C*>(comp.get()))
				return casted;
		}
		return nullptr;
	}

}
