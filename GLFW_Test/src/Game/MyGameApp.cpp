#include "MyGameApp.h"

using namespace EngineCore;

MyGameApp::MyGameApp()
	: Application("MyGameApp", "1.0.0") {
}

void MyGameApp::OnStart() {
	SetBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void MyGameApp::OnUpdate() {
	GLFWwindow* window = GetWindow();

	if (Input::KeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	BackgroundClear();
}

void MyGameApp::OnShutdown() {
}

void MyGameApp::OnWindowResize(int newWidth, int newHeight) {
}