#include "Engine.h"

namespace EngineCore {
	Engine::Engine(std::unique_ptr<Application> app)
		: app(std::move(app)) {
	}

	int Engine::EngineStart() {
		if(GLFWInit()) return EXIT_FAILURE;

		if (app->m_appApplicationHeader) {
			if (GLFWCreateWindow()) return EXIT_FAILURE;
			if (GLADInit()) return EXIT_FAILURE;
		}

		Input::Init(m_window);
		app->m_window = m_window;

		String msg = Log::GetFormattedString("Starts application: \"{}\", version: \"{}\"", app->m_appApplicationName, app->m_appApplicationVersion);
		Log::Info(msg);
		if (!app->m_appApplicationHeader) {
			Log::Info("Application is Headerless");
		}
		String seperator;
		size_t headerLength = strlen("[INFO]: ");
		for (int i = 0; i < msg.length() + headerLength; i++) {
			seperator += '=';
		}
		Log::Print(seperator);
		Log::Print("");

		stbi_set_flip_vertically_on_load(true);
		app->OnStart();

		return 0;
	}

	void Engine::EngineLoop() {
		while (m_isEngineRunning) {
			bool hasWindow = m_window != nullptr;

			double time = glfwGetTime();// is in sec
			Update(time);

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
		Input::frameCount = m_frameCount;

		Time::UpdateTime(currentTimeSec);
		Input::Update(m_window);

		m_fpsCounter += Time::GetDeltaTime();
		if (m_fpsCounter >= 1) {
			m_framesPerSecond = m_frameCount - m_lastFrameCount;
			m_lastFrameCount = m_frameCount;
			m_fpsCounter -= 1.0;
		}

		app->m_appApplicationFramesPerSecond = m_framesPerSecond;
		app->OnUpdate();

		LateUpdate();
	}

	void Engine::LateUpdate() {
		Input::LateUpdate();
	}

	void Engine::Shutdown() {
		app->OnShutdown();

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
			return EXIT_FAILURE;
		}
		Log::Info("Engine::GLFW: Initialized GLFW successfully");

		return 0;
	}

	int Engine::GLFWCreateWindow() {
		if (!app->m_appApplicationHeader) return 0;

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
			return EXIT_FAILURE;
		}
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetFramebufferSizeCallback(m_window, GLFWFramebufferSizeCallback);

		glfwSwapInterval(1);

		return 0;
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
		if (engine)
			engine->OnWindowResize(width, height);
	}

	int Engine::GLADInit() {
		if (m_window == nullptr) return 0;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Log::Error("Engine::GLAD: Failed to initialize GLAD!");
			glfwTerminate();
			return EXIT_FAILURE;
		}

		glViewport(0, 0, app->m_appApplicationWindowWidth, app->m_appApplicationWindowHeight);
		return 0;
	}
}