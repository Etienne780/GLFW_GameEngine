#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

#include "InputTypes.h"
#include "InputAction.h"

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
	friend class Engine;
	public:
		// Conversion between integer key codes (GLFW) and internal KeyCode enum

		static KeyCode IntToKeyCode(int key);
		static int KeyCodeToInt(KeyCode key);
		static MouseButton IntToMouseButton(int mb);
		static int MouseButtonToInt(MouseButton mb);

		/**
		* @brief Returns the mouse movement delta since the last frame.
		* The Y axis is inverted for convenience (typical screen coordinates).
		*/
		static Vector2 GetMousePositionDelta();

		/**
		* @brief Returns the vertical scroll direction for this frame.
		* @return Positive for scrolling up, negative for scrolling down, 0 if no scroll.
		*/
		static int GetScrollDir();

		/**
		* @brief Stores the scroll direction into the provided reference if available.
		* @param dir Reference to receive the scroll direction.
		* @return True if scroll input occurred, false otherwise.
		*/
		static bool GetScrollDir(int& dir);

		// True only on the frame when the key is first pressed
		static bool KeyJustPressed(KeyCode key);

		// True only on the frame when the key is released
		static bool KeyJustReleased(KeyCode key);

		// True only on the frame when the mouse button is first pressed
		static bool MouseJustPressed(MouseButton mb);

		// True only on the frame when the mouse button is released
		static bool MouseJustReleased(MouseButton mb);

		// True while the key is held down
		static bool KeyPressed(KeyCode key);

		// True while the mouse button is held down
		static bool MousePressed(MouseButton mb);

		// True while the key is repeating (OS repeat events)
		static bool KeyRepeating(KeyCode key);

		// True if any key was pressed this frame
		static bool AnyKeyJustPressed();

		// True if any key was released this frame
		static bool AnyKeyJustReleased();

		// True if any mouse button was pressed this frame
		static bool AnyMouseJustPressed();

		// True if any mouse button was released this frame
		static bool AnyMouseJustReleased();

		// True if any key is currently held down
		static bool AnyKeyPressed();

		// True if no keys are currently held down
		static bool AnyKeyReleased();

		// True if any mouse button is currently held down
		static bool AnyMousePressed();

		// True if no mouse buttons are currently held down
		static bool AnyMouseReleased();

		// Returns all keys pressed on this frame
		static std::vector<KeyCode> KeysJustPressed();

		// Returns all keys released on this frame
		static std::vector<KeyCode> KeysJustReleased();

		// Returns all mouse buttons pressed on this frame
		static std::vector<MouseButton> MouseButtonsJustPressed();

		// Returns all mouse buttons released on this frame
		static std::vector<MouseButton> MouseButtonsJustReleased();

		// Returns all keys currently held down
		static std::vector<KeyCode> KeysPressed();

		// Returns all keys currently not pressed
		static std::vector<KeyCode> KeysReleased();

		// Returns all mouse buttons currently held down
		static std::vector<MouseButton> MouseButtonsPressed();

		// Returns all mouse buttons currently not pressed
		static std::vector<MouseButton> MouseButtonsReleased();

		// Returns the current mouse position in screen coordinates
		static Vector2 GetMousePosition();

	private:
		Input();
		static void Init(GLFWwindow* window);
		static void LateUpdate();

		static inline std::unordered_map<int, KeyState> m_keyStates;
		static inline std::unordered_map<int, KeyState> m_mouseButtonStates;
		static inline Vector2 m_mousePosition;
		static inline Vector2 m_lastFrameMousePosition;
		static inline Vector2 m_mouseDelta;
		static inline int m_scrollDir = 0;

		// GLFW callbacks

		static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void GLFWMouseCallBack(GLFWwindow* window, double xpos, double ypos);
		static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		static bool getAnyKeyState(const std::unordered_map<int, KeyState>& map, bool keyPressed, bool justPressed);
		static std::vector<KeyCode> getKeyStates(bool keyPressed, bool justPressed);
		static std::vector<MouseButton> getMouseStates(bool mbPressed, bool justPressed);
	};

}