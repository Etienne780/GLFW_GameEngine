#pragma once
#include <string>

namespace EngineCore {

	class GameObject;

	class ComponentBase {
		friend class GameObject;
	public:
		virtual ~ComponentBase() = default;
		virtual void OnUpdate(float deltaTime) {}
		virtual bool IsDrawable() const { return false; }
		virtual void Draw() {}

		std::string GetComponentString() const;
		std::string GetComponentString(bool moreDetail) const;
		virtual void GetComponentString(const std::string& prefix, std::string& outStr, bool moreDetail) const;

		GameObject* GetGameObject() const;
		std::string GetName() const;

	protected:
		ComponentBase(const std::string& name, GameObject* gameObject);

		std::string m_name;
		GameObject* m_gameObject = nullptr;
	};

}