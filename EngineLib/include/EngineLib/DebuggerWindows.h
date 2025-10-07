#ifndef NDEBUG

#pragma once
#include <CoreLib/Math/Vector4.h>
#include "ComponentRendererImGui.h"
#include "EngineLib/UI/Elements/Element.h"

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
		void DrawGameObjectNode(const std::shared_ptr<GameObject>& obj);
		void HierarchyWindow();
		void InspectorWindow();
		void ConsoleWindow();
		/**
		* @brief Draws a single UI element and its children in a collapsible HTML-like tree node
		* @param elem Shared pointer to the UI element to draw
		*/
		void DrawUIElementNode(const std::shared_ptr<UI::ElementBase>& elem);
		void UIHierarchyWindow();
		void UIInspectorWindow();
		void IconDisplayWindow();
		
	private:
		ImGuiRenderer m_uiRenderer;
		Debugger* m_debugger = nullptr;

		ImFont* m_smallIconFont = nullptr;
		ImFont* m_largeIconFont = nullptr;

		bool m_statsWin = false;
		bool m_cameraWin = false;
		bool m_hierarchyWin = false;// and inspector
		bool m_consoleWin = false;
		bool m_uiInspectorWin = false;
		bool m_iconWin = false;

		// uses state vars for pos and size of the windows if first time opening
		bool m_firstStatsWin = true;
		bool m_firstCameraWin = true;
		bool m_firstHierarchyWin = true;
		bool m_firstInspectorWin = true;
		bool m_firstConsoleWin = true;
		bool m_firstUIHierarchyWin = true;
		bool m_firstUIInspectorWin = true;
		bool m_firstIconWin = true;

		// pos, size				x	y	w	h

		Vector4 m_statsWinState = { 80, 30, 250, 150 };
		Vector4 m_cameraWinState = { 80, 50, 300, 400 };
		Vector4 m_hierarchyWinState = { 80, 70, 250, 400 };
		Vector4 m_inspectorWinState = { 80, 70, 250, 400 };
		Vector4 m_consoleWinState = { 80, 90, 425, 300 };
		Vector4 m_uiHierarchyState = { 80, 100, 250, 400 };
		Vector4 m_uiInspectorState = { 80, 110, 250, 400 };
		Vector4 m_iconWinState = { 80, 120, 375, 250 };

		struct LogMsg {
			std::string m_msg;
			Log::Level m_logLevel;
			LogMsg(Log::Level logLevel, const std::string& msg) 
				: m_logLevel(logLevel), m_msg(msg) {};
		};
		std::vector<LogMsg> m_log;
		// info = 1 bit, warn = 2 bit, error = 3 bit, debug = 4 bit
		uint8_t m_logMask = 0b1111;
		Log::SubscriberID m_subscriberId;
	};

}

#endif