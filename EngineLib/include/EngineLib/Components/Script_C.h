#pragma once
#include <string>
#include "..\ComponentBase.h"

namespace EngineCore {
	class ResourceManager;

	class Script : public ComponentBase {
	protected:
		Script(unsigned int gameObjectID);

		void UpdateImpl() override;

		virtual void Update() {};

		static ResourceManager& resourceManager;
	};

}