#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <String>

#include <CoreLib\Math\Vector3.h>

class Vector2;

namespace EngineCore {
	class Engine;
	class Shader;
}

class Application {
	friend class EngineCore::Engine;

public:
	enum class WindowState {
		Minimized,
		Maximized,
		Restored,    // Normal Windowsize
		Fullscreen,
		Hidden
	};

	Application(std::string name, std::string version);
	virtual ~Application() = default;

	virtual void Start();
	virtual void Update();
	virtual void Shutdown();

	virtual void OnWindowResize(int newWidth, int newHeight);
	virtual void OnWindowFocusLost();
	virtual void OnWindowFocusGain();

	static Application* Get();

	const std::string& App_Application_Get_Name() const;
	const std::string& App_Application_Get_Version() const;
	GLFWwindow* App_Application_Get_Window() const;
	int App_Application_Get_Window_Height() const;
	int App_Application_Get_Window_Width() const;
	bool App_Application_Get_Window_Resizable() const;
	bool App_Application_Get_Window_Decoration() const;
	bool App_Application_Get_Window_Floating() const;
	bool App_Application_Get_Window_Visibility() const;
	bool App_Application_Get_Window_Cursor_Lock() const;
	bool App_Application_Get_Window_Cursor_Hidden() const;
	int App_Application_Get_FrameCount() const;
	int App_Application_Get_FramesPerSecond() const;
	bool App_Application_Get_Header() const;
	bool App_Application_Get_CloseAppOnWindowClose() const;

	void App_Application_Set_Header(bool value);
	void App_Application_Set_Window_Height(int height);
	void App_Application_Set_Window_Width(int width);
	void App_Application_Set_Window_Resizable(bool value);
	void App_Application_Set_Window_Decoration(bool value);
	void App_Application_Set_Window_Floating(bool value);
	void App_Application_Set_Window_Visibility(bool value);
	void App_Application_Set_Window_Cursor_LockHidden(bool value);
	void App_Application_Set_Window_Cursor_Hidden(bool value);
	void App_Application_Set_CloseAppOnWindowClose(bool value);
	

	void App_OpenGL_Get_Version(int& major, int& minor) const;
	bool App_OpenGL_Get_DepthTesting() const;
	void App_OpenGL_Get_BackgroundColor(float& r, float& g, float& b) const;
	const Vector3& App_OpenGL_Get_BackgroundColor() const;

	void App_OpenGL_Set_Version(int major, int minor);
	// cant be called in the constructor. needs a window
	void App_OpenGL_Set_DepthTesting(bool value);
	void App_OpenGL_Set_BackgroundColor(float r, float g, float b);
	void App_OpenGL_Set_BackgroundColor(float brightness);
	void App_OpenGL_Set_BackgroundColor(const Vector3& color);

	// needs a window
	void App_Shader_Bind(EngineCore::Shader* shader);
	// needs a window
	void App_Shader_Unbind(EngineCore::Shader* shader);
	// needs a window
	void App_Shader_Delete(EngineCore::Shader* shader);

private:
	static Application* s_instance;

	const std::string m_appApplicationName;
	const std::string m_appApplicationVersion;
	bool m_appApplicationHeader = true;
	int m_appApplicationWindowHeight = 600;
	int m_appApplicationWindowWidth = 800;
	int m_appApplicationFrameCount = 0;
	int m_appApplicationFramesPerSecond = 0;
	bool m_appApplicationWindowResizable = true;
	bool m_appApplicationWindowDecoration = true;
	bool m_appApplicationWindowFloating = false;
	bool m_appApplicationWindowVisibility = true;
	bool m_appApplicationWindowCursorLock = false;
	bool m_appApplicationWindowCursorHidden = false;
	WindowState m_appApplicationWindowState = WindowState::Restored;// not implementedasdflkdgs;
	bool m_appApplicationCloseAppOnWindowClose = true;

	int m_appOpenGLVersionMajor = 3;
	int m_appOpenGLVersionMinor = 3;
	bool m_appOpenGLDepthTesting = false;
	Vector3 m_appOpenGLBackgroundColor;

	EngineCore::Shader* m_appShaderCurrentShader = nullptr;

	GLFWwindow* m_window = nullptr;
};