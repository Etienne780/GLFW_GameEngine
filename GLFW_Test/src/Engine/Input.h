#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

#include "..\Core\Math\Vector2.h"

namespace EngineCore {

	class Input {
	public:
		static void Init(GLFWwindow* window);
		static void Update(GLFWwindow* window);

		static bool IsKeyPressed(int key);
		static bool IsKeyReleased(int key);

		static bool IsAnyKeyPressed();
		static bool IsAnyKeyReleased();

		static std::vector<int> KeysPressed();
		static std::vector<int> KeysReleased();

		static Vector2 GetMousePosition();

		// interner GLFW Callback
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		static std::unordered_map<int, int> keyStates; // key -> action
		static Vector2 m_mousePosition;

		static bool getAnyKeyState(int keyState);
		static std::vector<int> getKeysState(int keyState);
	};
}