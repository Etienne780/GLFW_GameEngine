#include <CoreLib\Math\Vector2.h>
#include <CoreLib\Log.h>

#include "EngineLib\Input.h"

namespace EngineCore {
	Vector2 Input::m_mousePosition;
	Vector2 Input::m_lastFrameMousePosition;
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
		m_lastFrameMousePosition = m_mousePosition;
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
		m_lastFrameMousePosition.x = m_mousePosition.x;
		m_lastFrameMousePosition.y = m_mousePosition.y;
		m_scrollDir = 0;
		// resets the was pressed bools for the just methods
		for (auto& [_, key] : keyStates) {
			key.update();
		}
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


	enum KeyCode {
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,

		NUM_0 = GLFW_KEY_0,
		NUM_1 = GLFW_KEY_1,
		NUM_2 = GLFW_KEY_2,
		NUM_3 = GLFW_KEY_3,
		NUM_4 = GLFW_KEY_4,
		NUM_5 = GLFW_KEY_5,
		NUM_6 = GLFW_KEY_6,
		NUM_7 = GLFW_KEY_7,
		NUM_8 = GLFW_KEY_8,
		NUM_9 = GLFW_KEY_9,

		SPACE = GLFW_KEY_SPACE,
		APOSTROPHE = GLFW_KEY_APOSTROPHE,
		COMMA = GLFW_KEY_COMMA,
		MINUS = GLFW_KEY_MINUS,
		PERIOD = GLFW_KEY_PERIOD,
		SLASH = GLFW_KEY_SLASH,
		SEMICOLON = GLFW_KEY_SEMICOLON,
		EQUAL = GLFW_KEY_EQUAL,
		LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
		BACKSLASH = GLFW_KEY_BACKSLASH,
		RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
		GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
		WORLD_1 = GLFW_KEY_WORLD_1,
		WORLD_2 = GLFW_KEY_WORLD_2,
		ESCAPE = GLFW_KEY_ESCAPE,
		ENTER = GLFW_KEY_ENTER,
		TAB = GLFW_KEY_TAB,
		BACKSPACE = GLFW_KEY_BACKSPACE,
		INSERT = GLFW_KEY_INSERT,
		DELETE = GLFW_KEY_DELETE,
		RIGHT = GLFW_KEY_RIGHT,
		LEFT = GLFW_KEY_LEFT,
		UP = GLFW_KEY_UP,
		DOWN = GLFW_KEY_DOWN,
		PAGE_UP = GLFW_KEY_PAGE_UP,
		PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
		HOME = GLFW_KEY_HOME,
		END = GLFW_KEY_END,
		CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
		SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
		NUM_LOCK = GLFW_KEY_NUM_LOCK,
		PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
		PAUSE = GLFW_KEY_PAUSE,

		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		F13 = GLFW_KEY_F13,
		F14 = GLFW_KEY_F14,
		F15 = GLFW_KEY_F15,
		F16 = GLFW_KEY_F16,
		F17 = GLFW_KEY_F17,
		F18 = GLFW_KEY_F18,
		F19 = GLFW_KEY_F19,
		F20 = GLFW_KEY_F20,
		F21 = GLFW_KEY_F21,
		F22 = GLFW_KEY_F22,
		F23 = GLFW_KEY_F23,
		F24 = GLFW_KEY_F24,
		F25 = GLFW_KEY_F25,

		KP_0 = GLFW_KEY_KP_0,
		KP_1 = GLFW_KEY_KP_1,
		KP_2 = GLFW_KEY_KP_2,
		KP_3 = GLFW_KEY_KP_3,
		KP_4 = GLFW_KEY_KP_4,
		KP_5 = GLFW_KEY_KP_5,
		KP_6 = GLFW_KEY_KP_6,
		KP_7 = GLFW_KEY_KP_7,
		KP_8 = GLFW_KEY_KP_8,
		KP_9 = GLFW_KEY_KP_9,
		KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
		KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
		KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
		KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
		KP_ADD = GLFW_KEY_KP_ADD,
		KP_ENTER = GLFW_KEY_KP_ENTER,
		KP_EQUAL = GLFW_KEY_KP_EQUAL,
		
		LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
		LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
		LEFT_ALT = GLFW_KEY_LEFT_ALT,
		LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
		RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
		RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
		RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
		RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
		MENU = GLFW_KEY_MENU,
	};

	enum MouseCode {
		
	};

}