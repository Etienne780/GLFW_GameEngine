#include <memory>
#ifndef NDEBUG

#pragma once
#include "EngineTypes.h"
#include "UI/Elements/Element.h"

struct GLFWwindow;
class Application;

namespace EngineCore {

	namespace Component {
		class Camera;
	}

	class GameObject;
	class DebuggerWindows;
	class GameObjectManager;
	class Debugger {
	friend class Engine;
	public:
		std::shared_ptr<GameObject> m_debugCameraGO = nullptr;
		std::shared_ptr<GameObject> m_hierarchySelectedGO = nullptr;
		std::shared_ptr<UI::ElementBase> m_uiSelectedElement = nullptr;
		std::shared_ptr<UI::ElementBase> m_hoveredUIElement = nullptr;

		~Debugger();

		void Init();
		void Start();
		void Update();
		void Close();
		void Shutdown();

		bool GetCursorLock() const;
		bool IsDebugCameraActive() const;

		Application* GetApp();
		GLFWwindow* GetWindow() const;
		GameObjectManager* GetGameObjectManager() const;

		void SetMainCamera(std::shared_ptr<Component::Camera> cam);

	private:
		Debugger(Engine* engine);

		Engine* m_engine = nullptr;
		std::weak_ptr<Application> m_app;
		GLFWwindow* m_window = nullptr;
		GameObjectManager* m_gameObjectManager = nullptr;
		std::unique_ptr<DebuggerWindows> m_debuggerWindows = nullptr;

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