#include "EngineLib/UI/Elements/Element.h"

namespace EngineCore::UI {

	Element::Element(std::string name, UIElementID id)
		: m_elementName(name), m_id(id) {
	}
		
	Element::Element(std::string name, UIElementID id, std::shared_ptr<Style> style)
		: m_elementName(name), m_id(id), m_style(style) {
	}

	const std::string& Element::GetName() const {
		return m_elementName;
	}

	UIElementID Element::GetID() const {
		return m_id; 
	}

	std::shared_ptr<Style> Element::GetStyle() const {
		return m_style;
	}

	std::vector<std::unique_ptr<Element>>& Element::GetChildren() {
		return m_children;
	}

	const std::vector<std::unique_ptr<Element>>& Element::GetChildren() const {
		return m_children;
	}

}