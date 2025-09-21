#pragma once
#include <vector>
#include "InputTypes.h"
#include "InputLayerManager.h"

namespace EngineCore {

	/*
	* InputAction represents a logical action that can be triggered by one or more keys
	* or mouse buttons.
	*
	* Example use case:
	* - Define an "Jump" action that can be triggered by the space bar OR the right mouse button.
	* - Check InputAction in your game loop instead of querying multiple individual inputs.
	*
	* Supports method chaining:
	*   action->AddKeyAction(KeyCode::W)
	*         ->AddMouseAction(MouseButton::Left)
	*         ->SetKeyAction({KeyCode::Space, KeyCode::Enter});
	*/
	class InputAction {
	public:
		InputAction() = default;
		~InputAction() = default;

		/**
		* @brief Add a single keyboard key as a trigger for this action.
		* @param key KeyCode to add
		* @return Pointer to this InputAction for method chaining
		*/
		InputAction* AddKeyAction(KeyCode key);

		/**
		* @brief Add a single mouse button as a trigger for this action.
		* @param mouseButton MouseButton to add
		* @return Pointer to this InputAction for method chaining
		*/
		InputAction* AddMouseAction(MouseButton mouseButton);

		/**
		* @brief Add multiple keyboard keys as triggers for this action.
		* @param keys A vector of KeyCodes
		* @return Pointer to this InputAction for method chaining
		*/
		InputAction* AddKeyAction(const std::vector<KeyCode>& keys);

		/**
		* @brief Add multiple mouse buttons as triggers for this action.
		* @param mouseButtons A vector of MouseButtons
		* @return Pointer to this InputAction for method chaining
		*/
		InputAction* AddMouseAction(const std::vector<MouseButton>& mouseButtons);


		InputLayerID GetInputLayer() const;

		/**
		* @brief Get all keyboard keys that can trigger this action.
		* @return A const reference to the vector of KeyCodes
		*/
		const std::vector<KeyCode>& GetKeyActions() const;

		/**
		* @brief Get all mouse buttons that can trigger this action.
		* @return A const reference to the vector of MouseButtons
		*/
		const std::vector<MouseButton>& GetMouseActions() const;

		/**
		* @brief Replace all existing key triggers with a new set.
		* @param keys A vector of KeyCodes
		* @return Pointer to this InputAction for method chaining
		*/
		InputAction* SetKeyAction(const std::vector<KeyCode>& keys);

		/**
		* @brief Replace all existing mouse button triggers with a new set.
		* @param mouseButtons A vector of MouseButtons
		* @return Pointer to this InputAction for method chaining
		*/
		InputAction* SetMouseAction(const std::vector<MouseButton>& mouseButtons);

		InputAction* SetInputLayer(InputLayerID layer);

	private:
		std::vector<KeyCode> m_keyActions;
		std::vector<MouseButton> m_mouseActions;
		InputLayerID m_layerID = InputLayerID(ENGINE_INVALID_ID);

		/**
		* @brief Helper function that adds an element to a vector only if it is not already present.
		* @tparam T Type of element (KeyCode or MouseButton)
		* @param vec Vector to insert into
		* @param value Value to add
		*/
		template <typename T>
		static void AddUnique(std::vector<T>& vec, T value) {
			if (std::find(vec.begin(), vec.end(), value) == vec.end()) {
				vec.push_back(value);
			}
		}
	};

}