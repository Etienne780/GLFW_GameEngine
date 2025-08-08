#include "EngineViewer.h"

using namespace EngineCore;

EngineViewer::EngineViewer()
	: Application("Engine Viewer", "1.0.0") {

	App_Application_Set_Header(true);
	App_Application_Set_CloseAppOnWindowClose(true);
	App_Application_Set_Window_Resizable(true);
	App_Application_Set_Window_Decoration(true);
	App_Application_Set_Window_Floating(false);
	App_Application_Set_Window_Cursor_LockHidden(true);

	App_OpenGL_Set_Version(3, 3);
	App_OpenGL_Set_DepthTesting(true);

	App_Application_Set_Header(false);

	Log::SaveLogs("Logs/");
}

void EngineViewer::Start() {

}

void EngineViewer::Update() {

}

void EngineViewer::Shutdown() {

}