#ifndef NDEBUG

#pragma once

struct ImFont;
class Application;

namespace EngineCore {

	class Engine;
	class GameObject;
	class GameObjectManager;

	class DebuggerWindows {
	public:
		static void Init(Engine* engine);
		static void SetIconFonts(ImFont* smallIcon, ImFont* largeIcon);
		static void MenuSidebar(float sidebarRatio, int windowWidth, int windowHeight);
		static void StatsWindow(float startX, float startY);
		static void CameraWindow(float startX, float startY);
		static void IconDisplayWindow(float startX, float startY);

	private:
		static Engine* m_engine;
		static std::weak_ptr<Application> m_app;
		static GameObjectManager* m_gameObjectManager;

		static ImFont* m_smallIconFont;
		static ImFont* m_largeIconFont;

		static bool m_statsWin;
		static bool m_cameraWin;
		static bool m_iconWin;
	};

}

#endif