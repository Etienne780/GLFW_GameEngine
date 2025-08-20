#ifndef NDEBUG

#pragma once
#include <memory.h>
#include "EngineTypes.h"

struct GLFWwindow;
class Application;

namespace EngineCore {

	class DebuggerWindows;
	class Debugger {
	friend class Engine;
	public:
		void Init();
		void Update();
		void Shutdown();

		bool GetCursorLock() const;
		bool IsDebugCameraActive() const;

		Application* GetApp();
		GLFWwindow* GetWindow() const;
		GameObjectManager* GetGameObjectManager() const;

	private:
		Debugger(Engine* engine);

		Engine* m_engine = nullptr;
		std::weak_ptr<Application> m_app;
		GLFWwindow* m_window = nullptr;
		GameObjectManager* m_gameObjectManager = nullptr;
		std::shared_ptr<GameObject> m_debugCamera;
		std::unique_ptr<DebuggerWindows> m_debuggerWindows;

		int m_windowWidth = 0;
		int m_windowHeight = 0;
		bool m_cursorLock = true;
		bool m_isDebugCameraActive = false;

		float m_menuSidebarWidthRatio = 0.07f;// is in %

		void DebugCameraInit();
		void SetVariables();
		void HandleCursorLock();
	};

}

#endif