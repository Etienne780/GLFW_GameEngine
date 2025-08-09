#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "EngineViewerDataStruct.h"
#include "UIHelper.h"
#include "UI.h"

EngineViewerData* engineViewerData;
void UI::Setup(EngineViewerData* data, GLFWwindow* window, GLFWmonitor* primaryMonitor) {
	UIHelper::SetupImGui(window, primaryMonitor);

    engineViewerData = data;
}

void DrawUI() {
    UIHelper::DrawProjectSelectPopUp(engineViewerData);
}


void UI::StartDraw() {
	UIHelper::StartDraw();
	UIHelper::CreateDockingArea();

    static bool openOnce = true;
    if (openOnce) {
        ImGui::OpenPopup("Select ProjectPath");
        openOnce = false;
    }
}

void UI::EndDraw() {
    DrawUI();
    UIHelper::EndDraw();
}

void UI::Shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}