#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "UI.h"

std::unordered_map<std::string, UI::UIParam> UI::params;

ImGuiIO* m_io;
float d = 10.0f;
void UI::Setup(GLFWwindow* window, GLFWmonitor* primaryMonitor) {
	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(primaryMonitor);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_io = &ImGui::GetIO(); (void)m_io;
	m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);// Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale;

	const char* glsl_version = "#version 150";
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}
void DrawHierarchyInspector();

void UI::Draw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("AssetsManager");
	ImGui::End();

	DrawHierarchyInspector();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ToggleLogic(const char* name, bool& isHierarchy, bool& windowSwitched, ImVec2& lastWinPos, ImVec2& lastWinSize);
void DrawHierarchyInspector() {
	static bool isHierarchy = false;
	static bool windowSwitched = false;
	static ImVec2 lasWinPosition;
	static ImVec2 lasWinSize;

	if (windowSwitched) {
		windowSwitched = false;
		ImGui::SetNextWindowPos(lasWinPosition);
		ImGui::SetNextWindowSize(lasWinSize);
	}

	if (isHierarchy) {
		ImGui::Begin("Hierarchy");
		ToggleLogic("Inspector", isHierarchy, windowSwitched, lasWinPosition, lasWinSize);
		ImGui::End();
	}
	else {
		ImGui::Begin("Inspector");
		ToggleLogic("Hierarchy", isHierarchy, windowSwitched, lasWinPosition, lasWinSize);
		ImGui::End();
	}
}

void ToggleLogic(const char* name, bool& isHierarchy, bool& windowSwitched, ImVec2& lastWinPos, ImVec2& lastWinSize) {
	if (ImGui::Button(name)) {
		isHierarchy = !isHierarchy;
		windowSwitched = true;
	}
	lastWinPos = ImGui::GetWindowPos();
	lastWinSize = ImGui::GetWindowSize();
}

void UI::Shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}