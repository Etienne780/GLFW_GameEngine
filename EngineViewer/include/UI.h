#pragma once
#include <unordered_map>
#include <typeindex>
#include <string>

#include "CoreLib\Log.h"

struct GLFWwindow;
struct GLFWmonitor;
struct EngineViewerData;

class UI {
public:
	static void Setup(EngineViewerData* data, GLFWwindow* window, GLFWmonitor* primaryMonitor);
    static void StartDraw();
	static void EndDraw();
	static void Shutdown();
};