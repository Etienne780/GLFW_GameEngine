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

    extern const StyleAttribute LayoutAtt;
    extern const StyleAttribute LayoutHorAtt;
    extern const StyleAttribute LayoutVerAtt;

    extern const StyleAttribute LayoutContentAtt;
    extern const StyleAttribute LayoutContentHorAtt;
    extern const StyleAttribute LayoutContentVerAtt;

    extern const StyleAttribute LayoutDirectionAtt;
    extern const StyleAttribute LayoutWrapAtt;

}
