#include <CoreLib\Math\Vector2.h>
#include <CoreLib\Log.h>

#include "EngineLib\Input.h"

namespace EngineCore {
	Vector2 Input::m_mousePosition;
	Vector2 Input::m_mouseDelta;

	int Input::m_scrollDir = 0;
	std::unordered_map<int, Key> Input::keyStates;

	void Input::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (keyStates.find(key) == keyStates.end()) {
			keyStates[key] = Key();
		}

		keyStates[key].setState(action);
	}

	void Input::GLFWMouseCallBack(GLFWwindow* window, double xpos, double ypos) {
		m_mouseDelta.x = xpos - m_mousePosition.x;
		m_mouseDelta.y = m_mousePosition.y - ypos;

		m_mousePosition.x = static_cast<float>(xpos);
		m_mousePosition.y =	static_cast<float>(ypos);
	}

	void Input::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		m_scrollDir = static_cast<int>(yoffset);
	}

	Input::Input() {}

	void Input::Init(GLFWwindow* window) {
		if (window == nullptr) return;

		glfwSetKeyCallback(window, Input::GLFWKeyCallback);
		glfwSetCursorPosCallback(window, Input::GLFWMouseCallBack);
		glfwSetScrollCallback(window, Input::GLFWScrollCallback);
	}

	void Input::LateUpdate() {
		m_scrollDir = 0;
		// resets the was pressed bools for the just methods
		for (auto& [_, key] : keyStates) {
			key.update();
		}
	}

	Vector2 Input::GetMousePositionDelta() {
		return m_mouseDelta;
	}

	int Input::GetScrollDir() {
		return m_scrollDir;
	}

	bool Input::GetScrollDir(int& dir) {
		dir = m_scrollDir;
		return (dir != 0);
	}

	bool Input::KeyJustPressed(int key) {
		auto it = keyStates.find(key);
		return it != keyStates.end() && it->second.justPressed();
	}

	bool Input::KeyJustReleased(int key) {
		auto it = keyStates.find(key);
		return it != keyStates.end() && it->second.justReleased();
	}

	bool Input::KeyPressed(int key) {
		auto it = keyStates.find(key);
		return it != keyStates.end() && it->second.isPressed;
	}

	bool Input::KeyRepeating(int key) {
		auto it = keyStates.find(key);
		return it != keyStates.end() && it->second.isRepeating;
	}

	bool Input::AnyKeyJustPressed() {
		return getAnyKeyState(true, true);
	}

	bool Input::AnyKeyJustReleased() {
		return getAnyKeyState(false, true);
	}

	bool Input::AnyKeyPressed() {
		return getAnyKeyState(true, false);
	}

	bool Input::AnyKeyReleased() {
		return getAnyKeyState(false, false);
	}

	std::vector<int> Input::KeysJustPressed() {
		return getKeysState(true, true);
	}

	std::vector<int> Input::KeysJustReleased() {
		return getKeysState(false, true);
	}

	std::vector<int> Input::KeysPressed() {
		return getKeysState(true, false);
	}

	std::vector<int> Input::KeysReleased() {
		return getKeysState(false, false);
	}

	Vector2 Input::GetMousePosition() {
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

	std::vector<int> Input::getKeysState(bool pressed, bool just) {
		std::vector<int> keys;

		for (const auto& [key, state] : keyStates) {
			if (just) {
				if (pressed && state.justPressed())
					keys.push_back(key);
				else if (!pressed && state.justReleased())
					keys.push_back(key);
			}
			else {
				if (pressed && state.isPressed)
					keys.push_back(key);
				else if (!pressed && !state.isPressed)
					keys.push_back(key);
			}
		}

		return keys;
	}

	void Key::update() {
		wasPressed = false;
		wasReleased = false;
	}

	void Key::setState(int state) {
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

	bool Key::justPressed() const {
		return wasPressed;
	}

	bool Key::justReleased() const {
		return wasReleased;
	}

}