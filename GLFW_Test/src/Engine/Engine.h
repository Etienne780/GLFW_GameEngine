#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Application.h"
#include "Time.h"
#include "Input.h"
#include "Shader.h"

#include "..\Core\FormatUtils.h"
#include "..\Core\Log.h"
#include "..\Core\Math.h"
#include "..\Core\ColorUtils.h"
#include "..\Core\stb_image.h"

namespace EngineCore {
	class Engine {
	public:
		std::unique_ptr<Application> app;

		Engine(std::unique_ptr<Application> app, GLFWwindow* window);

		void Start();
		void Update(double currentTimeSec);
		void LateUpdate();
		void Shutdown();

		void WindowResize(int width, int height);

	private:
		int m_frameCount = 0;
		int m_framesPerSecond = 0;// m_framesPerSecond = m_frameCount - m_lastFrameCount;
		float m_fpsCounter = 0;// checks if a second has passed
		int m_lastFrameCount = 0;// is the frame count from the last sec 

		GLFWwindow* m_window = nullptr;
	};
}