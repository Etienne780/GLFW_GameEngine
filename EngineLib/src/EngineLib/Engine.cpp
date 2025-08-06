#include "EngineLib\Engine.h"

namespace EngineCore {
	Engine::Engine(std::unique_ptr<Application> app)
		: app(std::move(app)) {
	}

	int Engine::EngineStart() {
		if (GLFWInit() != ENGINE_SUCCESS) return ENGINE_FAILURE;

		if (app->m_appApplicationHeader) {
			if (GLFWCreateWindow() != ENGINE_SUCCESS) return ENGINE_FAILURE;
			if (GLADInit() != ENGINE_SUCCESS) return ENGINE_FAILURE;
		}

		Input::Init(m_window);
		app->m_window = m_window;

		PrintApplicationHeader();
		stbi_set_flip_vertically_on_load(true);
		app->Start();

		return ENGINE_SUCCESS;
	}

	void Engine::EngineLoop() {
		while (m_isEngineRunning) {
			bool hasWindow = m_window != nullptr;

			Update(glfwGetTime());

			if (hasWindow) {
				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}

			if (hasWindow && glfwWindowShouldClose(m_window)) {
				if (app->m_appApplicationCloseAppOnWindowClose)
					m_isEngineRunning = false;
				GLFWCloseWindow();
			}
		}

		Shutdown();
	}

	void Engine::Update(double currentTimeSec) {
		m_frameCount++;
		app->m_appApplicationFrameCount = m_frameCount;

		Time::UpdateTime(currentTimeSec);
		m_fpsCounter += Time::GetDeltaTime();
		if (m_fpsCounter >= 1) {
			m_framesPerSecond = m_frameCount - m_lastFrameCount;
			m_lastFrameCount = m_frameCount;
			m_fpsCounter -= 1.0;
		}

		// cleares the window each frame
		if (m_window != nullptr) {
			glClearColor(app->m_appOpenGLBackgroundColor.x, 
						 app->m_appOpenGLBackgroundColor.y, 
						 app->m_appOpenGLBackgroundColor.z, 
						 1.0f);
			glClear(((app->m_appOpenGLDepthTesting) ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT));
		}
		app->m_appApplicationFramesPerSecond = m_framesPerSecond;
		app->Update();

		LateUpdate();
	}

	void Engine::LateUpdate() {
		Input::LateUpdate();
	}

	void Engine::Shutdown() {
		app->Shutdown();

		Texture2D::Cleanup();
		glfwTerminate();
	}

	void Engine::OnWindowResize(int width, int height) {
		app->m_appApplicationWindowWidth = width;
		app->m_appApplicationWindowHeight = height;
		app->OnWindowResize(width, height);
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
		if (!app->m_appApplicationHeader) return ENGINE_SUCCESS;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, app->m_appOpenGLVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, app->m_appOpenGLVersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_RESIZABLE, app->m_appApplicationWindowResizable);
		glfwWindowHint(GLFW_DECORATED, app->m_appApplicationWindowDecoration);
		glfwWindowHint(GLFW_FLOATING, app->m_appApplicationWindowFloating);
		glfwWindowHint(GLFW_VISIBLE, app->m_appApplicationWindowVisibility);

		m_window = glfwCreateWindow(app->m_appApplicationWindowWidth, app->m_appApplicationWindowHeight, app->m_appApplicationName.c_str(), NULL, NULL);
		
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
		if (app->m_appApplicationWindowCursorHidden)
			cursorMode = GLFW_CURSOR_HIDDEN;
		if (app->m_appApplicationWindowCursorLock)
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
			engine->app->OnWindowFocusGain();
		}
		else {
			engine->app->OnWindowFocusLost();
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

		glViewport(0, 0, app->m_appApplicationWindowWidth, app->m_appApplicationWindowHeight);
		if (app->m_appOpenGLDepthTesting)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		return ENGINE_SUCCESS;
	}

	void Engine::PrintApplicationHeader() const {
		std::string msg = Log::GetFormattedString("Start application: \"{}\", version: \"{}\"", app->m_appApplicationName, app->m_appApplicationVersion);
		Log::Info(msg);
		if (!app->m_appApplicationHeader) {
			Log::Info("Application is Headerless");
		}

		std::string separator(msg.length() + strlen("[INFO]: "), '=');
		Log::Print(separator);
		Log::Print("");
	}
}