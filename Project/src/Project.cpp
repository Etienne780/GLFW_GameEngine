#include "Scenes.h"
#include "Project.h"

using namespace EngineCore;

Project::Project()
	: Application("Project", "1.0.0") {

	App_Application_Set_Header(true);
	App_Application_Set_CloseAppOnWindowClose(true);
	App_Application_Set_Window_Resizable(true);
	App_Application_Set_Window_Decoration(true);
	App_Application_Set_Window_Floating(false);
	App_Application_Set_Window_Cursor_LockHidden(true);
	App_Application_Set_Header(true);

	App_OpenGL_Set_Version(3, 3);

	// Log::SaveLogs("Logs/");
}

void Project::Start() {
	App_OpenGL_Set_DepthTesting(true);
	App_OpenGL_Set_BackgroundColor(0.2f, 0.3f, 0.3f);
	// App_OpenGL_Set_PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	App_OpenGL_Set_FaceCulling(true);

	Scenes::LoadTest();
}

void Project::Update() {
	if (Input::KeyPressed(KeyCode::ESCAPE))
		App_Application_CloseWindow();

	if (Input::KeyJustPressed(KeyCode::L)) {
		App_Application_Set_Window_Floating(!App_Application_Get_Window_Floating());
	}

	if (Input::KeyJustPressed(KeyCode::H)) {
		App_Debug_Set_Active(!App_Debug_Get_Active());
	}
}

void Project::Shutdown() {
}

void Project::OnWindowResize(int newWidth, int newHeight) {
}

void Project::OnWindowFocusLost() {
}

void Project::OnWindowFocusGain() {
}