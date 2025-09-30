#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/AttributeHelper.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Attribute/DisplayAttributes.h"

namespace  {

    const StyleAttribute Visibility = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::visibility,
        "Visibility of the element and its childs, still effects layouting",
        { "visible", "hidden" },
        "visible"
    );

    const StyleAttribute Display = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::display,
        "Display mode of the element and its childs, if none has no effect in layouting",
        { "display", "none" },
        "display"
    );
    
    const StyleAttribute Overflow = AttributeHelper::MakeSimpleStringAttribute(
        Attribute::overflow,
        "Controls what happens when the element's content is larger than its available space.",
        { "hidden", "none" },
        "none"
    );

}

namespace EngineCore::UI::Init {

    const bool regDisplayAtt() {
        StyleAttribute::RegisterAttribute(Visibility);
        StyleAttribute::RegisterAttribute(Display);
        StyleAttribute::RegisterAttribute(Overflow);
        return true;
    }

}