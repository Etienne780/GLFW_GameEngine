#pragma once
#include "EngineTypes.h"

struct GLFWwindow;
class Application;

namespace EngineCore {

	class Debugger {
	friend class Engine;
	public:
		void Init(GLFWwindow* m_window, Application* app);
		void Update();
		void Shutdown();

	private:
		Debugger();

		GLFWwindow* m_window = nullptr;
		Application* m_app = nullptr;
	};

}