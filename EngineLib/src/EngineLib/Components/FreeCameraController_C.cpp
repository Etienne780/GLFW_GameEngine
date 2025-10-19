#include "EngineLib\Components\FreeCameraController_C.h"
#include "EngineLib\Application.h"

namespace EngineCore {

	namespace Component {

		FreeCameraController::FreeCameraController(GameObjectID gameObjectID)
			: Script("FreeCameraController", gameObjectID) {
			auto go = GetGameObject();
			m_camera = go->GetComponent<Component::Camera>();
			trans = go->GetTransform();
		}

		void FreeCameraController::OnInspectorGUI(IComponentUIRenderer& ui) {
			ui.DrawSeparatorText("Camera Control");
			ui.DrawCheckbox("Disable if not Main Camera", &m_disableIfNotMainCamera);

			ui.DrawSeparatorText("Zoom");
			ui.DrawCheckbox("Disable Zoom", &m_isZoomDisabled);
			ui.DrawDragFloat("FOV", &m_fov, 0.1f, m_minFov, m_maxFov);
			ui.DrawInputFloat("Min FOV", &m_minFov);
			ui.DrawInputFloat("Max FOV", &m_maxFov);

			ui.DrawSeparatorText("Movement");
			ui.DrawCheckbox("Disable Movement", &m_isMovementDisabled);
			ui.DrawDragFloat("Movement Speed", &m_movementSpeed, 0.1f);
			ui.DrawDragFloat("Sprint Multiplier", &m_sprintMultiplier, 0.01f);
			ui.DrawDragFloat("Slow Multiplier", &m_slowMultiplier, 0.01f);
			ui.DrawDragFloat("Vertical Movement Speed Multiplier", &m_verticalMovementspeedMultiplier, 0.01f);

			ui.DrawSeparatorText("Rotation");
			ui.DrawCheckbox("Disable Rotation", &m_isRotationDisabled);
			ui.DrawCheckbox("Rotate with Arrow Keys", &m_canRotateWithArrow);
			ui.DrawCheckbox("Rotate with Mouse", &m_canRotateWithMouse);
			ui.DrawDragFloat("Arrow Sensitivity", &m_arrowSensitivity, 0.01f);
			ui.DrawDragFloat("Mouse Sensitivity", &m_mouseSensitivity, 0.01f);
		}


		void FreeCameraController::UpdateAlways() {
			// disalbes the camera if it is not the main camera
			if (m_disableIfNotMainCamera) {
				auto mainCamera = GameObject::GetMainCamera();
				if (mainCamera) {
					bool isCamControllerMainCam = (mainCamera->GetGameObject()->GetID() == GetGameObject()->GetID());
					Disable(!isCamControllerMainCam);
				}
			}
		}

		void FreeCameraController::Update() {
			if (!m_isZoomDisabled) {
				CameraZoom();
			}

			if (!m_isMovementDisabled) {
				moveDir.x = 0;
				moveDir.z = 0;
				moveDirY = 0;

				if (Input::KeyPressed(m_keyMoveForward)) moveDir.z += 1;
				if (Input::KeyPressed(m_keyMoveBack)) moveDir.z -= 1;
				if (Input::KeyPressed(m_keyMoveLeft)) moveDir.x += 1;
				if (Input::KeyPressed(m_keyMoveRight)) moveDir.x -= 1;
				if (Input::KeyPressed(m_keyMoveUp)) moveDirY += 1;
				if (Input::KeyPressed(m_keyMoveDown)) moveDirY -= 1;
			}

			if (!m_isRotationDisabled) {
				if (m_canRotateWithArrow) {
					secondaryLookDir.x = 0;
					secondaryLookDir.y = 0;

					if (Input::KeyPressed(m_keyRotateUp)) secondaryLookDir.x += 1;
					if (Input::KeyPressed(m_keyRotateDown)) secondaryLookDir.x -= 1;
					if (Input::KeyPressed(m_keyRotateLeft)) secondaryLookDir.y -= 1;
					if (Input::KeyPressed(m_keyRotateRight)) secondaryLookDir.y += 1;

					secondaryLookDir *= m_arrowSensitivity * Time::GetDeltaTime() * 100;
					cameraRotation.y += secondaryLookDir.y;
					cameraRotation.x += secondaryLookDir.x;
				}

				CalculateCameraRotation(Input::GetMousePositionDelta());
			}

			CameraMovement();
			first = false;
		}

		void FreeCameraController::CalculateCameraRotation(Vector2 mouseDelta) {
			if (moveDirY != 0 || 
				moveDir.SquaredMagnitude() > 0 || 
				secondaryLookDir.SquaredMagnitude() > 0 || 
				mouseDelta.SquaredMagnitude() > 0 || 
				first) {
				if (m_canRotateWithMouse) {
					mouseDelta *= m_mouseSensitivity;
					cameraRotation.y += mouseDelta.x; // yaw (horizontal)
					cameraRotation.x += mouseDelta.y; // pitch (vertikal)	
				}

				MathUtil::Clamp(cameraRotation.x, -89.0f, 89.0f);
			}
		}

		void FreeCameraController::CameraZoom() {
			int dir;
			if (Input::GetScrollDir(dir)) {
				m_fov -= static_cast<float>(dir) * 2;
				MathUtil::Clamp(m_fov, m_minFov, m_maxFov);
				m_camera->SetFOV(m_fov);
			}
		}

		void FreeCameraController::CameraMovement() {
			forward = trans->GetForward();
			moveDir.Normalize();

			Vector3 right = trans->GetRight(forward);
			Vector3 up = trans->GetUp(forward, right);

			Vector3 worldMoveDir =
				right * moveDir.x +
				up * moveDir.y +
				forward * moveDir.z;

			worldMoveDir.y += moveDirY * m_verticalMovementspeedMultiplier;

			float multiplier = (Input::KeyPressed(m_keyMoveSprint)) ? m_sprintMultiplier : 1;
			multiplier *= (Input::KeyPressed(m_keyMoveSlow)) ? m_slowMultiplier : 1;
			cameraPosition += worldMoveDir * m_movementSpeed * multiplier * Time::GetDeltaTime();

			trans->SetPosition(cameraPosition);
			trans->SetRotation(cameraRotation);
		}

	}

}