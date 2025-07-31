#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <String>

#include "..\Core\Math\Vector3.h"


typedef std::string String;

namespace EngineCore {
	class Engine;
	class Shader;
}

class Application {
	friend class EngineCore::Engine;

public:
	const String app_name;
	const String app_version;

	int app_window_height = 600;
	int app_window_width = 800;

	int app_frameCount = 0;
	int app_framesPerSecond = 0;

	Application(String name, String version);
	virtual ~Application() = default;

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnShutdown();

	virtual void OnWindowResize(int newWidth, int newHeight);

	void App_Background_SetColor(float r, float g, float b);
	void App_Background_SetColor(float brightness);
	void App_Background_SetColor(const Vector3& color);
	void App_Background_Clear();

	void App_Shader_Bind(EngineCore::Shader* shader);
	void App_Shader_Unbind(EngineCore::Shader* shader);
	void App_Shader_Delete(EngineCore::Shader* shader);

	GLFWwindow* GetWindow();

private:
	Vector3 m_backgroundColor;
	GLFWwindow* m_window = nullptr;

	EngineCore::Shader* m_currentShader = nullptr;
};