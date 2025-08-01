#include "Application.h"

#include "Shader.h"

#include "..\Core\Log.h"
#include "..\Core\Math.h"


Application::Application(String name, String version)
    : m_appApplicationName(name), m_appApplicationVersion(version) {
    m_appOpenGLBackgroundColor = Vector3(0.2f, 0.3f, 0.3f);
}

void Application::OnStart() {}
void Application::OnUpdate() {}
void Application::OnShutdown() {}

void Application::OnWindowResize(int newWidth, int newHeight) {}

#pragma region get_funcs

// Application
const String& Application::App_Application_Get_Name() const {
    return m_appApplicationName;
}

const String& Application::App_Application_Get_Version() const {
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

int Application::App_Application_Get_FrameCount() const {
    return m_appApplicationFrameCount;
}

int Application::App_Application_Get_FramesPerSecond() const {
    return m_appApplicationFramesPerSecond;
}

bool Application::App_Application_Get_WindowHeader() const {
    return m_appApplicationWindowHeader;
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

EngineCore::Shader* Application::App_Shader_Get_Bind() {
    if (m_window == nullptr) return nullptr;

    return m_appShaderCurrentShader;
}

#pragma endregion

#pragma region set_funcs

void Application::App_Application_Set_WindowHeader(bool value) {
    m_appApplicationWindowHeader = value;
}

void Application::App_Application_Set_CloseAppOnWindowClose(bool value) {
    m_appApplicationCloseAppOnWindowClose = value;
}

void Application::App_OpenGL_Set_DepthTesting(bool value) {
    if (m_window == nullptr) return;

    m_appOpenGLDepthTesting = value;
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

#pragma endregion

#pragma region other_funcs

void Application::App_OpenGL_BackgroundColor() {
    if (m_window == nullptr) return;

    glClearColor(m_appOpenGLBackgroundColor.x, m_appOpenGLBackgroundColor.y, m_appOpenGLBackgroundColor.z, 1.0f);
    glClear(((m_appOpenGLDepthTesting) ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT));
}

void Application::App_Shader_Bind(EngineCore::Shader* shader) {
    if (shader == nullptr || m_window == nullptr) return;

    if (m_appShaderCurrentShader != nullptr && m_appShaderCurrentShader != shader) {
        m_appShaderCurrentShader->Unbind();
    }

    m_appShaderCurrentShader = shader;
    m_appShaderCurrentShader->Bind();
}

void Application::App_Shader_Unbind(EngineCore::Shader* shader) {
    if (shader == nullptr || m_window == nullptr) return;

    if (m_appShaderCurrentShader != nullptr && m_appShaderCurrentShader == shader) {
        shader->Unbind();
        m_appShaderCurrentShader = nullptr;
    }
}

void Application::App_Shader_Delete(EngineCore::Shader* shader) {
    if (shader == nullptr || m_window == nullptr) return;

    if (m_appShaderCurrentShader != nullptr && m_appShaderCurrentShader == shader) {
        m_appShaderCurrentShader = nullptr;
    }

    shader->Delete();
}

#pragma endregion