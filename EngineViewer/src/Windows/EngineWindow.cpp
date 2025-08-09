#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "Windows\EngineWindow.h"

EngineWindow::EngineWindow(const char* name) {
	m_name = name;
}

void EngineWindow::Draw() {
	ImGui::Begin(m_name);

	DrawInter();

	ImGui::End();
}

void EngineWindow::DrawInter() {}

std::string EngineWindow::GetName() const {
	return std::string(m_name);
}

void EngineWindow::Init(GLFWwindow* window, GLFWmonitor* primaryMonitor) {
	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(primaryMonitor);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_io = &ImGui::GetIO(); (void)m_io;
	m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Docking aktivieren
	// Optional: Multi-Viewport (Fenster außerhalb der Haupt-Render-Fläche)
	m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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