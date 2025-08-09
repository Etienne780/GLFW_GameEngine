#pragma once
#include <string>

struct GLFWwindow;
struct GLFWmonitor;

class EngineWindow {
public:
	void Draw();

	std::string GetName() const;

	static void Init(GLFWwindow* window, GLFWmonitor* primaryMonitor);

protected:
	EngineWindow(const char* name);

	const char* m_name;
	const float m_sizeX = 250, m_sizeY = 200;

	static ImGuiIO* m_io;

	virtual void DrawInter();
};