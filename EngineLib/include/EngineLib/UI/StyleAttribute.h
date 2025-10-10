#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>


#include "UITypes.h"

namespace EngineCore::UI {

	class ElementBase;

	/*
	* @brief is collection of all style attributes or an object of an style
	*/
	class StyleAttribute {
	public:
		StyleAttribute() = default;
		/*
		* @brief inputs are the possible inputs a Attribute can have, as an example (center, flex,...)
		*/
		StyleAttribute(std::string name, std::string description, std::string fallbackStr, std::vector<std::string> m_inputs,
			std::function<StyleValue(const ElementBase& element, const StyleAttribute*, const std::string&)> p);

		StyleValue GetValue(const ElementBase& element, const std::string& value) const {
			if (m_parser)
				return m_parser(element, this, value);

			Log::Error("StyleAttribute: Cannot get value of Attribute '{}', no parser defined!", m_name);
			return StyleValue{};
		}

		const std::string& GetName() const { return m_name; }
		const std::string& GetDesc() const { return m_description; }
		const std::vector<std::string>& GetInputs() const { return m_inputs; }
		std::string GetFallbackStr() const { return m_fallbackStr; }
		
	private:
		std::string m_name = "UNKNOWN";
		std::string m_description = "-";
		std::vector<std::string> m_inputs;
		std::string m_fallbackStr;
		std::function<StyleValue(const ElementBase& element, const StyleAttribute*, const std::string&)> m_parser = nullptr;

	public:
		static StyleValue GetAttributeValue(const std::string& name, const ElementBase& element, const std::string& valueStr);
		static const StyleAttribute& GetAttribute(const std::string& name);
		static const std::unordered_map<std::string, StyleAttribute>& GetAllAttributes();
		static void RegisterAttribute(const StyleAttribute& attribute);

	private:
		static inline std::unordered_map<std::string, StyleAttribute> m_registeredStyleAttributes;
	};

}