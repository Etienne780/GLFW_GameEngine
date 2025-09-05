namespace EngineCore {
	
	template<typename T>
	void Material::SetParam(const std::string & name, const T & value) {
        static_assert(
            std::is_same<T, bool>::value ||
            std::is_same<T, int>::value ||
            std::is_same<T, float>::value ||
            std::is_same<T, Vector2>::value ||
            std::is_same<T, Vector3>::value ||
            std::is_same<T, Vector4>::value ||
            std::is_same<T, Matrix>::value ||
            std::is_same<T, Asset_Texture2DID>::value,
            "T is not a valid param type. Must be bool, int, float, Vector2, Vector3, Vector4, Matrix, or Asset_Texture2DID"
            );

        // save value in the right map
        if constexpr (std::is_same<T, bool>::value) {
            m_boolParams[name] = value;
        }
        else if constexpr (std::is_same<T, int>::value) {
            m_intParams[name] = value;
        }
        else if constexpr (std::is_same<T, float>::value) {
            m_floatParams[name] = value;
        }
        else if constexpr (std::is_same<T, Vector2>::value) {
            m_vector2Params[name] = value;
        }
        else if constexpr (std::is_same<T, Vector3>::value) {
            m_vector3Params[name] = value;
        }
        else if constexpr (std::is_same<T, Vector4>::value) {
            m_vector4Params[name] = value;
        }
        else if constexpr (std::is_same<T, Matrix>::value) {
            m_matrixParams[name] = value;
        }
        else if constexpr (std::is_same<T, Asset_Texture2DID>::value) {
            m_textureParams[name] = value;
        }
    }

}