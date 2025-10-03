#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/AttributeHelper.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Attribute/LayoutAttributes.h"

namespace {

    static std::vector<std::string> layoutAttInputs = {
        "start", "center", "end", "stretch", "space-evenly", "space-around"
    };

    #pragma region Layout

    const StyleAttribute LayoutAtt = AttributeHelper::MakeMultiStringAttribute(
        Attribute::layout,
        "Layouts the elements inside of a container (major, minor)",
        { 1, 2 },
        layoutAttInputs,
        { layoutAttInputs[0], layoutAttInputs[0] }
    );

    const StyleAttribute LayoutMajorAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutMajor,
        "Major layout of elements inside a container",
        layoutAttInputs,
        layoutAttInputs[0]
    );

    const StyleAttribute LayoutMinorAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutMinor,
        "Minor layout of elements inside a container",
        layoutAttInputs,
        layoutAttInputs[0]
    );

    #pragma endregion

    const StyleAttribute LayoutItemAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutItem,
        "Minor layout aligment with other elements",
        { "start", "center", "end", "stretch" },
        { "start" }
    );

    const StyleAttribute LayoutDirectionAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutDirection,
        "Direction in which the elements will arranged",
        { "row", "column"},
        "row"
    );

    const StyleAttribute LayoutWrapAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutWrap,
        "Defines how child elements are arranged when they exceed the available space.",
        { "none", "wrap" },
        "wrap"
    );

}

namespace EngineCore::UI::Init {

    const bool regLayoutAtt() {
        StyleAttribute::RegisterAttribute(LayoutAtt);
        StyleAttribute::RegisterAttribute(LayoutMajorAtt);
        StyleAttribute::RegisterAttribute(LayoutMinorAtt);

        StyleAttribute::RegisterAttribute(LayoutItemAtt);
        StyleAttribute::RegisterAttribute(LayoutDirectionAtt);
        StyleAttribute::RegisterAttribute(LayoutWrapAtt);
        return true;
    }

}