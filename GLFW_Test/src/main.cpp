#include "Game\MyGameApp.h"

void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

std::unique_ptr<EngineCore::Engine> engine;

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		Log::Error("Initialization of GLFW faild!");
		return EXIT_FAILURE;
	}
	Log::Info("Initialized GLFW successfully");

	auto app = std::make_unique<MyGameApp>();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(app->windowWidth, app->windowHeight, app->name.c_str(), NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
		Log::Error("Window or OpenGL context creation failed!");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Log::Error("Failed to initialize GLAD!");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glViewport(0, 0, app->windowWidth, app->windowHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1);

	engine = std::make_unique<EngineCore::Engine>(std::move(app), window);
	engine->Start();

	while (!glfwWindowShouldClose(window)) {
		double time = glfwGetTime();// is in sec
		engine->Update(time);
		engine->LateUpdate();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	engine->Shutdown();
	engine.reset();
	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void glfw_error_callback(int error, const char* description)
{
	Log::Error("GLFW Error: {}, {}", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	if (engine)
		engine->WindowResize(width, height);
}