#include <CoreLib\Math\Vector4.h>

#include "EngineLib\GameObject.h"
#include "EngineLib\Components\Transform_C.h"

namespace EngineCore {

	namespace Component {
		
		const std::string compName = "Transform";

		Transform::Transform(GameObjectID gameObjectID) :
			ComponentBase(compName, gameObjectID) {
			m_gameObject = GetGameObject();
		}

		void Transform::OnInspectorGUIImpl(IUIRenderer& ui) {
			ui.DrawDragFloat3("Position", &m_position, 0.15f);
			ui.DrawDragFloat3("Rotation", &m_rotation, 0.3f);
			ui.DrawDragFloat3("Scale", &m_scale, 0.2f);

			if (ui.DrawCollapsingHeader("World Transform")) {
				Vector3 worldPos = GetWorldPosition();
				Vector3 worldRot = GetWorldRotation();
				Vector3 worldScale = GetWorldScale();

				ui.DrawLabel(FormatUtils::formatString("Position: ({}, {}, {})", worldPos.x, worldPos.y, worldPos.z));
				ui.DrawLabel(FormatUtils::formatString("Rotation: ({}, {}, {})", worldRot.x, worldRot.y, worldRot.z));
				ui.DrawLabel(FormatUtils::formatString("Scale: ({}, {}, {})", worldScale.x, worldScale.y, worldScale.z));
			}

			MarkDirty();
		}

		void Transform::CalculateLocalModelMat() {
			using namespace GLTransform4x4;

			Vector3 radians = { 
				ConversionUtils::ToRadians(m_rotation.x),
				ConversionUtils::ToRadians(m_rotation.y),
				ConversionUtils::ToRadians(m_rotation.z) 
			};

			m_localMatrix = Scale(m_scale);
			MakeRotateXYZ(m_localMatrix, radians);
			MakeTranslate(m_localMatrix, m_position);
		}

		void Transform::CalculateWorldModelMat() {
			using namespace GLTransform4x4;

			if (m_localMatrixDirty) {
				CalculateLocalModelMat();
				m_localMatrixDirty = false;
			}

			if (auto parent = m_gameObject->GetParent()) {
				m_worldMatrix = parent->GetTransform()->GetWorldModelMatrix() * m_localMatrix;
			}
			else {
				m_worldMatrix = m_localMatrix;
			}
		}

		void Transform::MarkDirty() {
			m_localMatrixDirty = true;
			m_worldMatrixDirty = true;
			for (auto& child : m_gameObject->GetChildren()) {
				child->GetTransform()->MarkDirty();
			}
		}

		void Transform::LookAt(float x, float y, float z) {
			LookAt(x, y, z, Vector3::up);
		}

		void Transform::LookAt(float x, float y, float z, const Vector3& worldUp) {
			Vector3 forward = (Vector3{x, y, z} - GetWorldPosition()).Normalize();
			Matrix4x4 rotMat = GLTransform4x4::LookRotation(forward, worldUp);

			Vector3 eulerRad = GLTransform4x4::MatrixToEuler(rotMat);
			Vector3 eulerDeg{
				ConversionUtils::ToDegrees(eulerRad.x),
				ConversionUtils::ToDegrees(eulerRad.y),
				ConversionUtils::ToDegrees(eulerRad.z)
			};
			SetRotation(eulerDeg);
		}

		void Transform::LookAt(const Vector3& target) {
			LookAt(target, Vector3::up);
		}

		void Transform::LookAt(const Vector3& target, const Vector3& worldUp) {
			Vector3 forward = (target - GetWorldPosition()).Normalize();
			Matrix4x4 rotMat = GLTransform4x4::LookRotation(forward, worldUp);

			Vector3 eulerRad = GLTransform4x4::MatrixToEuler(rotMat);
			Vector3 eulerDeg{
				ConversionUtils::ToDegrees(eulerRad.x),
				ConversionUtils::ToDegrees(eulerRad.y),
				ConversionUtils::ToDegrees(eulerRad.z)
			};
			SetRotation(eulerDeg);
		}

		#pragma region Get

		Vector3 Transform::GetLocalPosition() const {
			if (IsDead("Cant get local position")) {
				return Vector3::zero;
			}
			return m_position;
		}

		Vector3 Transform::GetLocalRotation() const {
			if (IsDead("Cant get local rotation")) {
				return Vector3::zero;
			}
			return m_rotation;
		}

		Vector3 Transform::GetLocalScale() const {
			if (IsDead("Cant get local scale")) {
				return Vector3::zero;
			}
			return m_scale;
		}

		Matrix4x4 Transform::GetLocalModelMatrix() {
			if (IsDead("Cant get local Model-Matrix")) {
				return Matrix4x4();
			}
			if (m_localMatrixDirty) {
				CalculateLocalModelMat();
				m_localMatrixDirty = false;
			}

			return m_localMatrix;
		}

		Vector3 Transform::GetWorldPosition() const {
			if (IsDead("Cant get world position")) {
				return Vector3::zero;
			}
			if (auto parent = m_gameObject->GetParent()) {
				Matrix4x4 parentMatrix = parent->GetTransform()->GetWorldModelMatrix();
				Vector4 transformedPos = parentMatrix * Vector4(m_position, 1.0f);
				return Vector3(transformedPos.x, transformedPos.y, transformedPos.z);
			}
			return m_position;
		}

		Vector3 Transform::GetWorldRotation() const {
			if (IsDead("Cant get world rotation")) {
				return Vector3::zero;
			}
			if (auto parent = m_gameObject->GetParent()) {
				return parent->GetTransform()->GetWorldRotation() + m_rotation;
			}
			return m_rotation;
		}

		Vector3 Transform::GetWorldScale() const {
			if (IsDead("Cant get world scale")) {
				return Vector3::zero;
			}
			if (auto parent = m_gameObject->GetParent()) {
				return m_scale * parent->GetTransform()->GetWorldScale();
			}
			return m_scale;
		}

		Matrix4x4* Transform::GetWorldModelMatrixPtr() {
			if (IsDead("Cant get world Model-Matrix")) {
				return nullptr;
			}

			if (m_worldMatrixDirty) {
				CalculateWorldModelMat();
				m_worldMatrixDirty = false;
			}

			return &m_worldMatrix;
		}

		const Matrix4x4& Transform::GetWorldModelMatrix() {
			if (IsDead("Cant get world Model-Matrix")) {
				static Matrix4x4 identity;
				return identity;
			}

			if (m_worldMatrixDirty) {
				CalculateWorldModelMat();
				m_worldMatrixDirty = false;
			}

			return m_worldMatrix;
		}

		Vector3 Transform::GetForward() const {
			if (IsDead("Cant get forward")) {
				return Vector3::zero;
			}
			Vector3 forward{
				sin(ConversionUtils::ToRadians(m_rotation.y)) * cos(ConversionUtils::ToRadians(m_rotation.x)),
				sin(ConversionUtils::ToRadians(m_rotation.x)),
				cos(ConversionUtils::ToRadians(m_rotation.y)) * cos(ConversionUtils::ToRadians(m_rotation.x))
			};
			forward.Normalize();
			return forward;
		}

		Vector3 Transform::GetRight() const {
			if (IsDead("Cant get right")) {
				return Vector3::zero;
			}
			return GetForward().Cross(Vector3::up).Normalized();
		}

		Vector3 Transform::GetRight(const Vector3& forward) const {
			if (IsDead("Cant get right")) {
				return Vector3::zero;
			}
			return forward.Cross(Vector3::up).Normalized();
		}

		Vector3 Transform::GetUp() const {
			if (IsDead("Cant get up")) {
				return Vector3::zero;
			}
			return GetRight().Cross(GetForward()).Normalized();
		}

		Vector3 Transform::GetUp(const Vector3& forward) const {
			if (IsDead("Cant get up")) {
				return Vector3::zero;
			}
			return GetRight().Cross(forward).Normalized();
		}

		Vector3 Transform::GetUp(const Vector3& forward, const Vector3& right) const {
			if (IsDead("Cant get up")) {
				return Vector3::zero;
			}
			return right.Cross(forward).Normalized();
		}

		#pragma endregion

		#pragma region Set

		Transform& Transform::SetPosition(float x, float y, float z) {
			if (IsDead("Cant set position")) {
				return *this;
			}
			m_position.Set(x, y, z);
			MarkDirty();
			return *this;
		}

		Transform& Transform::SetRotation(float x, float y, float z) {
			if (IsDead("Cant set rotation")) {
				return *this;
			}
			m_rotation.Set(x, y, z);
			MarkDirty();
			return *this;
		}

		Transform& Transform::SetScale(float x, float y, float z) {
			if (IsDead("Cant set scale")) {
				return *this;
			}
			m_scale.Set(x, y, z);
			MarkDirty();
			return *this;
		}

		Transform& Transform::SetPosition(const Vector3& pos) {
			if (IsDead("Cant set position")) {
				return *this;
			}
			m_position = pos;
			MarkDirty();
			return *this;
		}

		Transform& Transform::SetRotation(const Vector3& rot) {
			if (IsDead("Cant set rotation")) {
				return *this;
			}
			m_rotation = rot;
			MarkDirty();
			return *this;
		}

		Transform& Transform::SetScale(const Vector3& scale) {
			if (IsDead("Cant set scale")) {
				return *this;
			}
			m_scale = scale;
			MarkDirty();
			return *this;
		}

		#pragma endregion

		#pragma region Add

		Transform& Transform::AddPosition(float x, float y, float z) {
			if (IsDead("Cant add position")) {
				return *this;
			}
			m_position.x += x;
			m_position.y += y;
			m_position.z += z;
			MarkDirty();
			return *this;
		}

		Transform& Transform::AddRotation(float x, float y, float z) {
			if (IsDead("Cant add rotation")) {
				return *this;
			}
			m_rotation.x += x;
			m_rotation.y += y;
			m_rotation.z += z;
			MarkDirty();
			return *this;
		}

		Transform& Transform::AddScale(float x, float y, float z) {
			if (IsDead("Cant add scale")) {
				return *this;
			}
			m_scale.x += x;
			m_scale.y += y;
			m_scale.z += z;
			MarkDirty();
			return *this;
		}

		Transform& Transform::AddPosition(const Vector3& pos) {
			if (IsDead("Cant add position")) {
				return *this;
			}
			m_position += pos;
			MarkDirty();
			return *this;
		}

		Transform& Transform::AddRotation(const Vector3& rot) {
			if (IsDead("Cant add rotation")) {
				return *this;
			}
			m_rotation += rot;
			MarkDirty();
			return *this;
		}

		Transform& Transform::AddScale(const Vector3& scale) {
			if (IsDead("Cant add scale")) {
				return *this;
			}
			m_scale += scale;
			MarkDirty();
			return *this;
		}

		#pragma endregion
		
	}
}