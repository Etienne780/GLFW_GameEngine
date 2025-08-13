#pragma once
#include <string>

#include "..\ComponentBase.h"

namespace EngineCore {

	namespace Component {

		class Camera : public EngineCore::ComponentBase {
		public:
			Camera(GameObject* gameObject);
			void SetTest(int i);
			int GetTest();

		private:
			static const std::string compName;

			int m_test = 0;

		};

	}

}
