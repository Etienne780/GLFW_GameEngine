#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

class Vector2;

namespace EngineCore {

	enum class KeyCode;
	enum class MouseButton;

	class KeyState {
	public:
		bool isPressed = false;     // true, if key pressed
		bool isRepeating = false;   // true, if GLFW_REPEAT triggered
		bool wasPressed = false;    // true, only in the frame that the key was pressed
		bool wasReleased = false;   // true, only in the frame that the key was released

		void update();              // should only be called once per frame
		void setState(int state);   // Gets called by the GLFW-Callback

		bool justPressed() const;   // true, if key pressed in this frame
		bool justReleased() const;  // true, if key released in this frame
	};

	class Input {
#ifndef NDEBUG
	friend class Debugger;
#endif 
	friend class Engine;
	public:
		static KeyCode IntToKeyCode(int key);
		static int KeyCodeToInt(KeyCode key);

		/**
		* @brief gets the diff of the current mouse pos and the last frame pose (inverts the y axies for convenietios)
		* @return returns a new Vector2 that is the delta of the mouse position
		*/
		static Vector2 GetMousePositionDelta();

		static int GetScrollDir();
		static bool GetScrollDir(int& dir);

		// is only one frame true
		static bool KeyJustPressed(KeyCode key);
		// is only one frame true
		static bool KeyJustReleased(KeyCode key);

		// is true as long as the key has the right state
		static bool KeyPressed(KeyCode key);
		// is true as long as the key has the right state
		static bool KeyRepeating(KeyCode key);

		// is only one frame true
		static bool AnyKeyJustPressed();
		// is only one frame true
		static bool AnyKeyJustReleased();

		// is true as long as the key has the right state
		static bool AnyKeyPressed();
		// is true as long as the key has the right state
		static bool AnyKeyReleased();

		static std::vector<KeyCode> KeysJustPressed();
		static std::vector<KeyCode> KeysJustReleased();

		static std::vector<KeyCode> KeysPressed();
		static std::vector<KeyCode> KeysReleased();

		static Vector2 GetMousePosition();

	private:
		Input();
		static void Init(GLFWwindow* window);
		static void LateUpdate();
		/*
		* @brief Locks the Input so all input methods return false
		*/
		static void SetLockDebug(bool value);

		static std::unordered_map<int, KeyState> keyStates;
		static Vector2 m_mousePosition;
		static Vector2 m_lastFrameMousePosition;
		static Vector2 m_mouseDelta;
		static int m_scrollDir;
		// Locks the input to false. only works in debug
		static bool m_lockDebug;

		static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void GLFWMouseCallBack(GLFWwindow* window, double xpos, double ypos);
		static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		static bool getAnyKeyState(bool keyPressed, bool justPressed);
		static std::vector<KeyCode> getKeysState(bool keyPressed, bool justPressed);

#ifndef NDEBUG
		/**
		* @brief gets the diff of the current mouse pos and the last frame pose (inverts the y axies for convenietios)
		* (Can still be used even if Input is locked)
		* @return returns a new Vector2 that is the delta of the mouse position
		*/
		static Vector2 LockedGetMousePositionDelta();

		//(Can still be used even if Input is locked)
		static int LockedGetScrollDir();
		//(Can still be used even if Input is locked)
		static bool LockedGetScrollDir(int& dir);

		// is only one frame true. (Can still be used even if Input is locked)
		static bool LockedKeyJustPressed(KeyCode key);
		// is only one frame true. (Can still be used even if Input is locked)
		static bool LockedKeyJustReleased(KeyCode key);

		// is true as long as the key has the right state. (Can still be used even if Input is locked)
		static bool LockedKeyPressed(KeyCode key);
		// is true as long as the key has the right state. (Can still be used even if Input is locked)
		static bool LockedKeyRepeating(KeyCode key);

		// is only one frame true. (Can still be used even if Input is locked)
		static bool LockedAnyKeyJustPressed();
		// is only one frame true. (Can still be used even if Input is locked)
		static bool LockedAnyKeyJustReleased();

		// is true as long as the key has the right state. (Can still be used even if Input is locked)
		static bool LockedAnyKeyPressed();
		// is true as long as the key has the right state. (Can still be used even if Input is locked)
		static bool LockedAnyKeyReleased();

		//(Can still be used even if Input is locked)
		static std::vector<KeyCode> LockedKeysJustPressed();
		//(Can still be used even if Input is locked)
		static std::vector<KeyCode> LockedKeysJustReleased();

		//(Can still be used even if Input is locked)
		static std::vector<KeyCode> LockedKeysPressed();
		//(Can still be used even if Input is locked)
		static std::vector<KeyCode> LockedKeysReleased();

		//(Can still be used even if Input is locked)
		static Vector2 LockedGetMousePosition();
#endif
	};

	enum class EngineCore::KeyCode {
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
		UNDEFINED
	};

	enum class EngineCore::MouseButton {
		N1 = GLFW_MOUSE_BUTTON_1,
		N2 = GLFW_MOUSE_BUTTON_2,
		N3 = GLFW_MOUSE_BUTTON_3,
		N4 = GLFW_MOUSE_BUTTON_4,
		N5 = GLFW_MOUSE_BUTTON_5,
		N6 = GLFW_MOUSE_BUTTON_6,
		N7 = GLFW_MOUSE_BUTTON_7,
		N8 = GLFW_MOUSE_BUTTON_8,
		LAST = GLFW_MOUSE_BUTTON_LAST,
		LEFT = GLFW_MOUSE_BUTTON_LEFT,
		RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
		MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
	};

}