#include <CoreLib\Math\Vector2.h>
#include <CoreLib\Log.h>

#include "EngineLib\Input.h"

namespace EngineCore {

	void Input::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (m_keyStates.find(key) == m_keyStates.end()) {
			m_keyStates[key] = KeyState();
		}

		m_keyStates[key].setState(action);
	}

	void Input::GLFWMouseCallBack(GLFWwindow* window, double xpos, double ypos) {
		m_lastFrameMousePosition = m_mousePosition;
		m_mousePosition.x = static_cast<float>(xpos);
		m_mousePosition.y =	static_cast<float>(ypos);
	}

	void Input::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		m_scrollDir = static_cast<int>(yoffset);
	}

	void Input::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		if (m_mouseButtonStates.find(button) == m_mouseButtonStates.end()) {
			m_mouseButtonStates[button] = KeyState();
		}

		m_mouseButtonStates[button].setState(action);
	}

	Input::Input() {
	}

	void Input::Init(GLFWwindow* window) {
		if (window == nullptr) return;

		glfwSetKeyCallback(window, GLFWKeyCallback);
		glfwSetCursorPosCallback(window, GLFWMouseCallBack);
		glfwSetScrollCallback(window, GLFWScrollCallback);
		glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);
	}

	void Input::LateUpdate() {
		m_lastFrameMousePosition.x = m_mousePosition.x;
		m_lastFrameMousePosition.y = m_mousePosition.y;
		m_scrollDir = 0;
		// resets the was pressed bools for the just methods
		for (auto& [_, key] : m_keyStates) {
			key.update();
		}

		for (auto& [_, mb] : m_mouseButtonStates) {
			mb.update();
		}
	}

	KeyCode Input::IntToKeyCode(int key) {
		return static_cast<KeyCode>(key);
	}

	int Input::KeyCodeToInt(KeyCode key) {
		return static_cast<int>(key);
	}

	MouseButton Input::IntToMouseButton(int mb) {
		return static_cast<MouseButton>(mb);
	}

	int Input::MouseButtonToInt(MouseButton mb) {
		return static_cast<int>(mb);
	}

	Vector2 Input::GetMousePositionDelta() {
		m_mouseDelta.x = m_mousePosition.x - m_lastFrameMousePosition.x;
		m_mouseDelta.y = m_lastFrameMousePosition.y - m_mousePosition.y;

		return m_mouseDelta;
	}

	int Input::GetScrollDir() {
		return m_scrollDir;
	}

	bool Input::GetScrollDir(int& dir) {
		dir = m_scrollDir;
		return (dir != 0);
	}

	bool Input::ActionJustPressed(const InputAction& action) {
		return std::any_of(action.GetKeyActions().begin(), action.GetKeyActions().end(),
				[](KeyCode key) { return KeyJustPressed(key); }) ||
			std::any_of(action.GetMouseActions().begin(), action.GetMouseActions().end(),
				[](MouseButton mb) { return MouseJustPressed(mb); });
	}

	bool Input::ActionJustReleased(const InputAction& action) {
		return std::any_of(action.GetKeyActions().begin(), action.GetKeyActions().end(),
				[](KeyCode key) { return KeyJustReleased(key); }) ||
			std::any_of(action.GetMouseActions().begin(), action.GetMouseActions().end(),
				[](MouseButton mb) { return MouseJustReleased(mb); });
	}

	bool Input::KeyJustPressed(KeyCode key) {
		auto it = m_keyStates.find(KeyCodeToInt(key));
		return it != m_keyStates.end() && it->second.justPressed();
	}

	bool Input::KeyJustReleased(KeyCode key) {
		auto it = m_keyStates.find(KeyCodeToInt(key));
		return it != m_keyStates.end() && it->second.justReleased();
	}

	bool Input::MouseJustPressed(MouseButton mb) {
		auto it = m_mouseButtonStates.find(MouseButtonToInt(mb));
		return it != m_mouseButtonStates.end() && it->second.justPressed();
	}

	bool Input::MouseJustReleased(MouseButton mb) {
		auto it = m_mouseButtonStates.find(MouseButtonToInt(mb));
		return it != m_mouseButtonStates.end() && it->second.justReleased();
	}

	bool Input::ActionPressed(const InputAction& action) {
		return std::any_of(action.GetKeyActions().begin(), action.GetKeyActions().end(),
				[](KeyCode key) { return KeyPressed(key); }) ||
			std::any_of(action.GetMouseActions().begin(), action.GetMouseActions().end(),
				[](MouseButton mb) { return MousePressed(mb); });
	}

	bool Input::KeyPressed(KeyCode key) {
		auto it = m_keyStates.find(KeyCodeToInt(key));
		return it != m_keyStates.end() && it->second.isPressed;
	}

	bool Input::MousePressed(MouseButton mb) {
		auto it = m_mouseButtonStates.find(MouseButtonToInt(mb));
		return it != m_mouseButtonStates.end() && it->second.isPressed;
	}

	bool Input::KeyRepeating(KeyCode key) {
		auto it = m_keyStates.find(KeyCodeToInt(key));
		return it != m_keyStates.end() && it->second.isRepeating;
	}

	bool Input::AnyKeyJustPressed() {
		return getAnyKeyState(m_keyStates, true, true);
	}

	bool Input::AnyKeyJustReleased() {
		return getAnyKeyState(m_keyStates, false, true);
	}

	bool Input::AnyMouseJustPressed() {
		return getAnyKeyState(m_mouseButtonStates, true, true);
	}

	bool Input::AnyMouseJustReleased() {
		return getAnyKeyState(m_mouseButtonStates, false, true);
	}

	bool Input::AnyKeyPressed() {
		return getAnyKeyState(m_keyStates, true, false);
	}

	bool Input::AnyKeyReleased() {
		return getAnyKeyState(m_keyStates, false, false);
	}

	bool Input::AnyMousePressed() {
		return getAnyKeyState(m_mouseButtonStates, true, false);
	}

	bool Input::AnyMouseReleased() {
		return getAnyKeyState(m_mouseButtonStates, false, false);
	}

	std::vector<KeyCode> Input::KeysJustPressed() {
		return getKeyStates(true, true);
	}

	std::vector<KeyCode> Input::KeysJustReleased() {
		return getKeyStates(false, true);
	}

	std::vector<MouseButton> Input::MouseButtonsJustPressed() {
		return getMouseStates(true, true);
	}
		
	std::vector<MouseButton> Input::MouseButtonsJustReleased() {
		return getMouseStates(false, true);
	}

	std::vector<KeyCode> Input::KeysPressed() {
		return getKeyStates(true, false);
	}

	std::vector<KeyCode> Input::KeysReleased() {
		return getKeyStates(false, false);
	}

	std::vector<MouseButton> Input::MouseButtonsPressed() {
		return getMouseStates(true, false);
	}

	std::vector<MouseButton> Input::MouseButtonsReleased() {
		return getMouseStates(false, false);
	}

	Vector2 Input::GetMousePosition() {
		return m_mousePosition;
	}

	bool Input::getAnyKeyState(const std::unordered_map<int, KeyState>& map, bool pressed, bool justCheck) {
		for (const auto& [_, state] : map) {
			if (justCheck) {
				if (pressed && state.justPressed())
					return true;
				if (!pressed && state.justReleased())
					return true;
			}
			else {
				if (pressed && state.isPressed)
					return true;
				if (!pressed && !state.isPressed)
					return true;
			}
		}
		return false;
	}

	std::vector<KeyCode> Input::getKeyStates(bool pressed, bool just) {
		std::vector<KeyCode> keys;

		for (const auto& [key, state] : m_keyStates) {
			if (just) {
				if (pressed && state.justPressed())
					keys.push_back(IntToKeyCode(key));
				else if (!pressed && state.justReleased())
					keys.push_back(IntToKeyCode(key));
			}
			else {
				if (pressed && state.isPressed)
					keys.push_back(IntToKeyCode(key));
				else if (!pressed && !state.isPressed)
					keys.push_back(IntToKeyCode(key));
			}
		}

		return keys;
	}

	std::vector<MouseButton> Input::getMouseStates(bool pressed, bool just) {
		std::vector<MouseButton> mbs;

		for (const auto& [key, state] : m_keyStates) {
			if (just) {
				if (pressed && state.justPressed())
					mbs.push_back(IntToMouseButton(key));
				else if (!pressed && state.justReleased())
					mbs.push_back(IntToMouseButton(key));
			}
			else {
				if (pressed && state.isPressed)
					mbs.push_back(IntToMouseButton(key));
				else if (!pressed && !state.isPressed)
					mbs.push_back(IntToMouseButton(key));
			}
		}

		return mbs;
	}

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