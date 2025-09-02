#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include "UI/ProjectManagerUI.h"
#include "UI/AssetManagerUI.h"
#include "UI/UIManager.h"

#include <GLFW/glfw3.h>

void UIManager::Init(void* window, ProjectManager* projectManager, AssetManager* assetManager) {
    m_projectManagerUI = std::make_unique<ProjectManagerUI>(projectManager);
    m_assetManagerUI = std::make_unique<AssetManagerUI>(assetManager);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UIManager::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::Render() {
    BeginFrame();

    if (!m_projectManagerUI->GetManager()->HasProjectOpen()) {
        m_projectManagerUI->DrawSelectProject();
    }
    else {
        
    }

    // Render final
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}