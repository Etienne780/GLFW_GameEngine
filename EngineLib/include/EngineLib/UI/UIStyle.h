#pragma once
#include <any>
#include <unordered_map>
#include <string>

#include "../EngineTypes.h"

namespace EngineCore {
	
	namespace StylingAttribute {
		/*
		* @brief Layouts the elements inside of a container
		* @param valueHor:, valueVer: start, center, end, stretch 
		*/
		const char* layout = "engine_layout";
		/*
		* @brief Horizontal Layouting of elements inside of a container
		* @param start, center, end, stretch
		*/
		const char* layoutHor = "engine_layoutHor";
		/*
		* @brief Vertical Layouting of elements inside of a container
		* @param start, center, end, stretch
		*/
		const char* layoutVer = "engine_layoutVer";
		/*
		* @brief Layouts the whole content inside of a container
		* @param valueHor:, valueVer: start, center, end, space-evenly, space-around
		*/
		const char* layoutContent = "engine_layoutContent";
		/*
		* @brief Horizontal Layouting of the whole content inside of a container
		* @param start, center, end, space-evenly, space-around
		*/
		const char* layoutContentHor = "engine_layoutContentHor";
		/*
		* @brief Vertical Layouting of the whole content inside of a container
		* @param start, center, end, space-evenly, space-around
		*/
		const char* layoutContentVer = "engine_layoutContentVer";
		/*
		* @brief Direction in which the elements will arranged
		* @param row, column, row-start, row-end, column-start, column-end
		*/
		const char* layoutDirection = "engine_layoutDirection";
		/*
		* @brief sets element wrapping
		* @param no-wrap, wrap
		*/
		const char* layoutWrap = "engine_layoutWrap";
		/*
		* @brief offset the position of this element, should not be used to often
		* @param valueX:, valueY: number
		*/
		const char* position = "engine_position";
		/*
		* @brief offset the X-position of this element, should not be used to often
		* @param number
		*/
		const char* positionX = "engine_positionX";
		/*
		* @brief offset the Y-position of this element, should not be used to often
		* @param number
		*/
		const char* positionY = "engine_positionY";
		/*
		* @brief rotation (z) of this element
		* @param number
		*/
		const char* rotation = "engine_rotation";
		/*
		* @brief margin of this element
		* @param value all
		* @param valueTop:, valueRight:, valueBottom:, valueLeft: number
		*/
		const char* margin = "engine_padding";
		const char* marginTop = "engine_paddingTop";
		const char* marginBottom = "engine_paddingBottom";
		const char* marginLeft = "engine_paddingLeft";
		const char* marginRight = "engine_paddingRight";

		/*
		* @brief padding of this element
		* @param value all
		* @param valueTop:, valueRight:, valueBottom:, valueLeft: number
		*/
		const char* padding = "engine_padding";
		const char* paddingTop = "engine_paddingTop";
		const char* paddingBottom = "engine_paddingBottom";
		const char* paddingLeft = "engine_paddingLeft";
		const char* paddingRight = "engine_paddingRight";
		/*
		* @brief Sets the overflow of this element
		* @param hidden, none
		*/
		const char* overflow = "engine_overflow";
		/*
		* @brief Sets the color of this element
		* @param hex
		*/
		const char* color = "engine_color";
		/*
		* @brief Sets the borderRadius of this element
		* @param number
		*/
		const char* borderRadius = "engine_borderRadius";
		/*
		* @brief Sets the transition duration time between styles
		* @param number in sekunds
		*/
		const char* duration = "engine_duration";
	}

	class UIStyle {
	public:
		UIStyle() = default;

		void Extend(const UIStyle& style);
		void Set(const char* name, std::any value);

		template<typename T>
		T Get(const char* name) const {
			auto it = m_attributes.find(name);
			if (it != m_attributes.end()) {
				try {
					return std::any_cast<T>(it->second);
				}
				catch (const std::bad_any_cast&) {
					return ENGINE_STYLING_BAD_TYPE;
				}
			}

			
			return ENGINE_STYLING_BAD_TYPE;
		}

		const std::unordered_map<std::string, std::any>& GetAll() const;
	
	private:
		std::unordered_map<std::string, std::any> m_attributes;
	};

}

/*
styling attribute:
- layout von childs =  horizontal start, center, end, stretch vertical start, center, end, stretch
- Position = x,y
- rotation = z
- overflow = hidden, unset
- color = hex
- radius = 2
- custome styling attribute
*/
