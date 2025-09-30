#pragma once
#include <string>
#include <vector>

#include "EngineLib/UI/StyleAttribute.h"

class Vector4;
namespace EngineCore::UI {
	
	class AttributeHelper {
	public:
		enum class NumberType {
			SIZE,
			TIME
		};

		/**
		* @brief Splits a string of attribute values separated by spaces. Multiple consecutive spaces are ignored.
		* @param input The input string containing one or more values
		* @return A vector containing each individual value as a string
		*/
		static std::vector<std::string> GetValues(const std::string& input);

		/**
		* @brief Checks if a unit is valid (size or time)
		* @param unit The unit string to check
		* @return True if the unit is valid, false otherwise
		*/
		static bool IsUnit(const std::string& unit);

		/**
		* @brief Checks if a unit is a valid size unit
		* @param unit The unit string to check
		* @return True if the unit is a valid size unit, false otherwise
		*/
		static bool IsSizeUnit(const std::string& unit);

		/**
		* @brief Checks if a unit is a valid time unit
		* @param unit The unit string to check
		* @return True if the unit is a valid time unit, false otherwise
		*/
		static bool IsTimeUnit(const std::string& unit);

		static bool IsUnitType(NumberType type, const std::string& unit);

		/**
		* @brief Checks whether all given units are valid units (size or time)
		* @tparam Args Variadic template parameter for unit strings
		* @param args The units to check
		* @return True if all units are valid, false otherwise
		*/
		template<typename... Args>
		static bool IsUnit(const Args&... args) {
			return (IsUnit(args) && ...);
		}

		/**
		* @brief Checks whether all given units are valid size units
		* @tparam Args Variadic template parameter for unit strings
		* @param args The units to check
		* @return True if all units are valid size units, false otherwise
		*/
		template<typename... Args>
		static bool IsSizeUnit(const Args&... args) {
			return (IsSizeUnit(args) && ...);
		}

		/**
		* @brief Checks whether all given units are valid time units
		* @tparam Args Variadic template parameter for unit strings
		* @param args The units to check
		* @return True if all units are valid time units, false otherwise
		*/
		template<typename... Args>
		static bool IsTimeUnit(const Args&... args) {
			return (IsTimeUnit(args) && ...);
		}

		template<typename... Args>
		static bool IsUnitType(NumberType type, const Args&... args) {
			return (IsUnitType(type, args) && ...);
		}

		/**
		* @brief Retrieves all supported units (size + time)
		* @return A constant reference to a vector of all unit strings
		*/
		static const std::vector<std::string>& GetAllUnits();

		/**
		* @brief Attempts to extract a unit from a given input string
		* @param input The input string containing a potential unit
		* @param out Output parameter to receive the extracted unit
		* @return True if a valid unit was found, false otherwise
		*/
		static bool TryGetUnit(const std::string& input, std::string& out);

		/**
		* @brief Attempts to extract a numeric value and its unit from an input string
		* @param element Reference to the element providing context for size calculations
		* @param input The input string containing number + unit
		* @param outValue Output parameter to receive the numeric value
		* @param outUnit Output parameter to receive the extracted unit
		* @return True if a valid number and unit were parsed, false otherwise
		*/
		static bool TryGetNumber(const ElementBase& element, const std::string& input, float& outValue, std::string& outUnit);

		/**
		* @brief Checks whether a value exists in a given list
		* @tparam T The type of elements in the list
		* @param list The list to search in
		* @param value The value to check
		* @return True if the value exists in the list, false otherwise
		*/
		template<typename T>
		static bool ListContains(const std::vector<T>& list, const T& value) {
			auto it = std::find(list.begin(), list.end(), value);
			return it != list.end();
		}

		/**
		* @brief Creates a simple string attribute
		* @param name The name of the attribute
		* @param description A description of the attribute
		* @param inputs Valid input values
		* @param defaultValue The default value
		* @return A StyleAttribute representing the string attribute
		*/
		static StyleAttribute MakeSimpleStringAttribute(
			const char* name,
			const char* description,
			std::vector<std::string> inputs,
			const std::string& defaultValue);

		/**
		* @brief Creates a simple number attribute
		* @param name The name of the attribute
		* @param description A description of the attribute
		* @param defaultValue The default value
		* @param type The type of number (size or time)
		* @return A StyleAttribute representing the number attribute
		*/
		static StyleAttribute MakeSimpleNumberAttribute(
			const char* name,
			const char* description,
			float defaultValue,
			NumberType type);

		/**
		* @brief Creates a simple color attribute
		* @param name The name of the attribute
		* @param description A description of the attribute
		* @param inputs Valid input formats
		* @param defaultValue The default color value
		* @return A StyleAttribute representing the color attribute
		*/
		static StyleAttribute MakeSimpleColorAttribute(
			const char* name,
			const char* description,
			std::vector<std::string> inputs,
			Vector4 defaultValue);

		/**
		* @brief Creates a multi-string attribute with multiple allowed input counts
		* @param name The name of the attribute
		* @param description A description of the attribute
		* @param numberOfInputs Allowed numbers of inputs
		* @param inputs Valid input values
		* @param defaultValues The default values
		* @return A StyleAttribute representing the multi-string attribute
		*/
		static StyleAttribute MakeMultiStringAttribute(
			const char* name,
			const char* description,
			std::vector<size_t> numberOfInputs,
			std::vector<std::string> inputs,
			std::vector<std::string> defaultValues);
		
		/**
		* @brief Creates a multi-number attribute that supports 1 to 4 numeric inputs.
		*
		* The attribute can accept multiple numbers separated by spaces. The values are mapped to vectors
		* according to the number of inputs:
		*
		* - 1 value: all components receive the same value.
		* - 2 values: the first value is applied to the first and second components, the second value to the third and fourth components.
		* - 3 values: the first value is applied to the first component, the second to the second, and the third to the third and fourth components.
		* - 4 values: each value is applied to the corresponding component in order.
		*
		* @param name The name of the attribute
		* @param description A description of the attribute
		* @param maxInput Maximum allowed number of numeric inputs (1-4)
		* @param defaultValues Default numeric values for the attribute; used to build the default vector according to maxInput
		* @param type The type of numbers expected (Size or Time)
		* @return A StyleAttribute representing the multi-number attribute
		*/
		static StyleAttribute MakeMultiNumberAttribute(
			const char* name,
			const char* description,
			size_t maxInput,
			std::vector<float> defaultValue,
			NumberType type);

	private:
		AttributeHelper() = delete;
		AttributeHelper(const AttributeHelper&) = delete;
		AttributeHelper(AttributeHelper&&) = delete;

		/*
		* Units:
		* px = pixel
		* %w = percent width of the parent element
		* %h = percent height of the parent element
		* vw = viewport width (referenc screen width)
		* vh = viewport height (referenc screen height)
		* s = seconds
		*/

		static inline const std::vector<std::string> m_sizeUnits = { "px", "%w", "%h", "vw", "vh" };
		static inline const std::vector<std::string> m_timeUnits = { "s", "ms" };
	};

}