#pragma once
#include "../GameObject.h"
#include "../ComponentBase.h"
#include "CoreLib/Math/Vector3.h"
#include "CoreLib/Math/Matrix4x4.h"

namespace EngineCore {

	namespace Component {

		class Transform : public EngineCore::ComponentBase {
		friend class GameObject;
		public:
			Transform(unsigned int gameObjectID);
			void OnInspectorGUIImpl(IUIRenderer& ui) override;
			bool CanDisalbe() const override { return false; }

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
			Matrix4x4 GetLocalModelMatrix();

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
			Matrix4x4* GetWorldModelMatrixPtr();

			/**
			* @brief Gets the world Model-Matrix (local to world origin).
			*/
			const Matrix4x4& GetWorldModelMatrix();

			// Returns the normalized forward direction vector based on the current rotation (in Euler angles).
			// Forward is typically the direction the object is facing in world space.
			Vector3 GetForward() const;

			// Returns the normalized right direction vector based on the current rotation.
			// Right is perpendicular to forward and points to the object's local right-hand side.
			Vector3 GetRight() const;

			// Returns the normalized right direction vector based on a given forward vector.
			// This is useful when you already have a forward vector and want to avoid recalculating it from rotation.
			Vector3 GetRight(const Vector3& forward) const;

			// Returns the normalized up direction vector based on the current rotation.
			// Up is perpendicular to both forward and right and points to the object's local upward direction.
			Vector3 GetUp() const;

			// Returns the normalized up direction vector based on a given forward vector.
			// This is useful when you already have forward and want to calculate up without using rotation directly.
			Vector3 GetUp(const Vector3& forward) const;

			// Returns the normalized up direction vector based on given forward and right vectors.
			// This is useful when you already have both forward and right vectors and want to avoid redundant calculations.
			Vector3 GetUp(const Vector3& forward, const Vector3& right) const;

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
			// if the transform has changed
			bool m_localMatrixDirty = true;
			bool m_worldMatrixDirty = true;
			// Local position
			Vector3 m_position;
			// Local rotation
			Vector3 m_rotation;
			// Local scale
			Vector3 m_scale{1, 1, 1};

			// Local model matrix
			Matrix4x4 m_localMatrix;
			Matrix4x4 m_worldMatrix;
			std::shared_ptr<GameObject> m_gameObject = nullptr;

			void CalculateLocalModelMat();
			void CalculateWorldModelMat();
			/**
			* @brief Marks this and children dirty
			*/
			void MarkDirty();
		};

	}

}
