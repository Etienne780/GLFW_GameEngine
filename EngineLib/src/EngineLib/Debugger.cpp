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

#include "EngineLib/IconsFontAwesome5Pro.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/Application.h"
#include "EngineLib/Input.h"
#include "EngineLib/Time.h"
#include "EngineLib/RenderLayerManager.h"
#include "EngineLib/DebuggerWindows.h"
#include "EngineLib/Engine.h"
#include "EngineLib/Debugger.h"

namespace EngineCore {
	
	Debugger::Debugger(Engine* engine) 
		:m_engine(engine), m_app(engine->m_app), m_window(engine->m_window), m_gameObjectManager(engine->m_gameObjectManager) {
		m_debuggerWindows = std::make_unique<DebuggerWindows>(this);
	}
	
	Debugger::~Debugger() = default;
	
	void Debugger::Init() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
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
		if (File::Exists(fontPath)) {
			ImFont* smallIconFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), iconFontSize, &icons_config, icons_ranges);
			ImFont* largeIconFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 128.0f, &icons_config, icons_ranges);
			
			if (!smallIconFont || !largeIconFont) {
				Log::Error("Debugger: could not load FontAwesome fonts!");
			}
			else {
				m_debuggerWindows->SetIconFonts(smallIconFont, largeIconFont);
			}
		}
		else {
			Log::Warn("Debugger: extra font could not be loaded, file '{}' dosent exits", fontPath);
		}

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		Log::Debug("ImGui Debugger initialized");
	}

	void Debugger::DebugCameraInit() {
		m_debugCameraGO = GameObject::Create("Debug-Camera");
		m_debugCameraGO->SetPersistent(true);
		auto cam = m_debugCameraGO->AddComponent<Component::Camera>();
		cam->AddCameraLayer(RenderLayerManager::GetRenderLayerID("Debug"));
		m_debugCameraGO->AddComponent<Component::FreeCameraController>()->m_isZoomDisabled = true;
		m_debugCameraGO->Disable(true);
	}

	void Debugger::Start() {
		DebugCameraInit();
	}

	void Debugger::Update() {
		static bool lastCursorLock = m_cursorLock;
		SetVariables();
		HandleCursorLock();

		if (m_isDebugCameraActive && lastCursorLock != m_cursorLock) {
			lastCursorLock = m_cursorLock;
			m_debugCameraGO->GetComponent
				<Component::FreeCameraController>()->m_isRotationDisabled = !m_cursorLock;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_debuggerWindows->MenuSidebar(m_menuSidebarWidthRatio, m_windowWidth, m_windowHeight);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Debugger::Close() {
		GameObject::Delete(m_debugCameraGO);
		m_debugCameraGO = nullptr;

		m_cursorLock = true;
		if (auto app = m_app.lock()) {
			app->App_Application_Set_Window_Cursor_LockHidden(true);
		}
	}

	void Debugger::Shutdown() {
		m_hierarchySelectedGO = nullptr;
		m_debuggerWindows->Shutdown();

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
		m_cursorLock = !Input::LockedKeyPressed(KeyCode::LEFT_ALT);

		if (Input::LockedKeyJustPressed(KeyCode::F1)) {
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

	bool Debugger::GetCursorLock() const {
		return m_cursorLock;
	}

	bool Debugger::IsDebugCameraActive() const {
		return m_isDebugCameraActive;
	}

	Application* Debugger::GetApp() {
		return m_app.lock().get();
	}

	GLFWwindow* Debugger::GetWindow() const {
		return m_window;
	}

	GameObjectManager* Debugger::GetGameObjectManager() const {
		return m_gameObjectManager;
	}

	void Debugger::SetMainCamera(std::shared_ptr<Component::Camera> cam) {
		if (!cam)
			return;
		m_isDebugCameraActive = (cam->GetGameObject()->GetID() == m_debugCameraGO->GetID());
		m_debugCameraGO->Disable(!m_isDebugCameraActive);
		GameObject::SetMainCamera(cam);
	}
}

#endif