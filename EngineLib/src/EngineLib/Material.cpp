#include "CoreLib\FormatUtils.h"
#include "CoreLib\Log.h"
#include "EngineLib\Material.h"

namespace EngineCore {

	Material::Material(unsigned int shaderID) 
		: m_shaderID(shaderID){
	}

	void Material::ApplyToShader() const {
		if (m_shaderID == ENGINE_INVALID_ID) {
			Log::
		}


		for (const auto& [name, value] : m_boolParams) {

		}

		for (const auto& [name, value] : m_intParams) {

		}

		for (const auto& [name, value] : m_floatParams) {

		}

		for (const auto& [name, value] : m_vector2Params) {

		}

		for (const auto& [name, value] : m_vector3Params) {

		}

		for (const auto& [name, value] : m_vector4Params) {

		}

		for (const auto& [name, value] : m_matrixParams) {

		}

		for (const auto& [name, value] : m_textureParams) {

		}
	}

	std::string Material::GetParamString() const {
		std::string pStr = FormatUtils::formatString("Material params:\n");
		pStr.append(FormatUtils::formatString(" - ShaderID: {}\n", m_shaderID));

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
			pStr.append(FormatUtils::formatString(" - textureID: {}; {}\n", name, value));
		}

		return pStr;
	}

}