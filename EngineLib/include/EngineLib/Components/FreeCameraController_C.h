#pragma once
#include "EngineLib\Script.h"

namespace EngineCore {

	namespace Component {
		
		class FreeCameraController : public EngineCore::Script {
		public:
			FreeCameraController(GameObject* gameObject);

			COMPONENT_TYPE_DEFINITION(FreeCameraController);

			void Update() override;
			void CalculateCameraRotation(Vector2 mouseDelta);

			bool first = true;
			std::shared_ptr<Component::Camera> m_camera;
			Vector3 cameraRotation;
			Vector3 cameraPosition;
			Vector3 forward;
			Vector3 moveDir;
			float moveDirY = 0;

			const float m_sensitivity = 0.1f;

			float m_movementSpeed = 30.0f;
			float m_sprintMultiplier = 2.5f;
			float m_slowMultiplier = 0.4f;
			float m_verMovementspeedMultiplier = 1.2f;

			float m_fov = 66.0f;
			float m_minFov = 1.0f;
			float m_maxFov = 120.0f;
		};
		
	}

}