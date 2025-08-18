#include "EngineViewer.h"

int main() {
	EngineCore::Engine engine(std::make_unique<EngineViewer>();

	const int result = engine.EngineStart();
	if (result != 0) {
		Log::Error("Engine start failed with code: {}", result);
		return EXIT_FAILURE;
	}

	engine.EngineLoop();

	return EXIT_SUCCESS;
}