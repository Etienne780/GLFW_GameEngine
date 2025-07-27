#include "Engine.h"

namespace EngineCore {
	Engine::Engine(std::unique_ptr<Application> app, GLFWwindow* window)
		: app(std::move(app)), m_window(window) {}

	void Engine::Start() {
		Input::Init(m_window);
		app->m_window = m_window;

		String msg = Log::GetFormattedString("Starts application: \"{}\", version: \"{}\"", app->app_name, app->app_version);
		Log::Info(msg);
		String seperator;
		int headerLength = strlen("[INFO]: ");
		for (int i = 0; i < msg.length() + headerLength; i++) {
			seperator += '=';
		}
		Log::Print(seperator);
		Log::Print("");

		app->OnStart();
	}

	void Engine::Update(double currentTimeSec) {
		m_frameCount++;
		app->app_frameCount = m_frameCount;
		Input::frameCount = m_frameCount;

		Time::UpdateTime(currentTimeSec);
		Input::Update(m_window);

		m_fpsCounter += Time::GetDeltaTimeSec();
		if (m_fpsCounter >= 1) {
			m_framesPerSecond = m_frameCount - m_lastFrameCount;
			m_lastFrameCount = m_frameCount;
			m_fpsCounter -= 1.0;
		}

		app->app_framesPerSecond = m_framesPerSecond;
		app->OnUpdate();
	}

	void Engine::LateUpdate() {
		Input::LateUpdate();
	}

	void Engine::Shutdown() {
		app->OnShutdown();
	}

	void Engine::WindowResize(int width, int height) {
		app->app_window_Width = width;
		app->app_window_Height = height;
		app->OnWindowResize(width, height);
	}
}