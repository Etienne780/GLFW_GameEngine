#include "EngineLib\Components\Camera_C.h"

namespace EngineCore {

	namespace Component {

		const std::string Camera::compName = "Camera";

		Camera::Camera(GameObject* gameObject) :
			ComponentBase(compName, gameObject) {
		}

		void Camera::SetTest(int i) {
			m_test = i;
		}

		int Camera::GetTest() {
			return m_test;
		}

	}

}
