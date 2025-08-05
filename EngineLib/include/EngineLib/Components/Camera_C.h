#pragma once
#include "..\Component.h"

namespace EngineCore {
	// class Component;
	
	class Camera : public Component {
	public:
		Camera() = default;
		void SetTest(int i);
		int GetTest();

	private:
		int m_test = 0;

	};

}
