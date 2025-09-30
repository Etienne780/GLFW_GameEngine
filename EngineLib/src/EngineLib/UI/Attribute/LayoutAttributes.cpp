#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/AttributeHelper.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Attribute/LayoutAttributes.h"

namespace {

    #pragma region Layout

    const StyleAttribute Layout = AttributeHelper::MakeMultiStringAttribute(
        Attribute::layout,
        "Layouts the elements inside of a container",
        { 1, 2 },
        { "start", "center", "end", "stretch" },
        { "start", "start" }
    );

    const StyleAttribute LayoutHor = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutHor,
        "Horizontal layout of elements inside a container",
        { "start", "center", "end", "stretch" },
        "start"
    );

    const StyleAttribute LayoutVer = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutVer,
        "Vertical layout of elements inside a container",
        { "start", "center", "end", "stretch" },
        "start"
    );

    #pragma endregion

    #pragma region LayoutContent

    const StyleAttribute LayoutContent = AttributeHelper::MakeMultiStringAttribute(
        Attribute::layoutContent,
        "Layouts the whole content inside of a container",
        { 1, 2 },
        { "start", "center", "end", "space-evenly", "space-around" },
        { "start", "start" }
    );


    const StyleAttribute LayoutContentHor = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutContentHor,
        "Horizontal Layouting of the whole content inside of a container",
        { "start", "center", "end", "space-evenly", "space-around"},
        "start"
    );

    const StyleAttribute LayoutContentVer = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutContentVer,
        "Vertical Layouting of the whole content inside of a container",
        { "start", "center", "end", "space-evenly", "space-around"},
        "start"
    );

    #pragma endregion

    const StyleAttribute LayoutDirection = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutDirection,
        "Direction in which the elements will arranged",
        { "row", "column", "row-start", "row-end", "column-start", "column-end"},
        "row-start"
    );

    const StyleAttribute LayoutWrap = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::layoutWrap,
        "Defines how child elements are arranged when they exceed the available space.",
        { "none", "wrap" },
        "wrap"
    );

}

namespace EngineCore::UI::Init {

    const bool regLayoutAtt() {
        StyleAttribute::RegisterAttribute(Layout);
        StyleAttribute::RegisterAttribute(LayoutHor);
        StyleAttribute::RegisterAttribute(LayoutVer);

        StyleAttribute::RegisterAttribute(LayoutContent);
        StyleAttribute::RegisterAttribute(LayoutContentHor);
        StyleAttribute::RegisterAttribute(LayoutContentVer);

        StyleAttribute::RegisterAttribute(LayoutDirection);
        StyleAttribute::RegisterAttribute(LayoutWrap);
        return true;
    }

}