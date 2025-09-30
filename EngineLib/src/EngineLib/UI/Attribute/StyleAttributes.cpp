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
        Attribute::margin,
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

    const StyleAttribute BorderWidth = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderWidth,
        "Sets the border width of this element",
        0, AHel::NumberType::SIZE);

    const StyleAttribute BorderRadius = AttributeHelper::MakeMultiNumberAttribute(
        Attribute::borderRadius,
        "Sets the border radius of this element (top-left, top-right, bottom-right, bottom-left)",
        4,
        { 0, 0, 0, 0 },
        AHel::NumberType::SIZE
    );

    const StyleAttribute Duration = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::duration,
        "Sets the transition duration time between styles",
        0, AHel::NumberType::TIME);

}

namespace EngineCore::UI::Init {

    const bool regStyleAtt() {
        StyleAttribute::RegisterAttribute(Width);
        StyleAttribute::RegisterAttribute(Height);

        StyleAttribute::RegisterAttribute(Margin);
        StyleAttribute::RegisterAttribute(MarginTop);
        StyleAttribute::RegisterAttribute(MarginBottom);
        StyleAttribute::RegisterAttribute(MarginLeft);
        StyleAttribute::RegisterAttribute(MarginRight);

        StyleAttribute::RegisterAttribute(Padding);
        StyleAttribute::RegisterAttribute(PaddingTop);
        StyleAttribute::RegisterAttribute(PaddingBottom);
        StyleAttribute::RegisterAttribute(PaddingLeft);
        StyleAttribute::RegisterAttribute(PaddingRight);
        
        StyleAttribute::RegisterAttribute(BackgroundColor);
        StyleAttribute::RegisterAttribute(BorderColor);
        StyleAttribute::RegisterAttribute(BorderWidth);
        StyleAttribute::RegisterAttribute(BorderRadius);
        StyleAttribute::RegisterAttribute(Duration);
        return true;
    }

}