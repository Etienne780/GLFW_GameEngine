#include <GLFW\glfw3.h>

#include "Engine\Engine.h"
#include "Game\MyGameApp.h"

#include "Core\Log.h"

#define WINDOW_TITLE "GLFW-Test"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void glfw_error_callback(int error, const char* description);

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		Log::Error("Initialization of GLFW faild!");
		exit(EXIT_FAILURE);
	}
	Log::Info("Initialized GLFW successfully");

	// Creates the window
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
		Log::Error("Window or OpenGL context creation failed!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	auto app = std::make_unique<MyGameApp>();
	EngineCore::Engine engine(std::move(app), window);
	engine.Start();

	// glfwSetWindowCloseCallback
	// glfwSetWindowShouldClose
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		double time = glfwGetTime();// is in sec
		engine.Update(time);
		engine.LateUpdate();

		glfwSwapBuffers(window);
	}

	engine.Shutdown();

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void glfw_error_callback(int error, const char* description)
{
	Log::Error("GLFW Error: {}, {}", error, description);
}

