#pragma once
#include "EngineLib\Engine.h"

#if __has_include(<.h>)
#include <optional_header.h>
#endif

class EngineViewer : public Application {
public:
	EngineViewer();

protected:
	void Start() override;
	void Update() override;
	void Shutdown() override;
};