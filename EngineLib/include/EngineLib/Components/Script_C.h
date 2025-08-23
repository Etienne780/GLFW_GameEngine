#pragma once
#include <string>
#include "..\ResourceManager.h"
#include "..\Application.h"
#include "..\ComponentBase.h"

namespace EngineCore {

	class Script : public ComponentBase {
	protected:
		Script(unsigned int gameObjectID);

		void UpdateImpl() override;
		void UpdateAlwaysImpl() override;

		virtual void Update() {};
		virtual void UpdateAlways() {};

		static ResourceManager& resourceManager;
		static Application* app;
	};

}