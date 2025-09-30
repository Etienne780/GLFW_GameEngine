#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/AttributeHelper.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Attribute/LayoutAttributes.h"

namespace {

    const StyleAttribute Layout(
        Attribute::layout,
        "Layouts the elements inside of a container",
        std::vector<std::string>{"start", "center", "end", "stretch"},
        [](const ElementBase& element, const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
            auto values = AttributeHelper::GetValues(val);
            bool errorType = false;

            if (values.size() == 1) {
                std::string s = FormatUtils::toLowerCase(values[0]);
                if (AttributeHelper::ListContains(styleAtt->GetInputs(), s))
                    return StyleValue(s);
                errorType = true;
            }
            else if (values.size() == 2) {
                std::string s1 = FormatUtils::toLowerCase(values[0]);
                std::string s2 = FormatUtils::toLowerCase(values[1]);
                const auto& list = styleAtt->GetInputs();

                if (AttributeHelper::ListContains(list, s1) &&
                    AttributeHelper::ListContains(list, s2)) {
                    return StyleValue({ StyleValue(s1), StyleValue(s2) });
                }
                errorType = true;
            }

            if (errorType)
                Log::Warn("StyleAttribute: layout could not calculate value in style '{}', spelling mistake, input:'{}'!", element.GetStyle()->GetName(), val);
            else
                Log::Warn("StyleAttribute: layout could not calculate value in style '{}', invalid argument count, input:'{}'!", element.GetStyle()->GetName(), val);

            return StyleValue("center");
        }
    );

    const StyleAttribute LayoutHor = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutHor,
        "Horizontal layout of elements inside a container",
        { "start", "center", "end", "stretch" },
        "center"
    );

    const StyleAttribute LayoutVer = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutVer,
        "Vertical layout of elements inside a container",
        { "start", "center", "end", "stretch" },
        "center"
    );

}

namespace EngineCore::UI::Init {

    const bool regLayoutAtt() {
        StyleAttribute::RegisterAttribute(Layout);
        StyleAttribute::RegisterAttribute(LayoutHor);
        StyleAttribute::RegisterAttribute(LayoutVer);
        return true;
    }

}