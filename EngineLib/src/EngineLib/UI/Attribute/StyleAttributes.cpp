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
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute Height = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::height,
        "height of this element",
        "0px", AHel::NumberType::SIZE);

    #pragma region Rotation

    const StyleAttribute Rotation = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::rotation,
        "rotation (x, y, z) of this element",
        3, "0px 0px 0px",
        AHel::NumberType::SIZE);

    const StyleAttribute RotationX = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::rotationX,
        "rotation (x) of this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute RotationY = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::rotationY,
        "rotation (Y) of this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute RotationZ = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::rotationZ,
        "rotation (Z) of this element",
        "0px", AHel::NumberType::SIZE);

    #pragma endregion


    #pragma region Margin

    const StyleAttribute Margin = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::margin,
        "margin of this element, order: top, right, bottom, left",
        4, "0px 0px 0px 0px",
        AHel::NumberType::SIZE);
    
    const StyleAttribute MarginTop = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::marginTop,
        "top margin of this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute MarginBottom = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::marginBottom,
        "bottom margin of this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute MarginLeft = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::marginLeft,
        "left margin of this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute MarginRight = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::marginRight,
        "right margin of this element",
        "0px", AHel::NumberType::SIZE);

    #pragma endregion

    #pragma region Padding

    const StyleAttribute Padding = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::padding,
        "padding of this element, order: top, right, bottom, left",
        4, "0px 0px 0px 0px",
        AHel::NumberType::SIZE);

    const StyleAttribute PaddingTop = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::paddingTop,
        "top padding of this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute PaddingBottom = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::paddingBottom,
        "bottom padding of this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute PaddingLeft = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::paddingLeft,
        "left padding of this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute PaddingRight = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::paddingRight,
        "right padding of this element",
        "0px", AHel::NumberType::SIZE);

    #pragma endregion

    const StyleAttribute BackgroundColor = AttributeHelper::MakeSimpleColorAttribute(
        Attribute::backgroundColor,
        "Sets the background color of this element",
        { "#ff", "#ff00ff", "#00ff00ff" },
        "#00000000");

    const StyleAttribute TextColor = AttributeHelper::MakeSimpleColorAttribute(
        Attribute::textColor,
        "Sets the text color of this element",
        { "#ff", "#ff00ff", "#00ff00ff" },
        "#ff");

    const StyleAttribute BorderColor = AttributeHelper::MakeSimpleColorAttribute(
        Attribute::borderColor,
        "Sets the border color of this element",
        { "#ff", "#ff00ff", "#00ff00ff" },
        "#ff");

    #pragma region BorderSize

    const StyleAttribute BorderSize = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::borderSize,
        "Sets the size of the border for this element",
        4, "0px 0px 0px 0px",
        AHel::NumberType::SIZE);

    const StyleAttribute BorderTop = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderTop,
        "Sets the top width of the border for this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute BorderLeft = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderLeft,
        "Sets the left width of the border for this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute BorderBottom = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderBottom,
        "Sets the bottom width of the border for this element",
        "0px", AHel::NumberType::SIZE);

    const StyleAttribute BorderRight = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderRight,
        "Sets the right width of the border for this element",
        "0px", AHel::NumberType::SIZE);

    #pragma endregion

    const StyleAttribute BorderRadius = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::borderRadius,
        "Sets the border radius of this element (top-left, top-right, bottom-right, bottom-left)",
        4, "0px 0px 0px 0px",
        AHel::NumberType::SIZE
    );

    const StyleAttribute BorderType = AttributeHelper::MakeIndexedAttribute(
        Attribute::borderType,
        "Sets the border type for this element",
        { "inner", "outer" },
        1
    );

    const StyleAttribute Duration = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::duration,
        "Sets the transition duration time between styles",
        "0s", AHel::NumberType::TIME);

}

namespace EngineCore::UI::Init {

    const bool regStyleAtt() {
        namespace Att = Attribute;
        // Size
        StyleAttribute::RegisterAttribute(Width);
        StyleAttribute::RegisterAttribute(Height);
        
        // Rotation
        StyleAttribute::RegisterCompositeAttribute(Rotation, Att::rotationX, Att::rotationY, Att::rotationZ);
        StyleAttribute::RegisterAttribute(RotationX);
        StyleAttribute::RegisterAttribute(RotationY);
        StyleAttribute::RegisterAttribute(RotationZ);

        // Margin
        StyleAttribute::RegisterCompositeAttribute(Margin, Att::marginTop, Att::marginRight, Att::marginBottom, Att::marginLeft);
        StyleAttribute::RegisterAttribute(MarginTop);
        StyleAttribute::RegisterAttribute(MarginRight);
        StyleAttribute::RegisterAttribute(MarginBottom);
        StyleAttribute::RegisterAttribute(MarginLeft);

        // Padding
        StyleAttribute::RegisterCompositeAttribute(Padding, Att::paddingTop, Att::paddingRight, Att::paddingBottom, Att::paddingLeft);
        StyleAttribute::RegisterAttribute(PaddingTop);
        StyleAttribute::RegisterAttribute(PaddingRight);
        StyleAttribute::RegisterAttribute(PaddingBottom);
        StyleAttribute::RegisterAttribute(PaddingLeft);
        
        StyleAttribute::RegisterAttribute(BackgroundColor);
        StyleAttribute::RegisterAttribute(TextColor);
        StyleAttribute::RegisterAttribute(BorderColor);
        // Border size
        StyleAttribute::RegisterCompositeAttribute(BorderSize, Att::borderTop, Att::borderRight, Att::borderBottom, Att::borderLeft);
        StyleAttribute::RegisterAttribute(BorderTop);
        StyleAttribute::RegisterAttribute(BorderRight);
        StyleAttribute::RegisterAttribute(BorderLeft);
        StyleAttribute::RegisterAttribute(BorderBottom);

        StyleAttribute::RegisterAttribute(BorderRadius);
        StyleAttribute::RegisterAttribute(BorderType);
        StyleAttribute::RegisterAttribute(Duration);
        return true;
    }

}