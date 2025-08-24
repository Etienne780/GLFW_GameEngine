#ifndef NDEBUG

#pragma once
#include "ComponentRendererImGui.h"

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
		void DrawGameObjectNode(std::shared_ptr<GameObject>& obj);
		void HierarchyWindow(float startX, float startY);
		void InspectorWindow(float startX, float startY);
		void IconDisplayWindow(float startX, float startY);
		
	private:
		ImGuiRenderer m_uiRenderer;
		Debugger* m_debugger = nullptr;

		ImFont* m_smallIconFont = nullptr;
		ImFont* m_largeIconFont = nullptr;

		bool m_statsWin = false;
		bool m_cameraWin = false;
		bool m_hierarchyWin = false;// and inspector
		bool m_iconWin = false;
	};

}

#endif