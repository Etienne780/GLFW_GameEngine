#include "Application.h"

#include "Shader.h"

#include "..\Core\Log.h"
#include "..\Core\Math.h"

Application::Application(String name, String version)
    : app_name(name), app_version(version) {
    m_backgroundColor = Vector3(0.2f, 0.3f, 0.3f);
}

void Application::OnStart() {}
void Application::OnUpdate() {}
void Application::OnShutdown() {}

void Application::OnWindowResize(int newWidth, int newHeight) {}

void Application::App_Background_SetColor(float r, float g, float b) {
    m_backgroundColor = Vector3(r, g, b);
}

void Application::App_Background_SetColor(float brightness) {
    m_backgroundColor = Vector3(brightness, brightness, brightness);
}

void Application::App_Background_SetColor(const Vector3& color) {
    m_backgroundColor = color;
}

void Application::App_Background_Clear() {

    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Application::App_Shader_Bind(Shader* shader) {
    if (shader == nullptr) return;

    if (m_currentShader != nullptr && m_currentShader != shader) {
        m_currentShader->Unbind();
    }

    m_currentShader = shader;
    m_currentShader->Bind();
}

void Application::App_Shader_Unbind(Shader* shader) {
    if (shader == nullptr) return;

    if (m_currentShader != nullptr && m_currentShader == shader) {
        shader->Unbind();
        m_currentShader = nullptr;
    }
}

void Application::App_Shader_Delete(Shader* shader) {
    if (shader == nullptr) return;

    if (m_currentShader != nullptr && m_currentShader == shader) {
        m_currentShader = nullptr;
    }

    shader->Delete();
}

GLFWwindow* Application::GetWindow() {
    return m_window;
}