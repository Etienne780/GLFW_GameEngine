#pragma once
#include <unordered_map>
#include <string>

#include "CoreLib\Math\Vector2.h"
#include "CoreLib\Math\Vector3.h"
#include "CoreLib\Math\Vector4.h"
#include "CoreLib\Math\Matrix.h"
#include "EngineTypes.h"

namespace EngineCore {

	class Shader;

	class Material {
	friend class Engine;
	public:
		Material() = default;
		Material(unsigned int shaderID);

		template<typename T>
		void SetParam(const std::string& name, const T& value);

		/**
		* @brief Binds and applys the params to the shader
		*/
		Shader* BindToShader() const;
		void ApplyParamsOnly(Shader* s) const;

		std::string GetParamString() const;
		unsigned int GetShaderID() const;
		
	private:
		static unsigned int m_maxTextureUnits;
		unsigned int m_shaderID = ENGINE_INVALID_ID;

		std::unordered_map<std::string, bool> m_boolParams;
		std::unordered_map<std::string, int> m_intParams;
		std::unordered_map<std::string, float> m_floatParams;
		std::unordered_map<std::string, Vector2> m_vector2Params;
		std::unordered_map<std::string, Vector3> m_vector3Params;
		std::unordered_map<std::string, Vector4> m_vector4Params;
		std::unordered_map<std::string, Matrix> m_matrixParams;
		std::unordered_map<std::string, unsigned int> m_textureParams;

		void SetMatrixParam(Shader* shader, const std::string& name, Matrix m) const;
	};

}


#include "Material.inl"