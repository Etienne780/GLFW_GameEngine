#include "Game\MyGameApp.h"

int main() {
	auto engine = std::make_unique<EngineCore::Engine>(std::make_unique<MyGameApp>());

	if (engine->EngineStart()) {
		Log::Error("Engine start faild!");
		return EXIT_FAILURE;
	}

	engine->EngineLoop();
	engine.reset();

	return EXIT_SUCCESS;
}