#include <CoreLib/Log.h>
#include <CoreLib/Math/Matrix4x4.h>

#include "EngineLib/RenderLayerManager.h"
#include "EngineLib/Time.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/Components\Camera_C.h"

namespace EngineCore {

	namespace Component {

		const std::string compName = "Camera";
		int Camera::m_windowWidth = 1;
		int Camera::m_windowHeight = 1;

		Camera::Camera(GameObjectID gameObjectID) :
			ComponentBase(compName, gameObjectID) {
			m_gameObject = GetGameObject();
			m_renderLayers.push_back(RenderLayerManager::GetRenderLayerID("Default"));
		}

		void Camera::OnInspectorGUIImpl(IUIRenderer& ui) {
			ui.DrawSeparatorText("Projection");

			ui.DrawCheckbox("Orthographic", &m_isOrthograpic);

			if (!m_isOrthograpic) {
				ui.DrawSliderFloat("FOV", &m_fov, 30.0f, 180.0f);
			}
			else {
				ui.DrawLabelDisabled("FOV (not used in orthographic mode)");
			}

			ui.DrawCheckbox("Aspect Ratio Auto", &m_calculateAspectRatioWithWindow);

			if (!m_calculateAspectRatioWithWindow) {
				ui.DrawSliderFloat("Aspect Ratio", &m_aspectRatio, 0.5f, 3.0f);
			}
			else {
				ui.DrawLabelDisabled("Aspect Ratio (not used when Aspect Ratio Auto is on)");
			}

			ui.DrawSeparatorText("Clipping Planes");

			ui.DrawDragFloat("Near Plane", &m_nearPlane, 0.01f, 0.01f, m_farPlane - 0.01f);
			ui.DrawDragFloat("Far Plane", &m_farPlane, 1.0f, m_nearPlane + 0.01f, 10000.0f);


			if (ui.DrawCollapsingHeader("Render-Layers", false)) {
				auto renderLayers = RenderLayerManager::GetNameToRenderLayer();
				for (const auto& [name, ID] : renderLayers) {
					auto it = std::find(m_renderLayers.begin(), m_renderLayers.end(), ID);
					bool check = (it != m_renderLayers.end());
					bool preCheck = check;
					ui.DrawCheckbox(name, &check);

					if (preCheck != check) {
						if (check) {
							// Add layer
							m_renderLayers.push_back(ID);
						}
						else {
							// Remove layer
							auto it = std::find(m_renderLayers.begin(), m_renderLayers.end(), ID);
							if (it != m_renderLayers.end()) {
								m_renderLayers.erase(it);
							}
						}
					}
				}
			}

			m_projectionChanged = true;
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

		bool Camera::GetAspectRatioAuto() const {
			if (IsDead("Cant get is AspectRatioAuto")) {
				return false;
			}
			return m_calculateAspectRatioWithWindow;
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

		const std::vector<RenderLayerID>& Camera::GetRenderLayers() const {
			return m_renderLayers;
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

		Camera& Camera::SetCameraLayers(std::vector<RenderLayerID> renderLayers) {
			m_renderLayers = renderLayers;
			return *this;
		}

		Camera& Camera::AddCameraLayer(RenderLayerID renderLayer) {
			m_renderLayers.push_back(renderLayer);
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
