#pragma once

struct ImFont;

namespace EngineCore {

	namespace DebugWindow {

		void SetIconFonts(ImFont* smallIcon, ImFont* largeIcon);

		void MenuSidebar(float sidebarRatio, float windowWidth, float windowHeight);

		void IconDisplayWindow(float startX, float startY);
	}

}