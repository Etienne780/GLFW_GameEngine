#pragma once
#include <unordered_map>
#include <string>

#include <CoreLib/Log.h>
#include <CoreLib\Math\Vector2.h>
#include <CoreLib\Math\Vector3.h>
#include <CoreLib\Math\Vector4.h>
#include <CoreLib\Math\Matrix.h>

#include "EngineTypes.h"

namespace EngineCore {

	class Shader;
	class ShaderBindObject {
	friend class Engine;
	public:
		ShaderBindObject() = default;
		~ShaderBindObject() = default;

		template<typename T>
		T GetParam(const std::string& name);

		template<typename T>
		void SetParam(const std::string& name, const T& value);

		// binds the params of this object to the given shader
		void Bind(Shader* shader) const;

		std::string GetParamString() const;
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
		static inline unsigned int m_maxTextureUnits = 0;
		bool m_isTransparent = false;

		std::unordered_map<std::string, bool> m_boolParams;
		std::unordered_map<std::string, int> m_intParams;
		std::unordered_map<std::string, float> m_floatParams;
		std::unordered_map<std::string, Vector2> m_vector2Params;
		std::unordered_map<std::string, Vector3> m_vector3Params;
		std::unordered_map<std::string, Vector4> m_vector4Params;
		std::unordered_map<std::string, Matrix> m_matrixParams;
		std::unordered_map<std::string, Texture2DID> m_textureParams;

		void SetMatrixParam(Shader* shader, const std::string& name, Matrix m) const;
		void SetIsTransparent(Texture2DID id);

		template<typename T>
		std::vector<T> GetParamsImpl(const std::unordered_map<std::string, T>& map) const {
			std::vector<T> values;
			values.resize(map.size());

			for (auto& [name, value] : map) {
				values.push_back(value);
			}
			return values;
		}
	};

}

#include "ShaderBindObject.inl"