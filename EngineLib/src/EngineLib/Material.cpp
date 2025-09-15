#include <CoreLib\FormatUtils.h>

#include "EngineLib\ResourceManager.h"
#include "EngineLib\Shader.h"
#include "EngineLib\Material.h"

namespace EngineCore {

	unsigned int Material::m_maxTextureUnits = 0;

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
		auto* rm = ResourceManager::GetInstance();

		for (const auto& [name, value] : m_boolParams) {
			shader->SetBool(name, value);
		}

		for (const auto& [name, value] : m_intParams) {
			shader->SetInt(name, value);
		}

		for (const auto& [name, value] : m_floatParams) {
			shader->SetFloat(name, value);
		}

		for (const auto& [name, value] : m_vector2Params) {
			shader->SetVector2(name, value);
		}

		for (const auto& [name, value] : m_vector3Params) {
			shader->SetVector3(name, value);
		}

		for (const auto& [name, value] : m_vector4Params) {
			shader->SetVector4(name, value);
		}

		for (const auto& [name, value] : m_matrixParams) {
			SetMatrixParam(shader, name, value);
		}

		unsigned int counter = 0;
		for (const auto& [name, value] : m_textureParams) {
			if (counter >= m_maxTextureUnits) break;
			if (value.value == ENGINE_INVALID_ID) continue;

			Texture2D* texture = rm->GetTexture2D(value);
			if (texture) {
				texture->CreateGL();
				texture->Bind(counter);
				counter++;
			}
		}
	}

	void Material::SetMatrixParam(Shader* shader, const std::string& name, Matrix m) const {
		int row = m.GetRowCount();
		int column = m.GetColCount();

		if (row < 2 || row > 4 || column < 2 || column > 4) {
			Log::Error("Material: Cant add matrix '{}x{}' to shader. matrix has illegal dimensions", row, column);
			return;
		}

		if (row == column) {
			switch (row) {
			case 2: shader->SetMatrix2(name, m.ToOpenGLData()); break;
			case 3: shader->SetMatrix3(name, m.ToOpenGLData()); break;
			case 4: shader->SetMatrix4(name, m.ToOpenGLData()); break;
			}
		}
		else {
			if (row == 2) {
				if (column == 3) {
					shader->SetMatrix2x3(name, m.ToOpenGLData());
				}
				else {
					shader->SetMatrix2x4(name, m.ToOpenGLData());
				}
			}
			else if (row == 3) {
				if (column == 2) {
					shader->SetMatrix3x2(name, m.ToOpenGLData());
				}
				else {
					shader->SetMatrix3x4(name, m.ToOpenGLData());
				}
			}
			else if (row == 4) {
				if (column == 2) {
					shader->SetMatrix4x2(name, m.ToOpenGLData());
				}
				else {
					shader->SetMatrix4x3(name, m.ToOpenGLData());
				}
			}
		}
	}

	void Material::SetIsTransparent(Texture2DID id) {
		auto* rm = ResourceManager::GetInstance();
		auto tex = rm->GetTexture2D(id);
		if (tex->GetNrChannels() > 3)
			m_isTransparent = true;
	}

	std::string Material::GetParamString() const {
		std::string pStr = FormatUtils::formatString("Material params:\n");
		pStr.append(FormatUtils::formatString(" - ShaderID: {}\n", m_shaderID.value));

		for (const auto& [name, value] : m_boolParams) {
			pStr.append(FormatUtils::formatString(" - bool: {}; {}\n", name, value));
		}

		for (const auto& [name, value] : m_intParams) {
			pStr.append(FormatUtils::formatString(" - int: {}; {}\n", name, value));
		}

		for (const auto& [name, value] : m_floatParams) {
			pStr.append(FormatUtils::formatString(" - float: {}; {}\n", name, value));
		}

		for (const auto& [name, value] : m_vector2Params) {
			pStr.append(FormatUtils::formatString(" - Vector2: {}; {}\n", name, value.ToString()));
		}

		for (const auto& [name, value] : m_vector3Params) {
			pStr.append(FormatUtils::formatString(" - Vector3: {}; {}\n", name, value.ToString()));
		}

		for (const auto& [name, value] : m_vector4Params) {
			pStr.append(FormatUtils::formatString(" - Vector4: {}; {}\n", name, value.ToString()));
		}

		for (const auto& [name, value] : m_matrixParams) {
			pStr.append(FormatUtils::formatString(" - Matrix: {};\n", name));
			pStr.append(value.ToString("   "));
		}

		for (const auto& [name, value] : m_textureParams) {
			pStr.append(FormatUtils::formatString(" - textureID: {}; {}\n", name, value.value));
		}

		return pStr;
	}

	ShaderID Material::GetShaderID() const {
		return m_shaderID;
	}

	bool Material::GetIsTransparent() const {
		return m_isTransparent;
	}

	std::vector<bool> Material::GetParamBool() const {
		std::vector<bool> values;
		values.resize(m_boolParams.size());

		for (auto& [name, value] : m_boolParams) {
			values.push_back(value);
		}
		return values;
	}

	std::vector<int> Material::GetParamInt() const {
		std::vector<int> values;
		values.resize(m_intParams.size());

		for (auto& [name, value] : m_intParams) {
			values.push_back(value);
		}
		return values;
	}

	std::vector<float> Material::GetParamFloat() const {
		std::vector<float> values;
		values.resize(m_floatParams.size());

		for (auto& [name, value] : m_floatParams) {
			values.push_back(value);
		}
		return values;
	}

	std::vector<Vector2> Material::GetParamVector2() const {
		std::vector<Vector2> values;
		values.resize(m_vector2Params.size());

		for (auto& [name, value] : m_vector2Params) {
			values.push_back(value);
		}
		return values;
	}

	std::vector<Vector3> Material::GetParamVector3() const {
		std::vector<Vector3> values;
		values.resize(m_vector3Params.size());

		for (auto& [name, value] : m_vector3Params) {
			values.push_back(value);
		}
		return values;
	}

	std::vector<Vector4> Material::GetParamVector4() const {
		std::vector<Vector4> values;
		values.resize(m_vector4Params.size());

		for (auto& [name, value] : m_vector4Params) {
			values.push_back(value);
		}
		return values;
	}

	std::vector<Matrix> Material::GetParamMatrix() const {
		std::vector<Matrix> values;
		values.resize(m_matrixParams.size());

		for (auto& [name, value] : m_matrixParams) {
			values.push_back(value);
		}
		return values;
	}

	std::vector<Texture2DID> Material::GetParamTexture2D() const {
		std::vector<Texture2DID> values;
		values.resize(m_textureParams.size());

		for (auto& [name, value] : m_textureParams) {
			values.push_back(value);
		}
		return values;
	}

}