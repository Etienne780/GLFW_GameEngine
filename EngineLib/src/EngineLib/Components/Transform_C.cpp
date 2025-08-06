#include "EngineLib\Components\Transform_C.h"

namespace EngineCore {
	const std::string Transform::compName = "Transform";

	Transform::Transform() :
		Component(compName) {
	}

	Transform::Transform(const Vector3& pos, const Vector3& rot, const Vector3& scale) : 
		Component(compName) {
		m_position = pos;
		m_rotation = rot;
		m_scale = scale;
	}

	void Transform::CalculateModeMat() {
		using namespace GLTransform;

		m_modeMat = ScaleNonUniform(m_scale);
		RotationXYZ(m_modeMat, m_rotation);
		Translation(m_modeMat, m_position);
	}

	#pragma region Get

	Vector3 Transform::GetPosition() const {
		return m_position;
	}

	Vector3 Transform::GetRotation() const {
		return m_rotation;
	}

	Vector3 Transform::GetScale() const {
		return m_scale;
	}

	#pragma endregion

	#pragma region Set

	Transform& Transform::SetPosition(float x, float y, float z) {
		m_position.Set(x, y, z);
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::SetRotation(float x, float y, float z) {
		m_rotation.Set(x, y, z);
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::SetScale(float x, float y, float z) {
		m_scale.Set(x, y, z);
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::SetPosition(const Vector3& pos) {
		m_position = pos;
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::SetRotation(const Vector3& rot) {
		m_rotation = rot;
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::SetScale(const Vector3& scale) {
		m_scale = scale;
		CalculateModeMat();
		return *this;
	}

	#pragma endregion

	#pragma region Add

	Transform& Transform::AddPosition(float x, float y, float z) {
		m_position.x += x;
		m_position.y += y;
		m_position.z += z;
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::AddRotation(float x, float y, float z) {
		m_rotation.x += x;
		m_rotation.y += y;
		m_rotation.z += z;
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::AddScale(float x, float y, float z) {
		m_scale.x += x;
		m_scale.y += y;
		m_scale.z += z;
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::AddPosition(const Vector3& pos) {
		m_position += pos;
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::AddRotation(const Vector3& rot) {
		m_rotation += rot;
		CalculateModeMat();
		return *this;
	}

	Transform& Transform::AddScale(const Vector3& scale) {
		m_scale += scale;
		CalculateModeMat();
		return *this;
	}

	#pragma endregion

}