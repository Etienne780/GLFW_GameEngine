#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "CoreLib\File.h"
#include "EngineLib\Application.h"
#include "EngineViewerDataStruct.h"
#include "UIHelper.h"


namespace UIHelper {
	ImGuiIO* imGuiIo;
	void StartDraw() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EndDraw() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void SetupImGui(GLFWwindow* window, GLFWmonitor* primaryMonitor) {
		float windowScaler = 1.4;
		float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(primaryMonitor);
		main_scale *= windowScaler;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		imGuiIo = &ImGui::GetIO(); (void)imGuiIo;
		imGuiIo->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		imGuiIo->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		imGuiIo->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Docking aktivieren
		// Optional: Multi-Viewport (Fenster außerhalb der Haupt-Render-Fläche)
		imGuiIo->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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

	void CreateDockingArea() {
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// Fenster-Flags
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// Haupt-Dockspace Fenster
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

		// Dockspace erzeugen
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		ImGui::End();

	}

	const float popMinWidth = 300;
	const float popMinHeight = 110;
	float popWidth = popMinWidth;
	float popHeight = popMinHeight;
	void DrawProjectSelectPopUp(EngineViewerData* engineViewerData) {

		{
			// center window
			Application* app = Application::Get();
			int w = app->App_Application_Get_Window_Width();
			int h = app->App_Application_Get_Window_Height();

			ImGui::SetNextWindowPos(ImVec2(w/2 - popWidth/2, h/2 - popHeight/2));
		}

		ImGui::SetNextWindowSizeConstraints(ImVec2(popMinWidth, popMinHeight), ImVec2(FLT_MAX, FLT_MAX));
		if (ImGui::BeginPopupModal("Select ProjectPath", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			ImGui::Text("Path: %s", engineViewerData->projectPath.c_str());

			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			// Buttons mittig mit Abstand
			popWidth = ImGui::GetWindowWidth();
			popHeight = ImGui::GetWindowHeight();
			float buttonWidth = 100.0f;
			float buttonSpacing = 20.0f;
			float buttonsTotalWidth = buttonWidth * 2 + buttonSpacing;
			ImGui::SetCursorPosX((popWidth - buttonsTotalWidth) * 0.5f);

			if (ImGui::Button("Select", ImVec2(buttonWidth, 0))) {
				engineViewerData->projectPath = File::SelectFolderDialog("Select project folder");
			}
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + buttonSpacing);
			if (ImGui::Button("Confirm", ImVec2(buttonWidth, 0)) && !engineViewerData->projectPath.empty()) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::Dummy(ImVec2(0.0f, 10.0f));  // Abstand unten

			ImGui::EndPopup();
		}
	}
}