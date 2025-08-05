#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

namespace EngineCore {

	class Component;

	class GameObject {
	public:
		GameObject() = default;
		GameObject(const std::string& name);

		template<typename C, typename... Args>
		C* AddComponent(Args&&... args);

		template<typename C>
		C* GetComponent() const;

	private:
		std::string m_name;
		std::vector<std::unique_ptr<Component>> m_components;
	};

}

#include "GameObject.inl"
