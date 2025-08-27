#include <CoreLib\Math\Vector2.h>
#include <CoreLib\Log.h>

#include "EngineLib\Input.h"

namespace EngineCore {

	Vector2 Input::m_mousePosition;
	Vector2 Input::m_lastFrameMousePosition;
	Vector2 Input::m_mouseDelta;
	bool Input::m_lockDebug = false;

	int Input::m_scrollDir = 0;
	std::unordered_map<int, KeyState> Input::keyStates;

	void Input::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (keyStates.find(key) == keyStates.end()) {
			keyStates[key] = KeyState();
		}

		keyStates[key].setState(action);
	}

	void Input::GLFWMouseCallBack(GLFWwindow* window, double xpos, double ypos) {
		m_lastFrameMousePosition = m_mousePosition;
		m_mousePosition.x = static_cast<float>(xpos);
		m_mousePosition.y =	static_cast<float>(ypos);
	}

	void Input::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		m_scrollDir = static_cast<int>(yoffset);
	}

	Input::Input() {
	}

	void Input::Init(GLFWwindow* window) {
		if (window == nullptr) return;

		glfwSetKeyCallback(window, Input::GLFWKeyCallback);
		glfwSetCursorPosCallback(window, Input::GLFWMouseCallBack);
		glfwSetScrollCallback(window, Input::GLFWScrollCallback);
	}

	void Input::LateUpdate() {
		m_lastFrameMousePosition.x = m_mousePosition.x;
		m_lastFrameMousePosition.y = m_mousePosition.y;
		m_scrollDir = 0;
		// resets the was pressed bools for the just methods
		for (auto& [_, key] : keyStates) {
			key.update();
		}
	}

	void Input::SetLockDebug(bool value) {
		m_lockDebug = value;
	}

	KeyCode Input::IntToKeyCode(int key) {
		return static_cast<KeyCode>(key);
	}

	int Input::KeyCodeToInt(KeyCode key) {
		return static_cast<int>(key);
	}

	Vector2 Input::GetMousePositionDelta() {
#ifndef NDEBUG
		if (m_lockDebug)
			return Vector2 {};
#endif 
		m_mouseDelta.x = m_mousePosition.x - m_lastFrameMousePosition.x;
		m_mouseDelta.y = m_lastFrameMousePosition.y - m_mousePosition.y;

		return m_mouseDelta;
	}

	int Input::GetScrollDir() {
#ifndef NDEBUG
		if (m_lockDebug)
			return 0;
#endif 
		return m_scrollDir;
	}

	bool Input::GetScrollDir(int& dir) {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		dir = m_scrollDir;
		return (dir != 0);
	}

	bool Input::KeyJustPressed(KeyCode key) {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		auto it = keyStates.find(KeyCodeToInt(key));
		return it != keyStates.end() && it->second.justPressed();
	}

	bool Input::KeyJustReleased(KeyCode key) {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		auto it = keyStates.find(KeyCodeToInt(key));
		return it != keyStates.end() && it->second.justReleased();
	}

	bool Input::KeyPressed(KeyCode key) {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		auto it = keyStates.find(KeyCodeToInt(key));
		return it != keyStates.end() && it->second.isPressed;
	}

	bool Input::KeyRepeating(KeyCode key) {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		auto it = keyStates.find(KeyCodeToInt(key));
		return it != keyStates.end() && it->second.isRepeating;
	}

	bool Input::AnyKeyJustPressed() {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		return getAnyKeyState(true, true);
	}

	bool Input::AnyKeyJustReleased() {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		return getAnyKeyState(false, true);
	}

	bool Input::AnyKeyPressed() {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		return getAnyKeyState(true, false);
	}

	bool Input::AnyKeyReleased() {
#ifndef NDEBUG
		if (m_lockDebug)
			return false;
#endif 
		return getAnyKeyState(false, false);
	}

	std::vector<KeyCode> Input::KeysJustPressed() {
#ifndef NDEBUG
		if (m_lockDebug)
			return std::vector<KeyCode> {};
#endif 
		return getKeysState(true, true);
	}

	std::vector<KeyCode> Input::KeysJustReleased() {
#ifndef NDEBUG
		if (m_lockDebug)
			return std::vector<KeyCode> {};
#endif 
		return getKeysState(false, true);
	}

	std::vector<KeyCode> Input::KeysPressed() {
#ifndef NDEBUG
		if (m_lockDebug)
			return std::vector<KeyCode> {};
#endif 
		return getKeysState(true, false);
	}

	std::vector<KeyCode> Input::KeysReleased() {
#ifndef NDEBUG
		if (m_lockDebug)
			return std::vector<KeyCode> {};
#endif 
		return getKeysState(false, false);
	}

	Vector2 Input::GetMousePosition() {
#ifndef NDEBUG
		if (m_lockDebug)
			return Vector2 {};
#endif 
		return m_mousePosition;
	}

	bool Input::getAnyKeyState(bool pressed, bool justCheck) {
		for (const auto& [_, state] : keyStates) {
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

	std::vector<KeyCode> Input::getKeysState(bool pressed, bool just) {
		std::vector<KeyCode> keys;
#ifndef NDEBUG
		if (m_lockDebug)
			return std::vector<KeyCode> {};
#endif 
		for (const auto& [key, state] : keyStates) {
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
			// Keine Änderungen an wasPressed oder wasReleased
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


#ifndef NDEBUG
	Vector2 Input::LockedGetMousePositionDelta() {
		m_mouseDelta.x = m_mousePosition.x - m_lastFrameMousePosition.x;
		m_mouseDelta.y = m_lastFrameMousePosition.y - m_mousePosition.y;

		return m_mouseDelta;
	}


	int Input::LockedGetScrollDir() {
		return m_scrollDir;
	}

	bool Input::LockedGetScrollDir(int& dir) {
		dir = m_scrollDir;
		return (dir != 0);
	}


	bool Input::LockedKeyJustPressed(KeyCode key) {
		auto it = keyStates.find(KeyCodeToInt(key));
		return it != keyStates.end() && it->second.justPressed();
	}

	bool Input::LockedKeyJustReleased(KeyCode key) {
		auto it = keyStates.find(KeyCodeToInt(key));
		return it != keyStates.end() && it->second.justReleased();
	}


	bool Input::LockedKeyPressed(KeyCode key) {
		auto it = keyStates.find(KeyCodeToInt(key));
		return it != keyStates.end() && it->second.isPressed;
	}

	bool Input::LockedKeyRepeating(KeyCode key) {
		auto it = keyStates.find(KeyCodeToInt(key));
		return it != keyStates.end() && it->second.isRepeating;
	}


	bool Input::LockedAnyKeyJustPressed() {
		return getAnyKeyState(true, true);
	}

	bool Input::LockedAnyKeyJustReleased() {
		return getAnyKeyState(false, true);
	}


	bool Input::LockedAnyKeyPressed() {
		return getAnyKeyState(true, false);
	}

	bool Input::LockedAnyKeyReleased() {
		return getAnyKeyState(false, false);
	}


	std::vector<KeyCode> Input::LockedKeysJustPressed() {
		return getKeysState(true, true);
	}

	std::vector<KeyCode> Input::LockedKeysJustReleased() {
		return getKeysState(false, true);
	}


	std::vector<KeyCode> Input::LockedKeysPressed() {
		return getKeysState(true, false);
	}

	std::vector<KeyCode> Input::LockedKeysReleased() {
		return getKeysState(false, false);
	}


	Vector2 Input::LockedGetMousePosition() {
		return m_mousePosition;
	}

#endif
}