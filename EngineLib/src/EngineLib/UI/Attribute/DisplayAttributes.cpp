#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/AttributeHelper.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Attribute/DisplayAttributes.h"

namespace  {

    const StyleAttribute Visibility = AttributeHelper::MakeIndexedAttribute(
        Attribute::visibility,
        "Visibility of the element and its childs, still effects layouting",
        { "visible", "hidden" },
        "visible"
    );
    
    const StyleAttribute Overflow = AttributeHelper::MakeIndexedAttribute(
        Attribute::overflow,
        "Controls what happens when the element's content is larger than its available space.",
        { "hidden", "none" },
        "none"
    );

}

namespace EngineCore::UI::Init {

    const bool regDisplayAtt() {
        StyleAttribute::RegisterAttribute(Visibility);
        StyleAttribute::RegisterAttribute(Overflow);
        return true;
    }

}