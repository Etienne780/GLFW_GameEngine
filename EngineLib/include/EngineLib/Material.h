#pragma once
#include <unordered_map>
#include <string>

#include <CoreLib\Log.h>
#include <CoreLib\Math\Vector2.h>
#include <CoreLib\Math\Vector3.h>
#include <CoreLib\Math\Vector4.h>
#include <CoreLib\Math\Matrix.h>

#include "ResourceManager.h"
#include "ShaderBindObject.h"
#include "EngineTypes.h"

namespace EngineCore {

	class Shader;

	class Material {
	public:
		Material() = default;
		Material(ShaderID shaderID);

		template<typename T>
		T GetParam(const std::string& name) {
			return m_shaderBindObject.GetParam<T>(name);
		}

		template<typename T>
		void SetParam(const std::string& name, const T& value) {
			m_shaderBindObject.SetParam<T>(name, value);
		}

		/**
		* @brief Binds and applys the params to the shader
		*/
		Shader* BindToShader() const;
		void ApplyParamsOnly(Shader* s) const;

		std::string GetParamString() const;
		ShaderID GetShaderID() const;
		bool GetIsTransparent() const;
		
		std::vector<bool> GetParamBools() const;
		std::vector<int> GetParamInts() const;
		std::vector<float> GetParamFloats() const;
		std::vector<Vector2> GetParamVector2s() const;
		std::vector<Vector3> GetParamVector3s() const;
		std::vector<Vector4> GetParamVector4s() const;
		std::vector<Matrix> GetParamMatrices() const;
		std::vector<Texture2DID> GetParamTexture2Ds() const;

	private:
		ShaderID m_shaderID = ShaderID(ENGINE_INVALID_ID);
		ShaderBindObject m_shaderBindObject;
	};

}