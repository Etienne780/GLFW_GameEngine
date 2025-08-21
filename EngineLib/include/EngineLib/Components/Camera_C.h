#pragma once
#include <string>
#include <CoreLib\ConversionUtils.h>
#include <CoreLib\Math\Matrix4x4.h>

#include "..\ComponentBase.h"

namespace EngineCore {
	class Engine;

	namespace Component {

		class Camera : public EngineCore::ComponentBase {
			friend class EngineCore::Engine;
		public:
			Camera(unsigned int gameObjectID);
			bool CanDisalbe() const override { return false; }

			COMPONENT_TYPE_DEFINITION(Camera);

			float GetFOV() const;
			bool GetOrthograpic() const;
			float GetAspectRatio() const;
			float GetNearPlane() const;
			float GetFarPlane() const;
			Matrix4x4 GetProjectionMatrix();
			Matrix4x4 GetViewMatrix();

			Camera& SetFOV(float fov);
			Camera& SetOrthograpic(bool value);
			/**
			* @brief Sets if the aspect ratio should be created automaticly with the window dimensions or with a default value
			* @param value 
			*/
			Camera& SetAspectRatioAuto(bool value);
			/**
			* @brief changes the aspect ratio, need to call SetAspectRatioAuto(true) to take effect
			*/
			Camera& SetAspectRatio(float aspectRatio);
			Camera& SetNearPlane(float nearPlane);
			Camera& SetFarPlane(float farPlane);
			Camera& SetPlanes(float nearPlane, float farPlane);

		private:
			// gets updatet in the engine loop (before the update loop of the application)
			static int m_windowWidth, m_windowHeight;
			std::shared_ptr<GameObject> m_gameObject;

			float m_fov = 66.0f;
			bool m_isOrthograpic = false;
			bool m_projectionChanged = true;
			float m_lastAspectRatio = 0;

			bool m_calculateAspectRatioWithWindow = true;
			float m_aspectRatio = 16.0f / 9.0f;
			float m_nearPlane = 0.1f;
			float m_farPlane = 1000.0f;

			Matrix4x4 m_projection;
			Matrix4x4 m_view;

			float CalculateAspectRatio() const;
			void CreateProjectionMatrix(float aspectRatio);
			void CreateViewMatrix();

			static void SetWindowDimensions(int width, int height);
		};

	}

}
