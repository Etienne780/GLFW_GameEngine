#include "EngineLib/UI/Element.h"

namespace EngineCore::UI {

	Element::Element(UIElementID id)
		: m_id(id) {
	}
		
	Element::Element(UIElementID id, std::shared_ptr<Style> style)
		: m_id(id), m_style(style) {
	}

	UIElementID Element::GetID() const {
		return m_id; 
	}

	std::vector<std::unique_ptr<Element>>& Element::GetChildren() {
		return m_children;
	}

}