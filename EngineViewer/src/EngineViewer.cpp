#include "EngineViewer.h"

using namespace EngineCore;

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
	App_OpenGL_Set_DepthTesting(true);
	App_OpenGL_Set_BackgroundColor(0.2f, 0.3f, 0.3f);
	// App_OpenGL_Set_PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	App_OpenGL_Set_FaceCulling(true);

	auto go = GameObject::Create("Test");
	go->AddComponent<Component::MeshRenderer>()
		->SetMesh(ASSETS::ENGINE::MESH::Cube())
		->SetMaterial(ASSETS::ENGINE::MATERIAL::Default());
}

void EngineViewer::Update() {
    if (Input::KeyJustPressed(KeyCode::ESCAPE))
        App_Application_Set_WindowClose();

	if (Input::KeyJustPressed(KeyCode::H)) {
		App_Debug_Set_Active(!App_Debug_Get_Active());
	}
}

void EngineViewer::Shutdown() {
}