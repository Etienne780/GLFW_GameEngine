#pragma once
#include "EngineLib\Engine.h"

class Project : public Application {
public:
	Project();

protected:
	void Start() override;
	void Update() override;
	void Shutdown() override;

	void OnWindowResize(int newWidth, int newHeight) override;
	void OnWindowFocusLost() override;
	void OnWindowFocusGain() override;
};