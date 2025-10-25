#include "EngineLib/UI/Elements/Panel.h"

namespace EngineCore::UI {

    Panel::Panel(UIElementID id, std::shared_ptr<Style> style) 
        : Element<Panel>("Panel", id, style) {
    }

    void Panel::SendDrawCommand(Renderer* renderer) {
        renderer->Submit(m_cmd);
    }

}