#pragma once
#include "EngineLib\Script.h"

namespace EngineCore {

	namespace Component {
		
		class FreeCameraController : public EngineCore::Script {
		public:
			FreeCameraController(GameObjectID gameObjectID);
			void OnInspectorGUI(IUIRenderer& ui) override;

			COMPONENT_TYPE_DEFINITION(FreeCameraController);

			KeyCode m_keyMoveForward = KeyCode::W;
			KeyCode m_keyMoveBack = KeyCode::S;
			KeyCode m_keyMoveLeft = KeyCode::A;
			KeyCode m_keyMoveRight = KeyCode::D;
			KeyCode m_keyMoveUp = KeyCode::SPACE;
			KeyCode m_keyMoveDown = KeyCode::LEFT_CONTROL;
			KeyCode m_keyMoveSprint = KeyCode::LEFT_SHIFT;
			KeyCode m_keyMoveSlow = KeyCode::LEFT_ALT;

			KeyCode m_keyRotateUp = KeyCode::UP;
			KeyCode m_keyRotateDown = KeyCode::DOWN;
			KeyCode m_keyRotateLeft = KeyCode::LEFT;
			KeyCode m_keyRotateRight = KeyCode::RIGHT;

			bool m_isZoomDisabled = false;
			bool m_isMovementDisabled = false;
			bool m_isRotationDisabled = false;

			bool m_canRotateWithArrow = true;
			bool m_canRotateWithMouse = true;

			float m_arrowSensitivity = 0.8f;
			float m_mouseSensitivity = 0.2f;

			float m_movementSpeed = 20.0f;
			float m_sprintMultiplier = 3.0f;
			float m_slowMultiplier = 0.3f;
			float m_verticalMovementspeedMultiplier = 1.2f;

			float m_fov = 66.0f;
			float m_minFov = 1.0f;
			float m_maxFov = 120.0f;

			bool m_disableIfNotMainCamera = true;
		private:
			bool first = true;
			std::shared_ptr<Component::Camera> m_camera;
			Vector3 cameraRotation;
			Vector3 cameraPosition;
			Vector3 forward;
			Vector3 moveDir;
			float moveDirY = 0;
			Vector2 secondaryLookDir;
			std::shared_ptr<Transform> trans = nullptr;

			/*
			* @brief Called every frame before Application Update.
			*/
			void Update() override;
			/*
			* @brief Called every frame before Application Update and Component update,
			*        even if the Component is disabled.
			*/
			void UpdateAlways() override;

			void CalculateCameraRotation(Vector2 mouseDelta);
			void CameraZoom();
			void CameraMovement();
		};
		
	}

}