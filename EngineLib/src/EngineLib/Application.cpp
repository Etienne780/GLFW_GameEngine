#include "EngineLib\Application.h"

#include "EngineLib\Shader.h"
#include "CoreLib\Log.h"
#include "CoreLib\Math.h"

Application* Application::s_instance = nullptr;

Application::Application(std::string name, std::string version)
    : m_appApplicationName(name), m_appApplicationVersion(version) {
    s_instance = this;
    m_appOpenGLBackgroundColor = Vector3(0.2f, 0.3f, 0.3f);
}

void Application::Start() {}
void Application::Update() {}
void Application::Shutdown() {}

void Application::OnWindowResize(int newWidth, int newHeight) {}
void Application::OnWindowFocusLost() {}
void Application::OnWindowFocusGain() {}

#pragma region get_funcs

Application* Application::Get() {
    return s_instance;
}

// Application
const std::string& Application::App_Application_Get_Name() const {
    return m_appApplicationName;
}

const std::string& Application::App_Application_Get_Version() const {
    return m_appApplicationVersion;
}

GLFWwindow* Application::App_Application_Get_Window() const {
    return m_window;
}

int Application::App_Application_Get_Window_Height() const {
    return m_appApplicationWindowHeight;
}

int Application::App_Application_Get_Window_Width() const {
    return m_appApplicationWindowWidth;
}

bool Application::App_Application_Get_Window_Resizable() const {
    return m_appApplicationWindowResizable;
}

bool Application::App_Application_Get_Window_Decoration() const {
    return m_appApplicationWindowDecoration;
}

bool Application::App_Application_Get_Window_Floating() const {
    return m_appApplicationWindowFloating;
}

bool Application::App_Application_Get_Window_Visibility() const {
    return m_appApplicationWindowVisibility;
}

bool Application::App_Application_Get_Window_Cursor_Lock() const {
    return m_appApplicationWindowCursorLock;
}

bool Application::App_Application_Get_Window_Cursor_Hidden() const {
    return m_appApplicationWindowCursorHidden;
}

int Application::App_Application_Get_FramesPerSecond() const {
    return m_appApplicationFramesPerSecond;
}

bool Application::App_Application_Get_Header() const {
    return m_appApplicationHeader;
}

bool Application::App_Application_Get_CloseAppOnWindowClose() const {
    return m_appApplicationCloseAppOnWindowClose;
}

// OpenGL
void Application::App_OpenGL_Get_Version(int& major, int& minor) const {
    major = m_appOpenGLVersionMajor;
    minor = m_appOpenGLVersionMinor;
}

bool Application::App_OpenGL_Get_DepthTesting() const {
    return m_appOpenGLDepthTesting;
}

void Application::App_OpenGL_Get_BackgroundColor(float& r, float& g, float& b) const {
    r = m_appOpenGLBackgroundColor.x;
    g = m_appOpenGLBackgroundColor.y;
    b = m_appOpenGLBackgroundColor.z;
}
const Vector3& Application::App_OpenGL_Get_BackgroundColor() const {
    return m_appOpenGLBackgroundColor;
}

bool Application::App_OpenGL_Get_ManuallyClearBackground() const {
    return m_appOpenGLManuallyClearBackground;
}

#pragma endregion

#pragma region set_funcs

void Application::App_Application_Set_Header(bool value) {
    m_appApplicationHeader = value;
}

void Application::App_Application_Set_Window_Height(int height) {
    m_appApplicationWindowHeight = height;
}

void Application::App_Application_Set_Window_Width(int width) {
    m_appApplicationWindowWidth = width;
}

void Application::App_Application_Set_Window_Resizable(bool value) {
    m_appApplicationWindowResizable = value;

    if (m_window != nullptr)
        glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, value ? GLFW_TRUE : GLFW_FALSE);
}

void Application::App_Application_Set_Window_Decoration(bool value) {
    m_appApplicationWindowDecoration = value;

    if (m_window != nullptr)
        glfwSetWindowAttrib(m_window, GLFW_DECORATED, value ? GLFW_TRUE : GLFW_FALSE);
}

void Application::App_Application_Set_Window_Floating(bool value) {
    m_appApplicationWindowFloating = value;

    if (m_window != nullptr)
        glfwSetWindowAttrib(m_window, GLFW_FLOATING, value ? GLFW_TRUE : GLFW_FALSE);
}

void Application::App_Application_Set_Window_Visibility(bool value) {
    m_appApplicationWindowVisibility = value;
}

void Application::App_Application_Set_Window_Cursor_LockHidden(bool value) {
    m_appApplicationWindowCursorLock = value;
    m_appApplicationWindowCursorHidden = value;

    if (m_window != nullptr)
        glfwSetInputMode(m_window, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Application::App_Application_Set_Window_Cursor_Hidden(bool value) {
    m_appApplicationWindowCursorHidden = value;

    if (m_window != nullptr)
        glfwSetInputMode(m_window, GLFW_CURSOR, value ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
}

void Application::App_Application_Set_CloseAppOnWindowClose(bool value) {
    m_appApplicationCloseAppOnWindowClose = value;
}

void Application::App_Application_Set_WindowClose() {
    if (m_window != nullptr)
     glfwSetWindowShouldClose(m_window, true);
}

void Application::App_OpenGL_Set_DepthTesting(bool value) {
    m_appOpenGLDepthTesting = value;

    if (m_window == nullptr) return;
    if(value)
        glEnable(GL_DEPTH_TEST);
    else 
        glDisable(GL_DEPTH_TEST);
}

void Application::App_OpenGL_Set_Version(int major, int minor) {
    m_appOpenGLVersionMajor = major;
    m_appOpenGLVersionMinor = minor;
}

void Application::App_OpenGL_Set_BackgroundColor(float r, float g, float b) {
    m_appOpenGLBackgroundColor = Vector3(r, g, b);
}

void Application::App_OpenGL_Set_BackgroundColor(float brightness) {
    m_appOpenGLBackgroundColor = Vector3(brightness, brightness, brightness);
}

void Application::App_OpenGL_Set_BackgroundColor(const Vector3& color) {
    m_appOpenGLBackgroundColor = color;
}

void Application::App_OpenGL_Set_ManuallyClearBackground(bool value) {
    m_appOpenGLManuallyClearBackground = value;
}

#pragma endregion

#pragma region other_funcs

void Application::App_OpenGL_BackgroundColor() const {
    if (m_window != nullptr) {
        glClearColor(m_appOpenGLBackgroundColor.x,
            m_appOpenGLBackgroundColor.y,
            m_appOpenGLBackgroundColor.z,
            1.0f);
        glClear(((m_appOpenGLDepthTesting) ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT));
    }
}

#pragma endregion