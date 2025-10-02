#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/AttributeHelper.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Attribute/LayoutAttributes.h"

namespace {

    #pragma region Layout

    const StyleAttribute LayoutAtt = AttributeHelper::MakeMultiStringAttribute(
        Attribute::layout,
        "Layouts the elements inside of a container",
        { 1, 2 },
        { "start", "center", "end", "stretch" },
        { "start", "start" }
    );

    const StyleAttribute LayoutHorAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutHor,
        "Horizontal layout of elements inside a container",
        { "start", "center", "end", "stretch" },
        "start"
    );

    const StyleAttribute LayoutVerAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutVer,
        "Vertical layout of elements inside a container",
        { "start", "center", "end", "stretch" },
        "start"
    );

    #pragma endregion

    #pragma region LayoutContent

    const StyleAttribute LayoutContentAtt = AttributeHelper::MakeMultiStringAttribute(
        Attribute::layoutContent,
        "Layouts the whole content inside of a container",
        { 1, 2 },
        { "start", "center", "end", "space-evenly", "space-around" },
        { "start", "start" }
    );


    const StyleAttribute LayoutContentHorAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutContentHor,
        "Horizontal Layouting of the whole content inside of a container",
        { "start", "center", "end", "space-evenly", "space-around"},
        "start"
    );

    const StyleAttribute LayoutContentVerAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutContentVer,
        "Vertical Layouting of the whole content inside of a container",
        { "start", "center", "end", "space-evenly", "space-around"},
        "start"
    );

    #pragma endregion

    const StyleAttribute LayoutDirectionAtt = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutDirection,
        "Direction in which the elements will arranged",
        { "row", "column", "row-start", "row-end", "column-start", "column-end"},
        "row-start"
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
        StyleAttribute::RegisterAttribute(LayoutHorAtt);
        StyleAttribute::RegisterAttribute(LayoutVerAtt);

        StyleAttribute::RegisterAttribute(LayoutContentAtt);
        StyleAttribute::RegisterAttribute(LayoutContentHorAtt);
        StyleAttribute::RegisterAttribute(LayoutContentVerAtt);

        StyleAttribute::RegisterAttribute(LayoutDirectionAtt);
        StyleAttribute::RegisterAttribute(LayoutWrapAtt);
        return true;
    }

}