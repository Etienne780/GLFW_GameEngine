#pragma once
#include "../StyleAttribute.h"

/*
* @file DisplayAttributes.h
* @brief Contains attributes that control the visibility and rendering state of UI elements.
*
* These attributes define whether an element is shown, hidden, rendered, or excluded from layout.
* Examples: visibility, display, overflow, etc.
*/

namespace EngineCore::UI::Init {
    const bool regDisplayAtt();
}

namespace {

    using namespace EngineCore::UI;

    extern const StyleAttribute Visibility;
    extern const StyleAttribute Display;
    extern const StyleAttribute Overflow;

}