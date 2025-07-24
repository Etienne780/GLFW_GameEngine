#include "Input.h"

namespace EngineCore {

	Vector2 Input::m_mousePosition;
	std::unordered_map<int, int> Input::keyStates;

	void Input::Init(GLFWwindow* window) {
		glfwSetKeyCallback(window, Input::KeyCallback);
	}

	void Input::Update(GLFWwindow* window) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_mousePosition = Vector2(static_cast<float>(xpos), static_cast<float>(ypos));
	}

	bool Input::IsKeyPressed(int key) {
		auto it = keyStates.find(key);
		return it != keyStates.end() && it->second == GLFW_PRESS;
	}

	bool Input::IsKeyReleased(int key) {
		auto it = keyStates.find(key);
		return it != keyStates.end() && it->second == GLFW_RELEASE;
	}


	bool Input::IsAnyKeyPressed() {
		return getAnyKeyState(GLFW_PRESS);
	}

	bool Input::IsAnyKeyReleased() {
		return getAnyKeyState(GLFW_RELEASE);
	}

	std::vector<int> Input::KeysPressed() {
		return getKeysState(GLFW_PRESS);
	}

	std::vector<int> Input::KeysReleased() {
		return getKeysState(GLFW_RELEASE);
	}

	Vector2 Input::GetMousePosition() {
		return m_mousePosition;
	}

	bool Input::getAnyKeyState(int keyState) {
		for (const auto& [_, state] : keyStates) {
			if (state == keyState)
				return true;
		}
		
		return false;
	}

	std::vector<int> Input::getKeysState(int keyState) {
		std::vector<int> keys;

		for (const auto& [key, state] : keyStates) {
			if (state == keyState)
				keys.push_back(key);
		}

		return keys;
	}

	void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		keyStates[key] = action;
	}
}
