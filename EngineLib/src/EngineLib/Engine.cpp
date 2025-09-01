#ifndef NDEBUG
#include "EngineLib\Debugger.h"
#endif
#include "EngineLib\AssetRepository.h"
#include "EngineLib\Engine.h"

namespace EngineCore {

	Engine::Engine(std::shared_ptr<Application> app) {
		m_app = app;
	}

	Engine::~Engine() = default;

	int Engine::EngineStart() {
		if (m_app == nullptr) return ENGINE_FAILURE;
		if (GLFWInit() != ENGINE_SUCCESS) return ENGINE_FAILURE;

		if (m_app->m_appApplicationHeader) {
			if (GLFWCreateWindow() != ENGINE_SUCCESS) return ENGINE_FAILURE;
			if (GLADInit() != ENGINE_SUCCESS) return ENGINE_FAILURE;
		}

		m_app->m_window = m_window;
		Input::Init(m_window);
		GameObjectManager::Init();
		m_gameObjectManager = GameObjectManager::GetInstance();
		Material::m_maxTextureUnits = m_maxTextureUnits;

		LoadBaseAsset();

#ifndef NDEBUG
		m_debugger = std::unique_ptr<Debugger>(new Debugger(this));
		m_debugger->Init();
#endif

		PrintApplicationHeader();
		stbi_set_flip_vertically_on_load(true);
		
		RenderLayer::SetupDefaultLayers();
		m_app->Start();
		RenderLayer::Lock();
		return ENGINE_SUCCESS;
	}

	void Engine::EngineLoop() {
		while (m_isEngineRunning) {
			bool hasWindow = m_window != nullptr;

			if (hasWindow) {
				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}

			Update(glfwGetTime());

			if (hasWindow && glfwWindowShouldClose(m_window)) {
				if (m_app->m_appApplicationCloseAppOnWindowClose)
					m_isEngineRunning = false;
				GLFWCloseWindow();
			}
		}

		Shutdown();
	}

	void Engine::Update(double currentTimeSec) {
		m_frameCount++;

		Time::UpdateTime(currentTimeSec);
		m_fpsCounter += Time::GetDeltaTime();
		if (m_fpsCounter >= 1) {
			m_framesPerSecond = m_frameCount - m_lastFrameCount;
			m_lastFrameCount = m_frameCount;
			m_fpsCounter -= 1.0;
		}

		// cleares the window each frame
		if (m_window != nullptr && !m_app->m_appOpenGLManuallyClearBackground) {
			glClearColor(m_app->m_appOpenGLBackgroundColor.x, 
						 m_app->m_appOpenGLBackgroundColor.y, 
						 m_app->m_appOpenGLBackgroundColor.z, 
						 1.0f);
			glClear(((m_app->m_appOpenGLDepthTesting) ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT));
		}
		Component::Camera::SetWindowDimensions(m_app->m_appApplicationWindowWidth, m_app->m_appApplicationWindowHeight);
		m_app->m_appApplicationFramesPerSecond = m_framesPerSecond;

		m_gameObjectManager->UpdateGameObjects();
		m_app->Update();
		if (m_gameObjectManager->m_mainCamera.lock()) {
			m_gameObjectManager->DrawGameObjects();
		}
		else {
			Log::Warn("Engine: No camera available");
		}

		LateUpdate();
	}

	void Engine::LateUpdate() {
#ifndef NDEBUG
		if (m_app->m_appDebugActive) {
			if (!m_isDebuggerStart) {
				m_debugger->Start();
				m_isDebuggerStart = true;
			}
			m_debugger->Update();
			m_app->m_appDebugIsCursorLockDisabled = m_debugger->GetCursorLock();
			m_app->m_appDebugIsDebugCameraActive = m_debugger->IsDebugCameraActive();
		}
		else {
			if (m_isDebuggerStart) {
				m_debugger->Close();
				m_isDebuggerStart = false;
			}
		}
		Input::SetLockDebug(!m_debugger->GetCursorLock());
#endif 

		Input::LateUpdate();
	}

	void Engine::Shutdown() {
		m_app->Shutdown();
#ifndef NDEBUG
		m_debugger->Shutdown();
#endif 
		GameObjectManager::Shutdown();
		ResourceManager& rm = ResourceManager::GetInstance();
		rm.Cleanup();
		glfwTerminate();
	}

	void Engine::OnWindowResize(int width, int height) {
		m_app->m_appApplicationWindowWidth = width;
		m_app->m_appApplicationWindowHeight = height;
		m_app->OnWindowResize(width, height);
	}

	int Engine::GLFWInit() {
		glfwSetErrorCallback(GLFWErrorCallback);
		if (!glfwInit())
		{
			Log::Error("Engine::GLFW: Initialization of GLFW faild!");
			return ENGINE_FAILURE;
		}
		Log::Info("Engine::GLFW: Initialized GLFW successfully");

		return ENGINE_SUCCESS;
	}

	int Engine::GLFWCreateWindow() {
		if (!m_app->m_appApplicationHeader) return ENGINE_SUCCESS;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_app->m_appOpenGLVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_app->m_appOpenGLVersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_RESIZABLE, m_app->m_appApplicationWindowResizable);
		glfwWindowHint(GLFW_DECORATED, m_app->m_appApplicationWindowDecoration);
		glfwWindowHint(GLFW_FLOATING, m_app->m_appApplicationWindowFloating);
		glfwWindowHint(GLFW_VISIBLE, m_app->m_appApplicationWindowVisibility);

		m_window = glfwCreateWindow(m_app->m_appApplicationWindowWidth, m_app->m_appApplicationWindowHeight, m_app->m_appApplicationName.c_str(), NULL, NULL);
		
		if (!m_window)
		{
			// Window or OpenGL context creation failed
			Log::Error("Engine::GLFW: Window or OpenGL context creation failed!");
			glfwTerminate();
			return ENGINE_FAILURE;
		}
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetFramebufferSizeCallback(m_window, GLFWFramebufferSizeCallback);
		glfwSetWindowFocusCallback(m_window, GLFWFocusCallback);

		glfwSwapInterval(1);

		auto cursorMode = GLFW_CURSOR_NORMAL;
		if (m_app->m_appApplicationWindowCursorHidden)
			cursorMode = GLFW_CURSOR_HIDDEN;
		if (m_app->m_appApplicationWindowCursorLock)
			cursorMode = GLFW_CURSOR_DISABLED;

		glfwSetInputMode(m_window, GLFW_CURSOR, cursorMode);

		return ENGINE_SUCCESS;
	}

	void Engine::GLFWCloseWindow() {
		if (m_window == nullptr) return;

		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}

	void Engine::GLFWErrorCallback(int error, const char* description) {
		Log::Error("Engine::GLFW Error: {}, {}", error, description);
	}

	void Engine::GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);

		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		if (!engine) return;
		engine->OnWindowResize(width, height);
	}

	void Engine::GLFWFocusCallback(GLFWwindow* window, int focused) {
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		if (!engine) return;

		if (focused) {
			engine->m_app->OnWindowFocusGain();
		}
		else {
			engine->m_app->OnWindowFocusLost();
		}
	}

	int Engine::GLADInit() {
		if (m_window == nullptr) {
			Log::Error("GLADInit() called without valid window.");
			return ENGINE_FAILURE;
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Log::Error("Engine::GLAD: Failed to initialize GLAD!");
			glfwTerminate();
			return ENGINE_FAILURE;
		}
		Log::Info("Engine::GLAD: Initialized GLAD successfully");

		glPolygonMode(GL_FRONT, GL_FILL);
		glViewport(0, 0, m_app->m_appApplicationWindowWidth, m_app->m_appApplicationWindowHeight);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureUnits);
		if (m_app->m_appOpenGLDepthTesting)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		return ENGINE_SUCCESS;
	}

	void Engine::PrintApplicationHeader() const {
		std::string msg = Log::GetFormattedString("Start application: \"{}\", version: \"{}\"", m_app->m_appApplicationName, m_app->m_appApplicationVersion);
		Log::Info(msg);
		if (!m_app->m_appApplicationHeader) {
			Log::Info("Application is Headerless");
		}

		std::string separator(msg.length() + strlen("[INFO]: "), '=');
		Log::Print(separator);
		Log::Print("");
	}
}