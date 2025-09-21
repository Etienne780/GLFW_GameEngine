#pragma once
#include <vector>
#include "InputTypes.h"

namespace EngineCore {
	/*
	* an input actions is a concept where multiple keys can be used to check if a certain action should be executet
	*/
	class InputAction {
		InputAction() = default;
		~InputAction() = default;

		InputAction* AddKeyAction(KeyCode key);
		InputAction* AddMouseAction(MouseButton mouseButton);
		InputAction* AddKeyAction(const std::vector<KeyCode>& keys);
		InputAction* AddMouseAction(const std::vector<MouseButton>& mouseButtons);

		InputAction* SetKeyAction(const std::vector<KeyCode>& keys);
		InputAction* SetMouseAction(const std::vector<MouseButton>& mouseButtons);
	private:
		std::vector<KeyCode> m_keyActions;
		std::vector<MouseButton> m_mouseActions;
	};
}

