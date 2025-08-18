#pragma once
#include "EngineTypes.h"

struct GLFWwindow;

namespace EngineCore {

	class Debugger {
	friend class Engine;
	public:
		void Init(GLFWwindow* m_window);
		void Update();
		void Shutdown();

	private:
		Debugger();

		GLFWwindow* m_window = nullptr;
	};

}