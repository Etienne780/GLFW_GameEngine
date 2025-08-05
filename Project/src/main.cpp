#include "Project.h"

int main() {
	using EnginePtr = std::unique_ptr<EngineCore::Engine>;

	EnginePtr engine = std::make_unique<EngineCore::Engine>(std::make_unique<Project>());

	const int result = engine->EngineStart();
	if (result != 0) {
		Log::Error("Engine start failed with code: {}", result);
		return EXIT_FAILURE;
	}

	engine->EngineLoop();
	engine.reset();

	return EXIT_SUCCESS;
}