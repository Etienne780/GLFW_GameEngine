#include <CoreLib/FormatUtils.h>

#include "EngineLib/ResourceManager.h"
#include "EngineLib/Shader.h"
#include "EngineLib/ShaderBindObject.h"

namespace EngineCore {

	void ShaderBindObject::Bind(Shader* shader) const {
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

	std::string ShaderBindObject::GetParamString() const {
		std::string pStr = FormatUtils::formatString("Params:\n");

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

	bool ShaderBindObject::GetIsTransparent() const {
		return m_isTransparent;
	}

	std::vector<bool> ShaderBindObject::GetParamBools() const {
		return GetParamsImpl<bool>(m_boolParams);
	}

	std::vector<int> ShaderBindObject::GetParamInts() const {
		return GetParamsImpl<int>(m_intParams);
	}

	std::vector<float> ShaderBindObject::GetParamFloats() const {
		return GetParamsImpl<float>(m_floatParams);
	}

	std::vector<Vector2> ShaderBindObject::GetParamVector2s() const {
		return GetParamsImpl<Vector2>(m_vector2Params);
	}

	std::vector<Vector3> ShaderBindObject::GetParamVector3s() const {
		return GetParamsImpl<Vector3>(m_vector3Params);
	}

	std::vector<Vector4> ShaderBindObject::GetParamVector4s() const {
		return GetParamsImpl<Vector4>(m_vector4Params);
	}

	std::vector<Matrix> ShaderBindObject::GetParamMatrices() const {
		return GetParamsImpl<Matrix>(m_matrixParams);
	}

	std::vector<Texture2DID> ShaderBindObject::GetParamTexture2Ds() const {
		return GetParamsImpl<Texture2DID>(m_textureParams);
	}

	void ShaderBindObject::SetMatrixParam(Shader* shader, const std::string& name, Matrix m) const {
		int row = m.GetRowCount();
		int column = m.GetColCount();

		if (row < 2 || row > 4 || column < 2 || column > 4) {
			Log::Error("ShaderBindObject: Cant add matrix '{}x{}' to shader. matrix has illegal dimensions", row, column);
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

	void ShaderBindObject::SetIsTransparent(Texture2DID id) {
		auto* rm = ResourceManager::GetInstance();
		auto tex = rm->GetTexture2D(id);
		if (tex->GetNrChannels() > 3)
			m_isTransparent = true;
	}

}