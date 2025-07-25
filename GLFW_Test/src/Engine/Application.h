#pragma once
#include <String>

typedef std::string String;

class Application {
public:
	String name;
	String version;
	int frameCount = 0;

	Application(String name, String version);
	virtual ~Application() = default;

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnShutdown();
};