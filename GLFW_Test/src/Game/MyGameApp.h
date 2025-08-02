#pragma once
#include "..\Engine\Engine.h"

class MyGameApp : public Application {
public:
	MyGameApp();

protected:
	void OnStart() override;
	void OnUpdate() override;
	void OnShutdown() override;

	void OnWindowResize(int newWidth, int newHeight) override;
	void OnWindowFocusLost() override;
	void OnWindowFocusGain() override;
};