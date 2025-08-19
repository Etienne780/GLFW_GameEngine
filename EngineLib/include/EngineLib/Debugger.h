#ifndef NDEBUG

#pragma once
#include "EngineTypes.h"

struct GLFWwindow;
class Application;

namespace EngineCore {

	class Debugger {
	friend class Engine;
	public:
		void Init(GLFWwindow* window, std::weak_ptr<Application> app, Engine* engine);
		void Update();
		void Shutdown();

		bool GetCursorLock();

	private:
		Debugger();

		GLFWwindow* m_window = nullptr;
		std::weak_ptr<Application> m_app;
		Engine* m_engine = nullptr;
		std::shared_ptr<GameObject> m_debugCamera;

		int m_windowWidth = 0;
		int m_windowHeight = 0;
		bool m_cursorLock = true;

		float m_menuSidebarWidthRatio = 0.07f;// is in %

		void SetVariables();
		void HandleCursorLock();
	};

}

#endif