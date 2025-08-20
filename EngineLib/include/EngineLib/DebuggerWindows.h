#ifndef NDEBUG

#pragma once

struct ImFont;
namespace EngineCore {

	class Debugger;
	class DebuggerWindows {
	public:
		DebuggerWindows(Debugger* debugger);
		void SetIconFonts(ImFont* smallIcon, ImFont* largeIcon);
		void MenuSidebar(float sidebarRatio, int windowWidth, int windowHeight);
		void StatsWindow(float startX, float startY);
		void CameraWindow(float startX, float startY);
		void IconDisplayWindow(float startX, float startY);

	private:
		Debugger* m_debugger = nullptr;

		ImFont* m_smallIconFont = nullptr;
		ImFont* m_largeIconFont = nullptr;

		bool m_statsWin = false;
		bool m_cameraWin = false;
		bool m_iconWin = false;
	};

}

#endif