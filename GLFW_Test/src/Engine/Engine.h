#pragma once
#include <GLFW\glfw3.h>

#include "Application.h"
#include "Time.h"
#include "Input.h"

#include "..\Core\FormatUtils.h"
#include "..\Core\Log.h"
#include "..\Core\Math.h"

namespace EngineCore {
	class Engine {
	public:
		std::unique_ptr<Application> app;

		Engine(std::unique_ptr<Application> app);

		void Start(GLFWwindow* window);
		void Update(double currentTimeSec);
		void Shutdown();

	private:
		GLFWwindow* m_window;
		Time m_time;
		Input m_input;
	};
}