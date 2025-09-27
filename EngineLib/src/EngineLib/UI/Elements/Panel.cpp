#include "EngineLib/AssetRepository.h"
#include "EngineLib/UI/Elements/Panel.h"

namespace EngineCore::UI {

    Panel::Panel(UIElementID id, std::shared_ptr<Style> style) 
        : Element<Panel>("Panel", id, style) {
    }

    void Panel::SendDrawCommandImpl(Renderer* renderer) {
        m_cmd.isUI = true;
        m_cmd.type = RenderCommandType::Mesh;
        m_cmd.materialID = ASSETS::ENGINE::MATERIAL::DefaultUI();
        m_cmd.meshID = ASSETS::ENGINE::MESH::Plain();
        m_cmd.modelMatrix = GetWorldModelMatrixPtr();

        renderer->Submit(m_cmd);
    }

}