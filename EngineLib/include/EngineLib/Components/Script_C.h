#pragma once
#include <string>
#include "..\ComponentBase.h"

namespace EngineCore {
	class ResourceManager;

	class Script : public ComponentBase {
	protected:
		Script(GameObject* gameObject);

		void UpdateImpl(float deltaTime) override;

		virtual void Update() {};

		static ResourceManager& resourceManager;
	};

}