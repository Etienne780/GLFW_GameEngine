#include <CoreLib/FormatUtils.h>
#include <CoreLib/Math/Vector4.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/AttributeHelper.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Attribute/StyleAttributes.h"

namespace {
    typedef AttributeHelper AHel;

    const StyleAttribute Width = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::width,
        "width of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute Height = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::height,
        "height of this element",
        0, AHel::NumberType::SIZE);

    #pragma region Rotation

    const StyleAttribute Rotation = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::rotation,
        "rotation (x, y, z) of this element",
        3, { 0, 0, 0 },
        AHel::NumberType::SIZE);

    const StyleAttribute RotationX = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::rotationX,
        "rotation (x) of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute RotationY = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::rotationY,
        "rotation (Y) of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute RotationZ = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::rotationZ,
        "rotation (Z) of this element",
        0, AHel::NumberType::SIZE);

    #pragma endregion


    #pragma region Margin

    const StyleAttribute Margin = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::margin,
        "margin of this element, order: top, right, bottom, left",
        4, 
        { 0, 0, 0, 0 },
        AHel::NumberType::SIZE);
    
    const StyleAttribute MarginTop = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::marginTop,
        "top margin of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute MarginBottom = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::marginBottom,
        "bottom margin of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute MarginLeft = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::marginLeft,
        "left margin of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute MarginRight = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::marginRight,
        "right margin of this element",
        0, AHel::NumberType::SIZE);

    #pragma endregion

    #pragma region Padding

    const StyleAttribute Padding = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::padding,
        "padding of this element, order: top, right, bottom, left",
        4,
        { 0, 0, 0, 0 },
        AHel::NumberType::SIZE);

    const StyleAttribute PaddingTop = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::paddingTop,
        "top padding of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute PaddingBottom = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::paddingBottom,
        "bottom padding of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute PaddingLeft = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::paddingLeft,
        "left padding of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute PaddingRight = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::paddingRight,
        "right padding of this element",
        0, AHel::NumberType::SIZE);

    #pragma endregion

    const StyleAttribute BackgroundColor = AttributeHelper::MakeSimpleColorAttribute(
        Attribute::backgroundColor,
        "Sets the background color of this element",
        { "#ff", "#ff00ff", "#00ff00ff" },
        Vector4(1,1,1,1));

    const StyleAttribute TextColor = AttributeHelper::MakeSimpleColorAttribute(
        Attribute::textColor,
        "Sets the text color of this element",
        { "#ff", "#ff00ff", "#00ff00ff" },
        Vector4(1, 1, 1, 1));

    const StyleAttribute BorderColor = AttributeHelper::MakeSimpleColorAttribute(
        Attribute::borderColor,
        "Sets the border color of this element",
        { "#ff", "#ff00ff", "#00ff00ff" },
        Vector4(1, 1, 1, 1));

    #pragma region BorderSize

    const StyleAttribute BorderSize = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::borderSize,
        "Sets the size of the border for this element",
        4, { 0, 0, 0, 0 },
        AHel::NumberType::SIZE);

    const StyleAttribute BorderWidth = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderWidth,
        "Sets the width of the border for this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute BorderTop = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderTop,
        "Sets the top width of the border for this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute BorderLeft = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderLeft,
        "Sets the left width of the border for this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute BorderBottom = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderBottom,
        "Sets the bottom width of the border for this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute BorderRight = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderRight,
        "Sets the right width of the border for this element",
        0, AHel::NumberType::SIZE);

    #pragma endregion

    const StyleAttribute BorderRadius = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::borderRadius,
        "Sets the border radius of this element (top-left, top-right, bottom-right, bottom-left)",
        4, { 0, 0, 0, 0 },
        AHel::NumberType::SIZE
    );

    const StyleAttribute Duration = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::duration,
        "Sets the transition duration time between styles",
        0, AHel::NumberType::TIME);

}

namespace EngineCore::UI::Init {

    const bool regStyleAtt() {
        // Size
        StyleAttribute::RegisterAttribute(Width);
        StyleAttribute::RegisterAttribute(Height);
        
        // Rotation
        StyleAttribute::RegisterAttribute(Rotation);
        StyleAttribute::RegisterAttribute(RotationX);
        StyleAttribute::RegisterAttribute(RotationY);
        StyleAttribute::RegisterAttribute(RotationZ);

        // Margin
        StyleAttribute::RegisterAttribute(Margin);
        StyleAttribute::RegisterAttribute(MarginTop);
        StyleAttribute::RegisterAttribute(MarginBottom);
        StyleAttribute::RegisterAttribute(MarginLeft);
        StyleAttribute::RegisterAttribute(MarginRight);

        // Padding
        StyleAttribute::RegisterAttribute(Padding);
        StyleAttribute::RegisterAttribute(PaddingTop);
        StyleAttribute::RegisterAttribute(PaddingBottom);
        StyleAttribute::RegisterAttribute(PaddingLeft);
        StyleAttribute::RegisterAttribute(PaddingRight);
        
        StyleAttribute::RegisterAttribute(BackgroundColor);
        StyleAttribute::RegisterAttribute(TextColor);
        StyleAttribute::RegisterAttribute(BorderColor);
        // Border size
        StyleAttribute::RegisterAttribute(BorderSize);
        StyleAttribute::RegisterAttribute(BorderWidth);
        StyleAttribute::RegisterAttribute(BorderTop);
        StyleAttribute::RegisterAttribute(BorderLeft);
        StyleAttribute::RegisterAttribute(BorderBottom);
        StyleAttribute::RegisterAttribute(BorderRight);

        StyleAttribute::RegisterAttribute(BorderRadius);
        StyleAttribute::RegisterAttribute(Duration);
        return true;
    }

}