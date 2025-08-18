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

		bool GetCursorLock();

	private:
		Debugger();

		GLFWwindow* m_window = nullptr;
		Application* m_app = nullptr;

		float m_windowWidth = 0;
		float m_windowHeight = 0;
		bool m_cursorLock = true;

		float m_menuSidebarWidth = 30;

		void SetVariables();
		void HandleCursorLock();
	};

}