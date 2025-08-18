#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <CoreLib/Log.h>

#include "EngineLib/Debugger.h"
#include "EngineLib/Application.h"
#include "EngineLib/Input.h"
#include "EngineLib/Time.h"

namespace EngineCore {
	
	Debugger::Debugger() {	
	}

	void Debugger::Init(GLFWwindow* window, Application* app) {
		m_window = window;
		m_app = app;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		Log::Debug("ImGui Debugger initialized");
	}

	void Debugger::Update() {
		HandleCursorLock();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debugger");
		ImGui::Text("Hello from Debugger!");
		if (ImGui::Button("Klick mich")) {
			Log::Info("Debugger Button clicked");
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Debugger::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		Log::Debug("ImGui Debugger shutdown");
	}

	void Debugger::HandleCursorLock() {
		static bool toggleCursorLock = false;
		if (Input::KeyPressed(GLFW_KEY_LEFT_ALT)) {
			m_cursorLock = false;
		}
		else {
			m_cursorLock = true;
		}

		if (Input::KeyJustPressed(GLFW_KEY_F1)) {
			toggleCursorLock = !toggleCursorLock;
			if (!toggleCursorLock) m_cursorLock = true;
		}

		if (toggleCursorLock) {
			m_cursorLock = false;
		}

		if (m_app->App_Application_Get_Window_Cursor_Lock() != m_cursorLock) {
			m_app->App_Application_Set_Window_Cursor_LockHidden(m_cursorLock);
		}
	}


	bool Debugger::GetCursorLock() {
		return m_cursorLock;
	}
}