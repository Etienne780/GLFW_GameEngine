#include <GLFW\glfw3.h>

#include "Engine\Engine.h"
#include "Game\MyGameApp.h"

#include "Core\Log.h"

#define WINDOW_TITLE "GLFW-Test"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void glfw_error_callback(int error, const char* description);
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

	glfwSetKeyCallback(window, glfw_key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	auto app = std::make_unique<MyGameApp>();
	EngineCore::Engine engine(std::move(app));
	engine.Start(window);

	// glfwSetWindowCloseCallback
	// glfwSetWindowShouldClose
	while (!glfwWindowShouldClose(window))
	{
		double time = glfwGetTime();// is in sec
		engine.Update(time);

		glfwSwapBuffers(window);
		glfwPollEvents();
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

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

