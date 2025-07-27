#include "Application.h"

Application::Application(String name, String version)
    : name(name), version(version) {
}

void Application::OnStart() {}
void Application::OnUpdate() {}
void Application::OnShutdown() {}

void Application::OnWindowResize(int newWidth, int newHeight) {}

void Application::App_SetBackgroundColor(float r, float g, float b, float a) {
    m_backgroundColor = Vector4(r, g, b, a);
}

void Application::App_SetBackgroundColor(float r, float g, float b) {
    m_backgroundColor = Vector4(r, g, b, 1.0f);
}

void Application::App_SetBackgroundColor(float brightness) {
    m_backgroundColor = Vector4(brightness, brightness, brightness, 1.0f);
}

void Application::App_SetBackgroundColor(const Vector4& color) {
    m_backgroundColor = color;
}

void Application::App_BackgroundClear() {

    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow* Application::GetWindow() {
    return m_window;
}