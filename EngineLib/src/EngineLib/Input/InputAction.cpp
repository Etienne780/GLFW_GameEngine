#include "EngineLib/Input/InputAction.h"

namespace EngineCore {
	
	InputAction* InputAction::AddKeyAction(KeyCode key) {
		AddUnique<KeyCode>(m_keyActions, key);
		return this;
	}

	InputAction* InputAction::AddMouseAction(MouseButton mouseButton) {
		AddUnique<MouseButton>(m_mouseActions, mouseButton);
		return this;
	}

	InputAction* InputAction::AddKeyAction(const std::vector<KeyCode>& keys) {
		for (auto& k : keys)
			AddKeyAction(k);
		return this;
	}

	InputAction* InputAction::AddMouseAction(const std::vector<MouseButton>& mouseButtons) {
		for (auto& mb : mouseButtons)
			AddMouseAction(mb);
		return this;
	}

	const std::vector<KeyCode>& InputAction::GetKeyActions() const {
		return m_keyActions;
	}

	const std::vector<MouseButton>& InputAction::GetMouseActions() const {
		return m_mouseActions;
	}

	InputAction* InputAction::SetKeyAction(const std::vector<KeyCode>& keys) {
		m_keyActions = keys;
		return this;
	}

	InputAction* InputAction::SetMouseAction(const std::vector<MouseButton>& mouseButtons) {
		m_mouseActions = mouseButtons;
		return this;
	}

}