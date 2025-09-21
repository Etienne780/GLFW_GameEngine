#pragma once
#include "EngineLib/UI/Elements/Element.h"

namespace EngineCore::UI {

    class Panel : public Element<Panel> {
    public:
        using Element::Element;
        Panel(UIElementID id, std::shared_ptr<Style> style = std::make_shared<Style>());

    protected:
    };

}