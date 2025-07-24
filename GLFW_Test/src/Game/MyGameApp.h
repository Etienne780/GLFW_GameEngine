// MyGameApp.h
#pragma once
#include "..\Engine\Application.h"

class MyGameApp : public Application {
public:
	MyGameApp();

protected:
	void OnStart() override;
	void OnUpdate() override;
	void OnShutdown() override;
};