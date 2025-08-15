#pragma once
#include "..\ComponentBase.h"
#include "CoreLib\Math\Vector3.h"
#include "CoreLib\Math\Matrix.h"

namespace EngineCore {

	namespace Component {

		class Transform : public EngineCore::ComponentBase {
		public:
			Transform(GameObject* gameObject);

			COMPONENT_TYPE_DEFINITION(Transform);

			/**
			* @brief Gets the Position local to the parent GameObject if it has one
			*/
			Vector3 GetLocalPosition() const;
			/**
			* @brief Gets the Rotation local to the parent GameObject if it has one
			*/
			Vector3 GetLocalRotation() const;
			/**
			* @brief Gets the Scale local to the parent GameObject if it has one
			*/
			Vector3 GetLocalScale() const;
			/**
			* @brief Gets the Model-Matrix local to the parent GameObject if it has one
			*/
			Matrix GetLocalModelMatrix();

			/**
			* @brief Gets the world Position (local to world origin).
			*/
			Vector3 GetWorldPosition() const;
			/**
			* @brief Gets the world Rotation (local to world origin).
			*/
			Vector3 GetWorldRotation() const;
			/**
			* @brief Gets the world Scale (local to world origin).
			*/
			Vector3 GetWorldScale() const;
			/**
			* @brief Gets the world Model-Matrix (local to world origin).
			*/
			Matrix GetWorldModelMatrix();

			Transform& SetPosition(float x, float y, float z);
			Transform& SetRotation(float x, float y, float z);
			Transform& SetScale(float x, float y, float z);

			Transform& SetPosition(const Vector3& pos);
			Transform& SetRotation(const Vector3& rot);
			Transform& SetScale(const Vector3& scale);

			Transform& AddPosition(float x, float y, float z);
			Transform& AddRotation(float x, float y, float z);
			Transform& AddScale(float x, float y, float z);

			Transform& AddPosition(const Vector3& pos);
			Transform& AddRotation(const Vector3& rot);
			Transform& AddScale(const Vector3& scale);

		private:
			static const std::string compName;

			// if the transform has changed
			bool m_isTransformDirty = true;
			// Local position
			Vector3 m_position;
			// Local rotation
			Vector3 m_rotation;
			// Local scale
			Vector3 m_scale;

			// Local model matrix
			Matrix m_modeMat;

			void CalculateModeMat();
		};

	}

}
