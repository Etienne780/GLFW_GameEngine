#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "EngineLib\Time.h"

namespace EngineCore {
	
	Time::Time() {}

	void Time::UpdateTime(double currentTimeSec) {
		CalculateDeltaTime(currentTimeSec);
		m_lastFrameTimeSec = currentTimeSec;
		m_frameCount++;
	}

	void Time::CalculateDeltaTime(double currentTimeSec) {
		m_deltaTimeSec = currentTimeSec - m_lastFrameTimeSec;
	}

	int Time::GetFrameCount() {
		return m_frameCount;
	}

	float Time::GetTime() {
		return static_cast<float>(glfwGetTime());
	}

	float Time::GetDeltaTime() {
		return static_cast<float>(m_deltaTimeSec);
	}

	double Time::GetTimeDouble() {
		return glfwGetTime();
	}

	double Time::GetDeltaTimeDouble() {
		return m_deltaTimeSec;
	}
}