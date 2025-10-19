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

    const StyleAttribute LayoutTypeAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutType,
        "Layout type of the element. If set to 'none', the element will not be displayed and has no effect on layout calculations",
        { "flex", "grid", "none" },
        "flex"
    );

    #pragma region Layout

    const StyleAttribute LayoutAtt = AttributeHelper::MakeMultiStringAttribute(
        Attribute::layout,
        "Layouts the elements inside of a container (major, minor)",
        { 1, 2 },
        layoutAttInputs,
        layoutAttInputs[0]
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
        { layoutAttInputs[0], layoutAttInputs[1], layoutAttInputs[2], "stretch" },
        layoutAttInputs[0]
    );

    #pragma endregion

    const StyleAttribute LayoutItemAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutItem,
        "Minor layout aligment with other elements",
        { layoutAttInputs[0], layoutAttInputs[1], layoutAttInputs[2], "stretch" },
        layoutAttInputs[0]
    );

    const StyleAttribute LayoutDirectionAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutDirection,
        "Direction in which the elements will arranged",
        { "row", "column" },
        "row"
    );

    const StyleAttribute LayoutWrapAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutWrap,
        "Defines how child elements are arranged when they exceed the available space.",
        { "none", "wrap" },
        "none"
    );

}

namespace EngineCore::UI::Init {

    const bool regLayoutAtt() {
        StyleAttribute::RegisterAttribute(LayoutTypeAtt);

        StyleAttribute::RegisterCompositeAttribute(LayoutAtt, Attribute::layoutMajor, Attribute::layoutMinor);

        StyleAttribute::RegisterAttribute(LayoutMajorAtt);
        StyleAttribute::RegisterAttribute(LayoutMinorAtt);

        StyleAttribute::RegisterAttribute(LayoutItemAtt);
        StyleAttribute::RegisterAttribute(LayoutDirectionAtt);
        StyleAttribute::RegisterAttribute(LayoutWrapAtt);
        return true;
    }

}