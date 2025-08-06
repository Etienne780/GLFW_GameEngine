#include "EngineLib\Components\Camera_C.h"

namespace EngineCore {

	const std::string Camera::compName = "Camera";

	Camera::Camera() : 
		Component(compName) {
	}

	void Camera::SetTest(int i) {
		m_test = i;
	}

	int Camera::GetTest() {
		return m_test;
	}

}
