#include "EngineLib/Input/InputTypes.h"

namespace EngineCore {

	void KeyState::update() {
		wasPressed = false;
		wasReleased = false;
	}

	void KeyState::setState(int state) {
		switch (state) {
		case GLFW_PRESS:
			if (!isPressed) {
				wasPressed = true;
			}
			isPressed = true;
			isRepeating = false;
			wasReleased = false;
			break;

		case GLFW_REPEAT:
			isPressed = true;
			isRepeating = true;
			break;

		case GLFW_RELEASE:
			if (isPressed) {
				wasReleased = true;
			}
			isPressed = false;
			isRepeating = false;
			wasPressed = false;
			break;
		}
	}

	bool KeyState::justPressed() const {
		return wasPressed;
	}

	bool KeyState::justReleased() const {
		return wasReleased;
	}

}