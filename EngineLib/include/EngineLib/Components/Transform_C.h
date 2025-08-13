#pragma once
#include "..\ComponentBase.h"
#include "CoreLib\Math\Vector3.h"
#include "CoreLib\Math\Matrix.h"

namespace EngineCore {

	namespace Component {

		class Transform : public EngineCore::ComponentBase {
		public:
			Transform(GameObject* gameObject);

			Vector3 GetPosition() const;
			Vector3 GetRotation() const;
			Vector3 GetScale() const;
			Matrix GetModelMat() const;

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

			Vector3 m_position;
			Vector3 m_rotation;
			Vector3 m_scale;

			Matrix m_modeMat;

			void CalculateModeMat();
		};

	}

}
