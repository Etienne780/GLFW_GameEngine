#include <CoreLib\Math\Vector4.h>

#include "EngineLib\GameObject.h"
#include "EngineLib\Components\Transform_C.h"

namespace EngineCore {

	namespace Component {

		const std::string Transform::compName = "Transform";

		Transform::Transform(GameObject* gameObject) :
			ComponentBase(compName, gameObject) {
		}

		void Transform::CalculateModeMat() {
			using namespace GLTransform;

			m_modeMat = ScaleNonUniform(m_scale);
			RotationXYZ(m_modeMat, m_rotation);
			Translation(m_modeMat, m_position);
		}

		#pragma region Get

		Vector3 Transform::GetLocalPosition() const {
			return m_position;
		}

		Vector3 Transform::GetLocalRotation() const {
			return m_rotation;
		}

		Vector3 Transform::GetLocalScale() const {
			return m_scale;
		}

		Matrix Transform::GetLocalModelMatrix() {
			if (m_isTransformDirty) {
				CalculateModeMat();
				m_isTransformDirty = false;
			}

			return m_modeMat;
		}

		Vector3 Transform::GetWorldPosition() const {
			if (auto parent = m_gameObject->GetParent()) {
				Matrix parentMatrix = parent->GetTransform()->GetWorldModelMatrix();
				Vector4 transformedPos = parentMatrix * Vector4(m_position, 1.0f);
				return Vector3(transformedPos.x, transformedPos.y, transformedPos.z);
			}
			return m_position;
		}

		Vector3 Transform::GetWorldRotation() const {
			if (auto parent = m_gameObject->GetParent()) {
				return parent->GetTransform()->GetWorldRotation() + m_rotation;
			}
			return m_rotation;
		}

		Vector3 Transform::GetWorldScale() const {
			if (auto parent = m_gameObject->GetParent()) {
				return m_scale * parent->GetTransform()->GetWorldScale();
			}
			return m_scale;
		}

		Matrix Transform::GetWorldModelMatrix() {
			if (m_isTransformDirty) {
				CalculateModeMat();
				m_isTransformDirty = false;
			}

			if (auto parent = m_gameObject->GetParent()) {
				Matrix parentMatrix = parent->GetTransform()->GetWorldModelMatrix();
				return parentMatrix * m_modeMat;
			}
			return m_modeMat;
		}

		#pragma endregion

		#pragma region Set

		Transform& Transform::SetPosition(float x, float y, float z) {
			m_position.Set(x, y, z);
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::SetRotation(float x, float y, float z) {
			m_rotation.Set(x, y, z);
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::SetScale(float x, float y, float z) {
			m_scale.Set(x, y, z);
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::SetPosition(const Vector3& pos) {
			m_position = pos;
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::SetRotation(const Vector3& rot) {
			m_rotation = rot;
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::SetScale(const Vector3& scale) {
			m_scale = scale;
			m_isTransformDirty = true;
			return *this;
		}

		#pragma endregion

		#pragma region Add

		Transform& Transform::AddPosition(float x, float y, float z) {
			m_position.x += x;
			m_position.y += y;
			m_position.z += z;
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::AddRotation(float x, float y, float z) {
			m_rotation.x += x;
			m_rotation.y += y;
			m_rotation.z += z;
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::AddScale(float x, float y, float z) {
			m_scale.x += x;
			m_scale.y += y;
			m_scale.z += z;
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::AddPosition(const Vector3& pos) {
			m_position += pos;
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::AddRotation(const Vector3& rot) {
			m_rotation += rot;
			m_isTransformDirty = true;
			return *this;
		}

		Transform& Transform::AddScale(const Vector3& scale) {
			m_scale += scale;
			m_isTransformDirty = true;
			return *this;
		}

		#pragma endregion

	}
}