#pragma once
#include <string>
#include <vector>

#include "EngineLib/UI/StyleAttribute.h"

class Vector4;
namespace EngineCore::UI {
	
	class AttributeHelper {
	public:
		/*
		* @brief Splits a string of attribute values separated by spaces.
		* Multiple consecutive spaces are ignored.
		* @param input: The input string containing one or more values.
		* @return A vector containing each individual value as a string.
		*/
		static std::vector<std::string> GetValues(const std::string& input);

		static bool TryGetUnit(const std::string & input, std::string& out);

		static bool TryGetNumber(const ElementBase& element, const std::string& input, float& out);

		template<typename T>
		static bool ListContains(const std::vector<T>& list, const T& value) {
			auto it = std::find(list.begin(), list.end(), value);
			return it != list.end();
		}

		static StyleAttribute MakeSimpleStringAttribute(
			const char* name,
			const char* description,
			std::vector<std::string> inputs,
			const std::string& defaultValue);

		static StyleAttribute MakeSimpleNumberAttribute(
			const char* name,
			const char* description,
			float defaultValue);

		static StyleAttribute MakeSimpleColorAttribute(
			const char* name,
			const char* description,
			std::vector<std::string> inputs,
			Vector4 defaultValue);

	private:
		AttributeHelper() = delete;
		AttributeHelper(const AttributeHelper&) = delete;
		AttributeHelper(AttributeHelper&&) = delete;
	};

}