#include "Engine.h"

namespace EngineCore {
	Engine::Engine(std::unique_ptr<Application> app, GLFWwindow* window)
		: app(std::move(app)), m_window(window) {}

	void Engine::Start() {
		Input::Init(m_window);
		app->m_window = m_window;

		Log::Info("Starts application: \"{}\", version: \"{}\"", app->name, app->version);
		app->OnStart();
	}

	void Engine::Update(double currentTimeSec) {
		m_frameCount++;
		app->frameCount = m_frameCount;
		Input::frameCount = m_frameCount;

		Time::UpdateTime(currentTimeSec);
		Input::Update(m_window);

		app->OnUpdate();
	}

	void Engine::LateUpdate() {
		Input::LateUpdate();
	}

	void Engine::Shutdown() {
		app->OnShutdown();
	}

	void Engine::WindowResize(int width, int height) {
		app->windowWidth = width;
		app->windowHeight = height;
		app->OnWindowResize(width, height);
	}
}