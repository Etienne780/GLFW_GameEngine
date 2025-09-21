#pragma once
#include "EngineLib/UI/Elements/Element.h"

namespace EngineCore::UI {

    class Panel : public Element<Panel> {
    public:
        using Element::Element;

        void TestPanelMethod() {};

    protected:

        void Update() override;
        void SendDrawCommand() override;
    };

}