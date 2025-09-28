#pragma once
#include "../StyleAttribute.h"

/*
* @file LayoutAttributes.h
* @brief Contains attributes that control how child elements are arranged inside a container.
*
* These attributes affect alignment, direction, wrapping, and spacing of child UI elements.
* Examples: layout, layoutHor, layoutVer, layoutContent, layoutWrap, etc.
*/

namespace EngineCore::UI::Init {
    const bool regLayoutAtt();
}

namespace {

    using namespace EngineCore::UI;

    extern const StyleAttribute Layout;
    extern const StyleAttribute LayoutHor;
    extern const StyleAttribute LayoutVer;

    // not finished.
}
