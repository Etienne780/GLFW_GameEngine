#include <CoreLib/FormatUtils.h>
#include <CoreLib/Math/Vector4.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/AttributeHelper.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Attribute/StyleAttributes.h"

namespace {

    const StyleAttribute Width = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::width,
        "width of this element",
        0);

    const StyleAttribute Height = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::height,
        "height of this element",
        0);
    
    const StyleAttribute BackgroundColor = AttributeHelper::MakeSimpleColorAttribute(
        Attribute::backgroundColor,
        "Sets the background color of this element",
        { "#ff", "#ff00ff", "#00ff00ff" },
        Vector4(1,1,1,1));

    const StyleAttribute BorderColor = AttributeHelper::MakeSimpleColorAttribute(
        Attribute::borderColor,
        "Sets the border color of this element",
        { "#ff", "#ff00ff", "#00ff00ff" },
        Vector4(1, 1, 1, 1));

    const StyleAttribute BorderWidth = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::borderWidth,
        "Sets the border width of this element",
        0);

    const StyleAttribute BorderRadius(
        Attribute::borderRadius,
        "Sets the border radius of this element (top-left, top-right, bottom-right, bottom-left)",
        std::vector<std::string>{ "number" },
        [](const ElementBase& element, const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
            typedef AttributeHelper AHel;
            bool errorType = false;
            auto values = AHel::GetValues(val);
            if (values.size() == 1) {
                errorType = true;
                float v;
                if (AHel::TryGetNumber(element, values[0], v))
                    return Vector4(v, v, v, v);
            }
            else if (values.size() == 2) {
                errorType = true;
                float v1, v2;
                if (AHel::TryGetNumber(element, values[0], v1) &&
                    AHel::TryGetNumber(element, values[1], v2))
                    return Vector4(v1, v1, v2, v2);
            }
            else if (values.size() == 4) {
                errorType = true;
                float v1, v2, v3, v4;
                if (AHel::TryGetNumber(element, values[0], v1) &&
                    AHel::TryGetNumber(element, values[1], v2) &&
                    AHel::TryGetNumber(element, values[2], v3) &&
                    AHel::TryGetNumber(element, values[3], v4))
                    return Vector4(v1, v2, v3, v4);
            }
            if(errorType)
                Log::Warn("StyleAttribute: border radius could not calculate value in style '{}', invalid number, input:'{}'!", element.GetStyle()->GetName(), val);
            else
                Log::Warn("StyleAttribute: border radius could not calculate value in style '{}', invalid argument count, input:'{}'!", element.GetStyle()->GetName(), val);
            return StyleValue(Vector4(0, 0, 0, 0));
        }
    );

    const StyleAttribute Duration = AttributeHelper::MakeSimpleNumberAttribute(
        Attribute::duration,
        "Sets the transition duration time between styles",
        0);

}

namespace EngineCore::UI::Init {

    const bool regStyleAtt() {
        StyleAttribute::RegisterAttribute(Width);
        StyleAttribute::RegisterAttribute(Height);
        StyleAttribute::RegisterAttribute(BackgroundColor);
        StyleAttribute::RegisterAttribute(BorderColor);
        StyleAttribute::RegisterAttribute(BorderWidth);
        StyleAttribute::RegisterAttribute(BorderRadius);
        StyleAttribute::RegisterAttribute(Duration);
        return true;
    }

}
