#pragma once

struct GLFWmonitor;
struct GLFWwindow;
struct EngineViewerData;

namespace UIHelper {
	void StartDraw();
	void EndDraw();

	void SetupImGui(GLFWwindow* window, GLFWmonitor* primaryMonitor);

	void CreateDockingArea();

	void DrawProjectSelectPopUp(EngineViewerData* engineViewerData);
}