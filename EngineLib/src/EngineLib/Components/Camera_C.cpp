#include <CoreLib\Log.h>
#include <CoreLib\Math\Matrix4x4.h>

#include "EngineLib\Time.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\Components\Camera_C.h"

namespace EngineCore {

	namespace Component {

		const std::string compName = "Camera";
		int Camera::m_windowWidth = 1;
		int Camera::m_windowHeight = 1;

		Camera::Camera(unsigned int gameObjectID) :
			ComponentBase(compName, gameObjectID) {
			m_gameObject = GetGameObject();
		}

		#pragma region Get

		float Camera::GetFOV() const {
			if (IsDead("Cant get FOV")) {
				return 0;
			}
			return m_fov;
		}

		bool Camera::GetOrthograpic() const {
			if (IsDead("Cant get is Orthograpic")) {
				return false;
			}
			return m_isOrthograpic;
		}

		float Camera::GetAspectRatio() const {
			if (IsDead("Cant get aspect ratio")) {
				return 0;
			}
			return CalculateAspectRatio();
		}

		float Camera::GetNearPlane() const {
			if (IsDead("Cant get near plane")) {
				return 0;
			}
			return m_nearPlane;
		}

		float Camera::GetFarPlane() const {
			if (IsDead("Cant get far plane")) {
				return 0;
			}
			return m_farPlane;
		}

		Matrix4x4 Camera::GetProjectionMatrix() {
			if (IsDead("Cant get Projection-Matrix")) {
				return Matrix4x4();
			}
			float aspectRatio = CalculateAspectRatio();

			if (m_projectionChanged || (m_calculateAspectRatioWithWindow && m_lastAspectRatio != aspectRatio)) {
				m_lastAspectRatio = aspectRatio;
				CreateProjectionMatrix(aspectRatio);
				m_projectionChanged = false;
			}

			return m_projection;
		}


		Matrix4x4 Camera::GetViewMatrix() {
			if (IsDead("Cant get View-Matrix")) {
				return Matrix4x4();
			}
			static int lastFrame = -1;
			if (lastFrame < Time::GetFrameCount()) {
				CreateViewMatrix();
				lastFrame = Time::GetFrameCount();
			}

			return m_view;
		}

		#pragma endregion

		#pragma region Set

		Camera& Camera::SetFOV(float fov) {
			if (IsDead("Cant set FOV")) {
				return *this;
			}
			m_fov = fov;
			m_projectionChanged = true;

			return *this;
		}

		Camera& Camera::SetOrthograpic(bool value) {
			if (IsDead("Cant set Orthograpic")) {
				return *this;
			}
			m_isOrthograpic = value;
			m_projectionChanged = true;

			return *this;
		}

		Camera& Camera::SetAspectRatioAuto(bool value) {
			if (IsDead("Cant set AspectRatioAuto")) {
				return *this;
			}
			m_calculateAspectRatioWithWindow = value;
			m_projectionChanged = true;

			return *this;
		}

		Camera& Camera::SetAspectRatio(float aspectRatio) {
			if (IsDead("Cant set aspect ratio")) {
				return *this;
			}
			m_aspectRatio = aspectRatio;
			m_projectionChanged = true;

			return *this;
		}

		Camera& Camera::SetNearPlane(float nearPlane) {
			if (IsDead("Cant set near plane")) {
				return *this;
			}
			m_nearPlane = nearPlane;
			m_projectionChanged = true;
			
			return *this;
		}

		Camera& Camera::SetFarPlane(float farPlane) {
			if (IsDead("Cant set far plane")) {
				return *this;
			}
			m_farPlane = farPlane;
			m_projectionChanged = true;

			return *this;
		}

		Camera& Camera::SetPlanes(float nearPlane, float farPlane) {
			if (IsDead("Cant set planes")) {
				return *this;
			}
			m_nearPlane = nearPlane;
			m_farPlane = farPlane;
			m_projectionChanged = true;

			return *this;
		}

		#pragma endregion

		#pragma region Other
		
		float Camera::CalculateAspectRatio() const {
			return m_calculateAspectRatioWithWindow
				? static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight)
				: m_aspectRatio;
		}

		void Camera::CreateProjectionMatrix(float aspectRatio) {
			if (m_isOrthograpic) {
				float orthoHeight = m_fov; // Could treat FOV as "size" for ortho
				float orthoWidth = orthoHeight * aspectRatio;

				m_projection = GLTransform4x4::Orthographic(
					-orthoWidth * 0.5f, orthoWidth * 0.5f,
					-orthoHeight * 0.5f, orthoHeight * 0.5f,
					m_nearPlane, m_farPlane
				);
			}
			else {
				#ifndef NDEBUG
				if (m_nearPlane > m_farPlane) {
					Log::Warn("Camera: Near plane is greater than far plane, {} > {}!", m_nearPlane, m_farPlane);
				}
				#endif

				m_projection = GLTransform4x4::Perspective(
					ConversionUtils::ToRadians(m_fov), 
					aspectRatio,
					m_nearPlane, 
					m_farPlane);
			}
		}

		void Camera::CreateViewMatrix() {
			auto trans = m_gameObject->GetTransform();
			Vector3 worldPos = trans->GetWorldPosition();
			Vector3 forward = trans->GetForward();
			m_view = GLTransform4x4::LookAt(worldPos, worldPos + forward, trans->GetUp(forward));
		}

		void Camera::SetWindowDimensions(int width, int height) {
			m_windowWidth = width;
			m_windowHeight = height;
		}

		#pragma endregion

	}

}
