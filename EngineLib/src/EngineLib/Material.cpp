#include <CoreLib\FormatUtils.h>

#include "EngineLib\ResourceManager.h"
#include "EngineLib\Shader.h"
#include "EngineLib\Material.h"

namespace EngineCore {

	Material::Material(ShaderID shaderID)
		: m_shaderID(shaderID) {
	}

	Shader* Material::BindToShader() const {
		if (m_shaderID.value == ENGINE_INVALID_ID) {
			Log::Error("Material: Cant apply params to shader, shaderID is invalid!");
			return nullptr;
		}
		auto* rm = ResourceManager::GetInstance();
		Shader* shader = rm->GetShader(m_shaderID);
		if (!shader) {
			Log::Error("Material: Cant apply params to shader, shader is nullptr");
			return nullptr;
		}

		if (shader->GetID().value == ENGINE_INVALID_ID)
			shader->CreateGL();
		shader->Bind();

		ApplyParamsOnly(shader);

		return shader;
	}

	void Material::ApplyParamsOnly(Shader* shader) const {
		m_shaderBindObject.Bind(shader);
	}

	std::string Material::GetParamString() const {
		std::string str = "Material:\n";
		str.append(m_shaderBindObject.GetParamString());
		return str;
	}

	ShaderID Material::GetShaderID() const {
		return m_shaderID;
	}

	bool Material::GetIsTransparent() const {
		return m_shaderBindObject.GetIsTransparent();
	}

	std::vector<bool> Material::GetParamBools() const {
		return m_shaderBindObject.GetParamBools();
	}

	std::vector<int> Material::GetParamInts() const {
		return m_shaderBindObject.GetParamInts();
	}

	std::vector<float> Material::GetParamFloats() const {
		return m_shaderBindObject.GetParamFloats();
	}

	std::vector<Vector2> Material::GetParamVector2s() const {
		return m_shaderBindObject.GetParamVector2s();
	}

	std::vector<Vector3> Material::GetParamVector3s() const {
		return m_shaderBindObject.GetParamVector3s();
	}

	std::vector<Vector4> Material::GetParamVector4s() const {
		return m_shaderBindObject.GetParamVector4s();
	}

	std::vector<Matrix> Material::GetParamMatrices() const {
		return m_shaderBindObject.GetParamMatrices();
	}

	std::vector<Texture2DID> Material::GetParamTexture2Ds() const {
		return m_shaderBindObject.GetParamTexture2Ds();
	}

}