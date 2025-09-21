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

    State ElementBase::GetState() const { 
        return m_state; 
    }

    std::vector<std::unique_ptr<ElementBase>>& ElementBase::GetChildren() { 
        return m_children; 
    }

    const std::vector<std::unique_ptr<ElementBase>>& ElementBase::GetChildren() const { 
        return m_children; 
    }

	void ElementBase::SetState(State state) {
		m_state = state;
	}

}