#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "UIHelper.h"
#include "UI.h"

std::unordered_map<UI::UIParamName, UI::UIParam> UI::params;

enum UI::UIParamName {
	PARAM = 0,
	PARAMda
};

float d = 10.0f;
void UI::Setup(GLFWwindow* window, GLFWmonitor* primaryMonitor) {

}

void DrawUI() {
	ImGui::Begin("AssetsManager");
	ImGui::End();

	ImGui::Begin("Fenster A");
	ImGui::Text("Ich kann gedockt werden");
	ImGui::End();

	ImGui::Begin("Fenster B");
	ImGui::Text("Mich kannst du auch andocken");
	ImGui::End();
}

void UI::Draw() {
	UIHelper::StartDraw();
	UIHelper::CreateDockingArea();
	DrawUI();
	UIHelper::EndDraw();
}

void UI::Shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}