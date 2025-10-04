#pragma once

/*
* @brief is a namespace that contains the names of the attributes. is only used the make life easyer when using attributes because those values can be used instead of the realnames
*/
namespace Attribute {

	/*
	* @brief Layouts the elements inside of a container (major, minor)
	* @param valueHor:, valueVer: start, center, end, stretch, space-evenly, space-around
	*/
	constexpr const char* layout = "layout";
	/*
	* @brief Major Layouting of elements inside of a container
	* @param start, center, end, stretch, space-evenly, space-around
	*/
	constexpr const char* layoutMajor = "layoutMajor";
	/*
	* @brief Minor layout of elements inside a container
	* @param start, center, end, stretch
	*/
	constexpr const char* layoutMinor = "layoutMinor";
	/*
	* @brief Minor layout aligment with other elements
	* @param start, center, end, stretch
	*/
	constexpr const char* layoutItem = "layoutItem";
	/*
	* @brief Direction in which the elements will arranged
	* @param row, column
	*/
	constexpr const char* layoutDirection = "layoutDirection";
	/*
	* @brief sets element wrapping
	* @param none, wrap
	*/
	constexpr const char* layoutWrap = "layoutWrap";
	/*
	* @brief Visibility of the element and its childs, still effects layouting
	* @param visible, hidden
	*/
	constexpr const char* visibility = "visibility";
	/*
	* @brief Layout type of the element. If set to 'none', the element will not be displayed and has no effect on layout calculations
	* @param flex, grid, none
	*/
	constexpr const char* layoutType = "layoutType";
	/*
	* @brief Sets the overflow of this element
	* @param hidden, none
	*/
	constexpr const char* overflow = "overflow";
	/*
	* @brief rotation (x, y, z) of this element
	* @param value all
	* @param valueX:, valueY:, valueZ: number
	*/
	constexpr const char* rotation = "rotation";
	/*
	* @brief rotation (x) of this element
	* @param number
	*/
	constexpr const char* rotationX = "rotationX";
	/*
	* @brief rotation (y) of this element
	* @param number
	*/
	constexpr const char* rotationY = "rotationY";
	/*
	* @brief rotation (z) of this element
	* @param number
	*/
	constexpr const char* rotationZ = "rotationZ";
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
	constexpr const char* margin = "margin";
	constexpr const char* marginTop = "marginTop";
	constexpr const char* marginBottom = "marginBottom";
	constexpr const char* marginLeft = "marginLeft";
	constexpr const char* marginRight = "marginRight";

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
	* @brief Sets the size of the border for this element
	* @param value all
	* @param valueTop:, valueRight:, valueBottom:, valueLeft: number
	*/
	constexpr const char* borderSize = "borderSize";
	/*
	* @brief Sets the width of the border for this element
	* @param number
	*/
	constexpr const char* borderWidth = "borderWidth";
	/*
	* @brief Sets the top width of the border for this element
	* @param number
	*/
	constexpr const char* borderTop= "borderTop";
	/*
	* @brief Sets the left width of the border for this element
	* @param number
	*/
	constexpr const char* borderLeft = "borderLeft";
	/*
	* @brief Sets the bottom width of the border for this element
	* @param number
	*/
	constexpr const char* borderBottom = "borderBottom";
	/*
	* @brief Sets the right width of the border for this element
	* @param number
	*/
	constexpr const char* borderRight = "borderRight";
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


/*

CSS:
zOrder
position relative, absolute
top
left
bottom
right

*/