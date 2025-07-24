#include "MyGameApp.h"
#include "..\Engine\Input.h"
#include "..\Core\Log.h"

MyGameApp::MyGameApp()
	: Application("MyGameApp", "1.0.0") {
}

void MyGameApp::OnStart() {
}

void MyGameApp::OnUpdate() {
	if (EngineCore::Input::IsKeyPressed(GLFW_KEY_A)) {
		Log::Info("A pressed");
	}
}

void MyGameApp::OnShutdown() {
}