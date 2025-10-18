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
	/**
	* @brief Holds a set of shader parameters (uniforms) and applies them to a shader.
	*
	* This object stores per-object or per-material values that are applied to a shader.
	* It can contain bools, ints, floats, vectors, matrices, and textures.
	*
	* Important:
	* - To use a ShaderBindObject during rendering, it must be assigned to a RenderCommand
	*   via the `shaderBindOverride` pointer.
	* - All relevant uniform values that the shader expects should be set in the SBO.
	*   Otherwise, leftover values from a previous render may persist and affect other objects.
	* - SBOs allow reusing the same shader with different parameters per object without rebinding
	*   the shader program.
	*/
	class ShaderBindObject {
	friend class Engine;
	public:
		ShaderBindObject() = default;
		~ShaderBindObject() = default;

		/**
		* @brief Retrieves a stored parameter by name.
		* @tparam T Type of the parameter.
		* @param name Name of the parameter.
		* @return The stored value.
		*/
		template<typename T>
		T GetParam(const std::string& name);

		/**
		* @brief Stores or updates a parameter value.
		* @tparam T Type of the parameter.
		* @param name Name of the parameter.
		* @param value Value to store.
		*/
		template<typename T>
		void SetParam(const std::string& name, const T& value);

		/**
		* @brief Binds all stored parameters to the given shader.
		* @param shader Shader to which parameters should be applied.
		*/
		void Bind(Shader* shader) const;

		/**
		* @brief Returns a human-readable string with all stored parameters.
		* @return Formatted string of parameter names and values.
		*/
		std::string GetParamString() const;
		/**
		* @brief Indicates whether any bound textures are transparent.
		* @return True if at least one texture has alpha channels.
		*/
		bool GetIsTransparent() const;

		// Convenience functions to retrieve parameters of specific types

		std::vector<bool> GetParamBools() const;
		std::vector<int> GetParamInts() const;
		std::vector<float> GetParamFloats() const;
		std::vector<Vector2> GetParamVector2s() const;
		std::vector<Vector3> GetParamVector3s() const;
		std::vector<Vector4> GetParamVector4s() const;
		std::vector<Matrix> GetParamMatrices() const;
		std::vector<Texture2DID> GetParamTexture2Ds() const;

	private:
		static inline unsigned int m_maxTextureUnits = 0;/**< Maximum number of texture units supported. */
		bool m_isTransparent = false; /**< True if any bound texture has transparency. */

		// Stored parameters grouped by type
		std::unordered_map<std::string, bool> m_boolParams;
		std::unordered_map<std::string, int> m_intParams;
		std::unordered_map<std::string, float> m_floatParams;
		std::unordered_map<std::string, Vector2> m_vector2Params;
		std::unordered_map<std::string, Vector3> m_vector3Params;
		std::unordered_map<std::string, Vector4> m_vector4Params;
		std::unordered_map<std::string, Matrix> m_matrixParams;
		std::unordered_map<std::string, Texture2DID> m_textureParams;

		/**
		* @brief Helper to bind a matrix parameter to the shader, handling dimensions.
		* @param shader Shader to bind the matrix to.
		* @param name Uniform name in the shader.
		* @param m Matrix to set.
		*/
		void SetMatrixParam(Shader* shader, const std::string& name, Matrix m) const;

		/**
		* @brief Checks a texture for transparency and updates m_isTransparent.
		* @param id Texture ID to inspect.
		*/
		void SetIsTransparent(Texture2DID id);

		/**
		* @brief Helper to convert a map of parameters to a vector of values.
		* @tparam T Parameter type.
		* @param map Map containing the parameters.
		* @return Vector of all values in the map.
		*/
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