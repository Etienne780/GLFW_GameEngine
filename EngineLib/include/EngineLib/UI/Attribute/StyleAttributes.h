#pragma once
#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "../AttributeHelper.h"
#include "../StyleAttribute.h"

/*
* @file StyleAttributes.h
* @brief Contains attributes that control the visual styling of UI elements.
*
* These attributes define colors, borders, padding, margins, and other
* appearance-related properties.
* Examples: backgroundColor, borderColor, borderWidth, borderRadius, margin, padding, etc.
*/

namespace EngineCore::UI::Init {
	const bool regStyleAtt();
}

namespace {

	using namespace EngineCore::UI;

    // ----------- tranformations ------------
    // size
	extern const StyleAttribute Width;
	extern const StyleAttribute Height;

    // rotation
    extern const StyleAttribute Rotation;
    extern const StyleAttribute RotationX;
    extern const StyleAttribute RotationY;
    extern const StyleAttribute RotationZ;

    // Margin
    extern const StyleAttribute Margin;
    extern const StyleAttribute MarginTop;
    extern const StyleAttribute MarginRight;
    extern const StyleAttribute MarginBottom;
    extern const StyleAttribute MarginLeft;

    // Padding
    extern const StyleAttribute Padding;
    extern const StyleAttribute PaddingTop;
    extern const StyleAttribute PaddingRight;
    extern const StyleAttribute PaddingBottom;
    extern const StyleAttribute PaddingLeft;


    extern const StyleAttribute BackgroundColor;
    extern const StyleAttribute TextColor;
    extern const StyleAttribute BorderColor;

    //Border size
    extern const StyleAttribute BorderSize;
    extern const StyleAttribute BorderTop;
    extern const StyleAttribute BorderRight;
    extern const StyleAttribute BorderLeft;
    extern const StyleAttribute BorderBottom;

    extern const StyleAttribute BorderRadius;
    extern const StyleAttribute BorderType;
    extern const StyleAttribute Duration;

}