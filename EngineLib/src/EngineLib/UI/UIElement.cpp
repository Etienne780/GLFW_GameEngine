#include "EngineLib/UI/UIElement.h"

namespace EngineCore {

	UIElement::UIElement(UIElementID id) 
		: m_id(id) {
	}
		
	UIElement::UIElement(UIElementID id, std::shared_ptr<UIStyle> style)
		: m_id(id), m_style(style) {
	}

	void UIElement::AddChild(std::unique_ptr<UIElement> child) {
		m_children.push_back(std::move(child));
	}

	UIElementID UIElement::GetID() const { 
		return m_id; 
	}

	const std::vector<std::unique_ptr<UIElement>>& UIElement::GetChildren() const {
		return m_children;
	}

}