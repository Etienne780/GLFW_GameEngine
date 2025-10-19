#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>


#include "UITypes.h"

namespace EngineCore::UI {

	class ElementBase;

	/**
	* @brief Represents a single style attribute used in the UI styling system.
	*
	* Each StyleAttribute defines how a specific style property (e.g., "width", "layout")
	* should be parsed, validated, and converted into a typed StyleValue.
	* Attributes may also be composite — i.e., they map one style property
	* (like "layout") to multiple underlying properties (like "layoutMajor" and "layoutMinor").
	*/
	class StyleAttribute {
	public:
		/**
		* @brief Default constructor initializes the attribute as undefined.
		*/
		StyleAttribute() = default;
		/**
		* @brief Creates a new StyleAttribute.
		* @param name Unique name of the attribute (e.g. "width", "layout").
		* @param description Short description of the attribute purpose.
		* @param fallbackStr Fallback string value used if parsing fails.
		* @param m_inputs List of valid input strings (e.g. "center", "flex", etc.).
		* @param p Parser function that converts a string input into a StyleValue.
		*/
		StyleAttribute(std::string name, std::string description, std::string fallbackStr, std::vector<std::string> m_inputs,
			std::function<StyleValue(const StyleAttribute*, const std::string&)> p);

		/**
		* @brief Parses a string and returns the corresponding StyleValue.
		* @param value The input string to be parsed.
		* @return Parsed StyleValue; returns an empty StyleValue if no parser is defined.
		*/
		StyleValue GetValue(const std::string& value) const;
		/**
		* @brief Attempts to parse a StyleValue safely.
		* @param value The string value to parse.
		* @param outValue Reference where the parsed value will be stored.
		* @return True if parsing succeeded, false otherwise.
		*/
		bool TryGetValue(const std::string& value, StyleValue& outValue) const;

		std::unordered_map<std::string, std::string> ParseCompositeValues(const std::string& input) const;

		/**
		* @brief Returns the name of the attribute.
		*/
		const std::string& GetName() const;
		/**
		* @brief Returns the description text of the attribute.
		*/
		const std::string& GetDesc() const;
		/**
		* @brief Returns the list of valid input options for this attribute.
		*/
		const std::vector<std::string>& GetInputs() const;
		/**
		* @brief Returns the fallback string value used when parsing fails.
		*/
		std::string GetFallbackStr() const;
		/**
		* @brief Checks whether this attribute is a composite type.
		* @return True if this attribute maps to multiple sub-attributes.
		*/
		bool IsComposite() const;
		/**
		* @brief Returns the list of attributes this composite attribute maps to.
		*
		* This provides access to all sub-attributes that belong to a composite
		* attribute such as "layout" -> {"layoutMajor", "layoutMinor"}.
		* In debug builds, a warning is logged if the attribute is not composite.
		*
		* @return Reference to a vector of mapped attribute names (may be empty).
		*/
		const std::vector<std::string>& GetMappedAttributes() const;
		
	private:
		std::string m_name = "UNKNOWN";
		std::string m_description = "-";
		std::vector<std::string> m_inputs;
		std::string m_fallbackStr;
		std::function<StyleValue(const StyleAttribute*, const std::string&)> m_parser = nullptr;

		bool m_isComposite = false;
		std::vector<std::string> m_mappedAttributes;

	public:
		/**
		* @brief Retrieves a StyleValue for a given attribute by name.
		* @param name Name of the attribute.
		* @param input The string value to parse.
		* @return Parsed StyleValue.
		*/
		static StyleValue GetAttributeValue(const std::string& name, const std::string& input);
		/**
		* @brief Safely attempts to get a StyleValue by name.
		* @param name Name of the attribute.
		* @param input The string value to parse.
		* @param outValue Output reference to store the parsed StyleValue.
		* @return True if the attribute exists and parsing succeeded.
		*/
		static bool TryGetAttributeValue(const std::string& name, const std::string& input, StyleValue& outValue);
		/**
		* @brief Returns a registered attribute by name.
		* Logs an error and returns a dummy attribute if it does not exist.
		*/
		static const StyleAttribute& GetAttribute(const std::string& name);
		/**
		* @brief Attempts to find a registered attribute by name.
		* @param name Attribute name.
		* @param outValue Output reference to the found attribute.
		* @return True if the attribute exists.
		*/
		static const bool TryGetAttribute(const std::string& name, StyleAttribute& outValue);
		/**
		* @brief Returns a reference to all registered style attributes.
		*/
		static const std::unordered_map<std::string, StyleAttribute>& GetAllAttributes();
		/**
		* @brief Registers a single attribute in the global registry.
		* Logs a warning if another attribute with the same name already exists.
		*/
		static void RegisterAttribute(const StyleAttribute& attribute);

		/**
		* @brief Registers a composite attribute that expands into multiple attributes. (Composite attributes dosent have to be Reg in UIElements)
		*
		* Example:
		* @code
		* StyleAttribute::RegisterCompositeAttribute(LayoutAtt, "layoutMajor", "layoutMinor");
		* @endcode
		*
		* This makes the "layout" attribute automatically map its values to
		* "layoutMajor" and "layoutMinor" based on the number of tokens.
		*
		* @tparam Args Names of the mapped attributes.
		* @param attribute The base composite attribute.
		* @param args Names of the sub-attributes to map to.
		*/
		template<typename... Args>
		static void RegisterCompositeAttribute(const StyleAttribute& attribute, Args... args)
		{
			StyleAttribute composite = attribute;
			composite.m_isComposite = true;
			composite.m_mappedAttributes = { args... };

			m_registeredStyleAttributes[composite.GetName()] = composite;
		}

	private:
		// Global registry of all known StyleAttributes.
		static inline std::unordered_map<std::string, StyleAttribute> m_registeredStyleAttributes;
	};

}