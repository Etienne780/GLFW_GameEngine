#pragma once

struct ImFont;

namespace EngineCore {

	namespace DebugWindow {

		void SetIconFonts(ImFont* smallIcon, ImFont* largeIcon);

		void MenuSidebar(float sidebarRatio, int windowWidth, int windowHeight);

		void IconDisplayWindow(float startX, float startY);
	}

}