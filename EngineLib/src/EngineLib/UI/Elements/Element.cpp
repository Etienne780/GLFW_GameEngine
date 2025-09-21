#include "EngineLib/UI/UIManager.h"
#include "EngineLib/UI/Elements/Element.h"

namespace EngineCore::UI {

	ElementBase::ElementBase(std::string name, UIElementID id, std::shared_ptr<Style> style)
		: m_elementName(std::move(name)), m_id(id), m_elementStyle(std::move(style)) {
	}

    const std::string& ElementBase::GetName() const {
        return m_elementName; 
    }

    UIElementID ElementBase::GetID() const {
        return m_id; 
    }

    std::shared_ptr<Style> ElementBase::GetStyle() const {
        return m_elementStyle; 
    }

    Vector2 ElementBase::GetScreenPosition() const {
        Vector2 pos = m_localPosition;
        if (UIManager::GetUIScaling()) {
            pos *= UIManager::GetUIScaleFactor();
        }

        if (m_parentElement) {
            pos += m_parentElement->GetScreenPosition();
        }
        return pos;
    }

    Vector2 ElementBase::GetLocalPosition() const {
        return m_localPosition;
    }
    
    Vector2 ElementBase::GetScreenSize() const {
        Vector2 size = m_localSize;
        if (UIManager::GetUIScaling()) {
            size *= UIManager::GetUIScaleFactor();
        }
        return size;
    }

    Vector2 ElementBase::GetLocalSize() const {
        return m_localSize;
    }

    State ElementBase::GetState() const { 
        return m_state; 
    }

    std::vector<std::unique_ptr<ElementBase>>& ElementBase::GetChildren() { 
        return m_children; 
    }

    const std::vector<std::unique_ptr<ElementBase>>& ElementBase::GetChildren() const { 
        return m_children; 
    }

    void ElementBase::SendDrawCommand() {
    
    }

    ElementBase* ElementBase::GetParent() const {
        return m_parentElement;
    }

    void ElementBase::SetParent(ElementBase* elementPtr) {
        m_parentElement = elementPtr;
    }

	void ElementBase::SetState(State state) {
        if (m_state == state)
            return;
        m_state = state;

        switch (m_state) {
        case State::Hovered: 
            if (onHover) onHover();
            break;
        case State::Pressed:
            if (onPress) onPress();
            break;
        case State::Normal:
            break;
        default:
            break;
        }
	}

    bool ElementBase::IsMouseOver(const Vector2& mousePos) {
        Vector2 pos = GetScreenPosition();
        Vector2 size = GetScreenSize();
        return (mousePos.x > pos.x && pos.x + size.x > mousePos.x &&
                mousePos.y > pos.y && pos.y + size.y > mousePos.y);
    }

}