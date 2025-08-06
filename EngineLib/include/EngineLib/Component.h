#pragma once
#include <string>

namespace EngineCore {

	class GameObject;

	class Component {
	friend class GameObject;
	public:
		virtual ~Component() = default;
		virtual void OnUpdate(float deltaTime) {}
		std::string GetComponentString() const;
		std::string GetComponentString(bool moreDetail) const;
		virtual void GetComponentString(const std::string& prefix, std::string& outStr, bool moreDetail) const;

		GameObject* GetGameObject() const;
		std::string GetName() const;

	protected:
		Component(const std::string& name);

		std::string m_name;
		GameObject* m_owner = nullptr;
	};

}