#pragma once
#include <String>
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "..\Core\Log.h"
#include "..\Core\Math.h"

typedef std::string String;

namespace EngineCore {
	class Engine;
}

class Application {
	friend class EngineCore::Engine;

public:
	const String name;
	const String version;

	int window_Height = 600;
	int window_Width = 800;

	int app_frameCount = 0;

	Application(String name, String version);
	virtual ~Application() = default;

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnShutdown();

	virtual void OnWindowResize(int newWidth, int newHeight);

	void App_SetBackgroundColor(float r, float g, float b, float a);
	void App_SetBackgroundColor(float r, float g, float b);
	void App_SetBackgroundColor(float brightness);
	void App_SetBackgroundColor(const Vector4& color);
	void App_BackgroundClear();

	GLFWwindow* GetWindow();

private:
	Vector4 m_backgroundColor;
	GLFWwindow* m_window;
};