#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <CoreLib/Log.h>
#include <CoreLib/Math.h>
#include "CoreLib/File.h"
#include <CoreLib/FormatUtils.h>
#include <CoreLib/ColorUtils.h>
#include <CoreLib/TimeUtils.h>
#include <CoreLib/ConversionUtils.h>
#include <CoreLib/stb_image.h>
#include <CoreLib/Algorithm.h>
#include <CoreLib/MatchSet.h>

#include "EngineLib/EngineTypes.h"
#include "EngineLib/Application.h"
#include "EngineLib/Time.h"
#include "EngineLib/Input.h"
#include "EngineLib/Shader.h"
#include "EngineLib/Texture2D.h"
#include "EngineLib/Material.h"
#include "EngineLib/ResourceManager.h"
#include "EngineLib/AssetRepository.h"
#include "EngineLib/GameObjectManager.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/AllComponents.h"
#include "EngineLib/RenderLayerManager.h"
#include "EngineLib/FontManager.h"
#include "EngineLib/UI.h"
#include "EngineLib/Profiler.h"

namespace EngineCore {

	class Engine {
#ifndef NDEBUG
		friend class Debugger;
#endif
	public:
		std::shared_ptr<Application> m_app;

		Engine(std::shared_ptr<Application> app);
		~Engine();

		int EngineStart();
		void EngineLoop();

	private:
		bool m_isEngineRunning = true;

		int m_frameCount = 0;
		int m_framesPerSecond = 0;// m_framesPerSecond = m_frameCount - m_lastFrameCount;
		double m_fpsCounter = 0;// checks if a second has passed
		int m_lastFrameCount = 0;// is the frame count from the last sec 

		GLint m_maxTextureUnits = 0;

#ifndef NDEBUG
		std::unique_ptr<Debugger> m_debugger = nullptr;
		bool m_isDebuggerStart = false;
#endif 
		GLFWwindow* m_window = nullptr;
		GameObjectManager* m_gameObjectManager = nullptr;

		void Update(double currentTimeSec);
		void LateUpdate();
		void Shutdown();

		void OnWindowResize(int width, int height);

		int GLFWInit();
		int GLFWCreateWindow();
		void GLFWCloseWindow();
		static void GLFWErrorCallback(int error, const char* description);
		static void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
		static void GLFWFocusCallback(GLFWwindow* window, int focused);

		int GLADInit();

		void PrintApplicationHeader() const;
	};

}

/*
features:
Input
- Get just pressed/released keys
- Get pressed/released keys
- Get mouse pos
- Get delta mouse pos
- Can be locked by the engine 

Time
- Get time
- Get delta time
- Get frame count

Application
- application state

GameObjects
- Can have a specific RenderLayer
- Can have components
- Can have childs
- Can be Persistant

Components
- Transform
- Camera
- - Can see diffrent kind of Render-Layers
- MeshRenderer
- Script
- - FreeCameraScript

ResourceManager
- Texture2D
- Mesh
- Shader
- Material

Render-Layers
- Add custom Render-Layers

Renderer
- Renders batches
- Renders what the camera can see

Debugger with ImGui
- General stats
- Hierarchy
- Inspector
- Camera
- Console

Simple Profiler
- section start/end
- get the data
*/