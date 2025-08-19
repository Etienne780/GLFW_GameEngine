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

		int m_windowWidth = 0;
		int m_windowHeight = 0;
		bool m_cursorLock = true;

		float m_menuSidebarWidthRatio = 0.07f;// is in %

		void SetVariables();
		void HandleCursorLock();
	};

}