#pragma once

/*
* @brief is a namespace that contains the names of the attributes. is only used the make life easyer when using attributes because those values can be used instead of the realnames
*/
namespace Attribute {

	/*
	* @brief Layouts the elements inside of a container
	* @param valueHor:, valueVer: start, center, end, stretch
	*/
	constexpr const char* layout = "layout";
	/*
	* @brief Horizontal Layouting of elements inside of a container
	* @param start, center, end, stretch
	*/
	constexpr const char* layoutHor = "layoutHor";
	/*
	* @brief Vertical Layouting of elements inside of a container
	* @param start, center, end, stretch
	*/
	constexpr const char* layoutVer = "layoutVer";
	/*
	* @brief Layouts the whole content inside of a container
	* @param valueHor:, valueVer: start, center, end, space-evenly, space-around
	*/
	constexpr const char* layoutContent = "layoutContent";
	/*
	* @brief Horizontal Layouting of the whole content inside of a container
	* @param start, center, end, space-evenly, space-around
	*/
	constexpr const char* layoutContentHor = "layoutContentHor";
	/*
	* @brief Vertical Layouting of the whole content inside of a container
	* @param start, center, end, space-evenly, space-around
	*/
	constexpr const char* layoutContentVer = "layoutContentVer";
	/*
	* @brief Direction in which the elements will arranged
	* @param row, column, row-start, row-end, column-start, column-end
	*/
	constexpr const char* layoutDirection = "layoutDirection";
	/*
	* @brief sets element wrapping
	* @param no-wrap, wrap
	*/
	constexpr const char* layoutWrap = "layoutWrap";
	/*
	* @brief Visibility of the element and its childs, still effects layouting
	* @param visible, hidden
	*/
	constexpr const char* visibility = "visibility";
	/*
	* @brief display mode of the element and its childs, if none has no effect in layouting
	* @param display, none
	*/
	constexpr const char* display = "display";
	/*
	* @brief offset the position of this element, should not be used to often
	* @param valueX:, valueY: number
	*/
	// constexpr const char* position = "position";
	/*
	* @brief offset the X-position of this element, should not be used to often
	* @param number
	*/
	// constexpr const char* positionX = "positionX";
	/*
	* @brief offset the Y-position of this element, should not be used to often
	* @param number
	*/
	// constexpr const char* positionY = "positionY";
	/*
	* @brief rotation (z) of this element
	* @param number
	*/
	// constexpr const char* rotation = "rotation";
	/*
	* @brief width of this element
	* @param number
	*/
	constexpr const char* width = "width";
	/*
	* @brief height of this element
	* @param number
	*/
	constexpr const char* height = "height";
	/*
	* @brief margin of this element
	* @param value all
	* @param valueTop:, valueRight:, valueBottom:, valueLeft: number
	*/
	constexpr const char* margin = "padding";
	constexpr const char* marginTop = "paddingTop";
	constexpr const char* marginBottom = "paddingBottom";
	constexpr const char* marginLeft = "paddingLeft";
	constexpr const char* marginRight = "paddingRight";

	/*
	* @brief padding of this element
	* @param value all
	* @param valueTop:, valueRight:, valueBottom:, valueLeft: number
	*/
	constexpr const char* padding = "padding";
	constexpr const char* paddingTop = "paddingTop";
	constexpr const char* paddingBottom = "paddingBottom";
	constexpr const char* paddingLeft = "paddingLeft";
	constexpr const char* paddingRight = "paddingRight";
	/*
	* @brief Sets the overflow of this element
	* @param hidden, none
	*/
	constexpr const char* overflow = "overflow";
	/*
	* @brief Sets the background color of this element
	* @param color (#ff00ff, #00ff00ff)
	*/
	constexpr const char* backgroundColor = "backgroundColor";
	/*
	* @brief Sets the text color of this element
	* @param color (#ff00ff, #00ff00ff)
	*/
	constexpr const char* textColor = "textColor";
	/*
	* @brief Sets the border color of this element
	* @param color (#ff00ff, #00ff00ff)
	*/
	constexpr const char* borderColor = "borderColor";
	/*
	* @brief Sets the border width of this element
	* @param number
	*/
	constexpr const char* borderWidth = "borderWidth";
	/*
	* @brief Sets the border radius of this element (top-left, top-right, bottom-right, bottom-left)
	* @param number, numbers
	*/
	constexpr const char* borderRadius = "borderRadius";
	/*
	* @brief Sets the transition duration time between styles
	* @param number in sekunds
	*/
	constexpr const char* duration = "duration";

}