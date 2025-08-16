#pragma once
#include <string>
#include <CoreLib\ConversionUtils.h>
#include <CoreLib\Math\Matrix.h>

#include "..\ComponentBase.h"

namespace EngineCore {
	class Engine;

	namespace Component {

		class Camera : public EngineCore::ComponentBase {
			friend class EngineCore::Engine;
		public:
			Camera(GameObject* gameObject);

			COMPONENT_TYPE_DEFINITION(Camera);

			float GetFOV() const;
			bool GetOrthograpic() const;
			float GetAspectRatio() const;
			float GetNearPlane() const;
			float GetFarPlane() const;
			Matrix GetProjectionMatrix();
			Matrix GetViewMatrix();

			Camera& SetFOV(float fov);
			Camera& SetOrthograpic(bool value);
			/**
			* @brief Sets if the aspect ratio should be created automaticly with the window dimensions or with a default value
			* @param value 
			*/
			Camera& SetAspectRatioAuto(bool value);
			Camera& SetAspectRatio(float aspectRatio);
			Camera& SetNearPlane(float nearPlane);
			Camera& SetFarPlane(float farPlane);
			Camera& SetPlanes(float nearPlane, float farPlane);

		private:
			static const std::string compName;
			// gets updatet in the engine loop (before the update loop of the application)
			static int m_windowWidth, m_windowHeight;

			float m_fov = 66.0f;
			bool m_isOrthograpic = false;
			bool m_projectionChanged = true;
			float m_lastAspectRatio = 0;

			bool m_calculateAspectRatioWithWindow = true;
			float m_aspectRatio = 16.0f / 9.0f;
			float m_nearPlane = 0.1f;
			float m_farPlane = 1000.0f;

			Matrix m_projection;
			Matrix m_view;

			float CalculateAspectRatio() const;
			void CreateProjectionMatrix(float aspectRatio);
			void CreateViewMatrix();

			static void SetWindowDimensions(int width, int height);
		};

	}

}
