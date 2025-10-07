#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <CoreLib/Log.h>
#include <CoreLib/File.h>

#include "UITypes.h"
#include "StyleAttribute.h"
#include "../EngineTypes.h"

namespace EngineCore::UI {

	class Style {
	public:
		Style() = default;
		Style(const std::string& name);

		/**
		* @brief Creates a new style with the given name and automatically extends from BaseStyle
		* @param name The name of the new style
		* @return A shared pointer to the newly created Style object
		*/
		static std::shared_ptr<Style> Create(const std::string& name);

		/**
		* @brief Creates a new style with the given name and extends from the provided style
		* @param name The name of the new style
		* @param style A shared pointer to an existing Style object to extend from
		* @return A shared pointer to the newly created Style object
		*/
		static std::shared_ptr<Style> Create(const std::string& name, std::shared_ptr<Style> style);

		/**
		* @brief Extends the current style by adding another style to its list of extended styles
		* @param style A shared pointer to the Style object to extend from
		*/
		void Extend(std::shared_ptr<Style> style);

		/**
		* @brief Sets an attribute for the normal state of the style
		* @param name The name of the attribute
		* @param value The value to set for the attribute
		*/
		void Set(const char* name, std::string value);

		/**
		* @brief Sets an attribute for the normal state of the style
		* @param name The name of the attribute
		* @param value The value to set for the attribute
		*/
		void Set(std::string name, std::string value);

		/**
		* @brief Sets an attribute for a specific state of the style
		* @param state The state for which the attribute should be set
		* @param name The name of the attribute
		* @param value The value to set for the attribute
		*/
		void Set(State state, const char* name, std::string value);


		/**
		* @brief Retrieves the value of an attribute for the normal state
		* @param name The name of the attribute
		* @return The value of the attribute, or "-" if not found
		*/
		std::string Get(const char* name) const;

		/**
		* @brief Retrieves the value of an attribute for a specific state
		* @param state The state for which the attribute should be retrieved
		* @param name The name of the attribute
		* @return The value of the attribute, or "-" if not found
		*/
		std::string Get(State state, const char* name) const;

		/**
		* @brief Returns the name of the style
		* @return A constant reference to the style's name
		*/
		const std::string& GetName() const;

		/**
		* @brief Retrieves all attributes for all states
		* @return A constant reference to a map of states to attribute maps
		*/
		const std::unordered_map<State, std::unordered_map<std::string, std::string>>& GetAll() const;

		/**
		* @brief Retrieves all attributes for a specific state
		* @param state The state for which to retrieve all attributes
		* @return A constant reference to a map of attribute names to values for the given state
		*/
		const std::unordered_map<std::string, std::string>& GetAllState(State state) const;

	private:
        std::string m_name = "UNKNOWN";
        std::unordered_map<State, std::unordered_map<std::string, std::string>> m_attributes;// state, attName, attValue
		std::vector<std::shared_ptr<Style>> m_extendedStyles;
		mutable bool m_newStyleAdded = true;
		mutable std::unique_ptr<Style> m_cachedStyle;

		/**
		* @brief Generates a cached style by combining all extended styles and this style's attributes
		*
		* This function applies attributes from all extended styles in order, then applies this style's
		* own attributes last, updating the cached style for faster subsequent lookups.
		*/
		void GenerateCachedStyle() const;

	};

}