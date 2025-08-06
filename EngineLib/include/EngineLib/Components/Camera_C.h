#pragma once
#include <string>

#include "..\Component.h"

namespace EngineCore {

	class Camera : public Component {
	public:
		Camera();
		void SetTest(int i);
		int GetTest();

	private:
		static const std::string compName;

		int m_test = 0;

	};

}
