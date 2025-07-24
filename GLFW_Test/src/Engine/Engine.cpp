#include "Engine.h"

namespace EngineCore {
	Engine::Engine(std::unique_ptr<Application> app)
		: app(std::move(app)) {}

	void Engine::Start(GLFWwindow* window) {
		m_window = window;
		m_input.Init(m_window);

		Log::Info("Starts application: \"{}\", version: \"{}\"", app->name, app->version);
		app->OnStart();
	}

	void Engine::Update(double currentTimeSec) {
		m_time.UpdateTime(currentTimeSec);
		m_input.Update(m_window);

		app->OnUpdate();
	}

	void Engine::Shutdown() {
		app->OnShutdown();
	}
}