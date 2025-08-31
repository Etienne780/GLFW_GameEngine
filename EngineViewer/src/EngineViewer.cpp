#include <EngineLib/ComponentRendererImGui.h>
#include "EngineViewer.h"
#include "AssetManager.h"
#include "ProjectManager.h"
#include "Exporter.h"


using namespace EngineCore;

static AssetManager g_AssetManager;
static ProjectManager g_ProjectManager;
static Exporter g_Exporter;

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

	// opens file dialog to open a project
	g_ProjectManager.Open();
}

void EngineViewer::Update() {
    if (Input::KeyJustPressed(KeyCode::ESCAPE))
        App_Application_Set_WindowClose();

	// Top bar
	g_ProjectManager.Update();
	// Renders and updates asset manager
	g_AssetManager.Update();
}

void EngineViewer::Shutdown() {
	g_ProjectManager.Close();
}

/*
What is the Engine Viewer?
The Engine Viewer is a helper programm that SHOULD speed up development.
The main reason to use this programm is for Implementing assets and setting up GOs.
First the Engine Viewer will generate a file where 1. the path to an assets is saved or it will contain
the raw data, so that no extra files are need outside of the .exe and .dll files.
If the Assets.h files gets included a namespace ASSETS will become accessible, in it are functions and other namespaces
that represent the folder structer of in the Engine Viewer. the functions in the namespaces return the indexes of the assets
so that the indexes can be given to other objects

- Open Engine Viewer
- Selecet the Folder where the project is located
- - in the folder will be a save file with saved setupn data for the Engine Viewer
- Edit Data
- Export the current open project
- Engine Viewer will generate Assets.h in the project include path if it has one. if not it will put the file in the src folder
  if that folder alos dosent exist it will just put it in the project. To prevent this the exoprt path can be manuelly set in the settings

- Include the exportet file Assets.h
- call the Assets Setup function to setup alle the important data
- use it 
*/