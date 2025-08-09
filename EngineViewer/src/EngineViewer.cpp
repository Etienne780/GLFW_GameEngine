#include "EngineViewerDataStruct.h"
#include "UI.h"
#include "EngineViewer.h"

using namespace EngineCore;

EngineViewerData engineViewerData;
EngineViewer::EngineViewer()
	: Application("Engine Viewer", "1.0.0") {

	App_Application_Set_CloseAppOnWindowClose(true);
	App_Application_Set_Window_Resizable(true);
	App_Application_Set_Window_Decoration(true);
	App_Application_Set_Window_Floating(false);
	App_Application_Set_Window_Cursor_LockHidden(false);

	App_OpenGL_Set_Version(3, 3);
	App_OpenGL_Set_DepthTesting(true);

	// Log::SaveLogs("Logs/");
}


void EngineViewer::Start() {
	UI::Setup(&engineViewerData, App_Application_Get_Window(), glfwGetPrimaryMonitor());
}

void EngineViewer::Update() {
    if (Input::KeyJustPressed(GLFW_KEY_ESCAPE))
        App_Application_Set_WindowClose();

	UI::StartDraw();
	// RenderGame
	UI::EndDraw();// Renders UI
}

void EngineViewer::Shutdown() {
	UI::Shutdown();
}