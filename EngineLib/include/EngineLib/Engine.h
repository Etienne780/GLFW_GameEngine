#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <CoreLib\Log.h>
#include <CoreLib\Math.h>
#include "CoreLib\File.h"
#include <CoreLib\FormatUtils.h>
#include <CoreLib\ColorUtils.h>
#include <CoreLib\TimeUtils.h>
#include <CoreLib\ConversionUtils.h>
#include <CoreLib\stb_image.h>

#include "EngineLib\Application.h"
#include "EngineLib\Time.h"
#include "EngineLib\Input.h"
#include "EngineLib\Shader.h"
#include "EngineLib\Texture2D.h"
#include "EngineLib\GameObjectManager.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\AllComponents.h"

namespace EngineCore {

	constexpr int ENGINE_SUCCESS = 0;
	constexpr int ENGINE_FAILURE = 1;

	class Engine {
	public:
		std::unique_ptr<Application> app;

		Engine(std::unique_ptr<Application> app);

		int EngineStart();
		void EngineLoop();

	private:
		bool m_isEngineRunning = true;

		int m_frameCount = 0;
		int m_framesPerSecond = 0;// m_framesPerSecond = m_frameCount - m_lastFrameCount;
		double m_fpsCounter = 0;// checks if a second has passed
		int m_lastFrameCount = 0;// is the frame count from the last sec 

		GLFWwindow* m_window = nullptr;

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