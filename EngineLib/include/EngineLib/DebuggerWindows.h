#ifndef NDEBUG

#pragma once
#include <CoreLib/Math/Vector4.h>
#include "ComponentRendererImGui.h"

using SubscriberID = size_t;

struct ImFont;
namespace EngineCore {

	class Debugger;
	class DebuggerWindows {
	public:
		DebuggerWindows(Debugger* debugger);
		~DebuggerWindows();
		void Shutdown();
		void SetIconFonts(ImFont* smallIcon, ImFont* largeIcon);
		void MenuSidebar(float sidebarRatio, int windowWidth, int windowHeight);
		void StatsWindow();
		void CameraWindow();
		void DrawGameObjectNode(std::shared_ptr<GameObject>& obj);
		void HierarchyWindow();
		void InspectorWindow();
		void IconDisplayWindow();
		
	private:
		ImGuiRenderer m_uiRenderer;
		Debugger* m_debugger = nullptr;

		ImFont* m_smallIconFont = nullptr;
		ImFont* m_largeIconFont = nullptr;

		bool m_statsWin = false;
		bool m_cameraWin = false;
		bool m_hierarchyWin = false;// and inspector
		bool m_iconWin = false;

		// uses state vars for pos and size of the windows if first time opening
		bool m_firstStatsWin = true;
		bool m_firstCameraWin = true;
		bool m_firstHierarchyWin = true;
		bool m_firstInspectorWin = true;
		bool m_firstIconWin = true;

		// pos, size				x	y	w	h

		Vector4 m_statsWinState = { 80, 30, 250, -1 };
		Vector4 m_cameraWinState = { 80, 50, 300, 400 };
		Vector4 m_hierarchyWinState = { 80, 70, 250, 400 };
		Vector4 m_inspectorWinState = { 80, 70, 250, 400 };
		Vector4 m_iconWinState = { 80, 90, 375, 250 };

		std::vector<std::string> m_log;
		Log::SubscriberID m_subscriberId;
	};

}

#endif