#ifndef NDEBUG

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <CoreLib/Log.h>
#include <CoreLib\File.h>

#include "EngineLib/Engine.h"
#include "EngineLib/Application.h"
#include "EngineLib/Input.h"
#include "EngineLib/Time.h"
#include "EngineLib/DebuggerWindows.h"
#include "EngineLib/Debugger.h"
#include "EngineLib/IconsFontAwesome5Pro.h"

namespace EngineCore {
	
	Debugger::Debugger() {	
	}

	void Debugger::Init(GLFWwindow* window, std::weak_ptr<Application> app, Engine* engine) {
		m_window = window;
		m_app = app;
		m_engine = engine;
		DebuggerWindows::Init(m_engine);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		io.Fonts->AddFontDefault();
		float baseFontSize = 24.0f;
		float iconFontSize = baseFontSize * 2.0f / 3.0f;

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = iconFontSize;

		std::string fontPath = File::GetExecutableDir() + "/assets/fonts/fa-solid-900.ttf";
		ImFont* smallIconFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), iconFontSize, &icons_config, icons_ranges);
		ImFont* largeIconFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 128.0f, &icons_config, icons_ranges);

		if (!smallIconFont || !largeIconFont) {
			Log::Error("Debugger: could not load FontAwesome fonts!");
		}
		else {
			DebuggerWindows::SetIconFonts(smallIconFont, largeIconFont);
		}

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		Log::Debug("ImGui Debugger initialized");
	}

	void Debugger::Update() {
		SetVariables();
		HandleCursorLock();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		DebuggerWindows::MenuSidebar(m_menuSidebarWidthRatio, m_windowWidth, m_windowHeight);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Debugger::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		Log::Debug("ImGui Debugger shutdown");
	}

	void Debugger::SetVariables() {
		if (auto app = m_app.lock()) {
			m_windowWidth = app->App_Application_Get_Window_Width();
			m_windowHeight = app->App_Application_Get_Window_Height();
		}
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

		if (auto app = m_app.lock()) {
			if (app->App_Application_Get_Window_Cursor_Lock() != m_cursorLock) {
				app->App_Application_Set_Window_Cursor_LockHidden(m_cursorLock);
			}
		}
	}

	bool Debugger::GetCursorLock() {
		return m_cursorLock;
	}
}

#endif