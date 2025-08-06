#pragma once
#include <string>

namespace EngineCore {

	class GameObject;

	class Component {
	friend class GameObject;
	public:
		virtual ~Component() = default;
		virtual void OnUpdate(float deltaTime) {}

		GameObject* GetGameObject() const;
		std::string GetName() const;

	protected:
		Component(const std::string& name);

		std::string m_name;
		GameObject* m_owner = nullptr;
	};

}