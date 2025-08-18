#include "EngineLib\Components\FreeCameraController_C.h"

namespace EngineCore {

	namespace Component {

		FreeCameraController::FreeCameraController(GameObject* gameObject) : Script(gameObject){
			m_camera = m_gameObject->GetComponent<Component::Camera>();
			trans = m_gameObject->GetTransform();
		}

		void FreeCameraController::Update() {
			if (!m_isZoomDisabled) {
				CameraZoom();
			}

			if (!m_isMovementDisabled) {
				moveDir.x = 0;
				moveDir.z = 0;
				moveDirY = 0;

				if (Input::KeyPressed(GLFW_KEY_W)) moveDir.z += 1;
				if (Input::KeyPressed(GLFW_KEY_S)) moveDir.z -= 1;
				if (Input::KeyPressed(GLFW_KEY_A)) moveDir.x += 1;
				if (Input::KeyPressed(GLFW_KEY_D)) moveDir.x -= 1;
				if (Input::KeyPressed(GLFW_KEY_SPACE)) moveDirY += 1;
				if (Input::KeyPressed(GLFW_KEY_LEFT_CONTROL)) moveDirY -= 1;
			}

			if (!m_isRotationDisabled) {
				if (m_canRotateWithArrow) {
					secondaryLookDir.x = 0;
					secondaryLookDir.y = 0;

					if (Input::KeyPressed(GLFW_KEY_UP)) secondaryLookDir.x += 1;
					if (Input::KeyPressed(GLFW_KEY_DOWN)) secondaryLookDir.x -= 1;
					if (Input::KeyPressed(GLFW_KEY_LEFT)) secondaryLookDir.y -= 1;
					if (Input::KeyPressed(GLFW_KEY_RIGHT)) secondaryLookDir.y += 1;

					secondaryLookDir *= m_arrowSensitivity;
					cameraRotation.y += secondaryLookDir.y;
					cameraRotation.x += secondaryLookDir.x;
				}

				CalculateCameraRotation(Input::GetMousePositionDelta());
			}

			CameraMovement();
			first = false;
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

			float multiplier = (Input::KeyPressed(GLFW_KEY_LEFT_SHIFT)) ? m_sprintMultiplier : 1;
			multiplier *= (Input::KeyPressed(GLFW_KEY_LEFT_ALT)) ? m_slowMultiplier : 1;
			cameraPosition += worldMoveDir * m_movementSpeed * multiplier * Time::GetDeltaTime();

			trans->SetPosition(cameraPosition);
			trans->SetRotation(cameraRotation);
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

	}

}