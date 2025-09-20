#include "EngineLib/UI/Elements/Panel.h"

namespace EngineCore::UI {
	
	Panel::Panel(UIElementID id)
		: m_id(id) {
	}

	Panel::Panel(UIElementID id, std::shared_ptr<Style> style)
		: m_id(id), m_style(style) {
	}

	Panel::Panel(std::string name, UIElementID id)
		: m_elementName(name), m_id(id) {
	}
		
	Panel::Panel(std::string name, UIElementID id, std::shared_ptr<Style> style)
		: m_elementName(name), m_id(id), m_style(style) {
	}

	const std::string& Panel::GetName() const {
		return m_elementName;
	}

	UIElementID Panel::GetID() const {
		return m_id; 
	}

	std::shared_ptr<Style> Panel::GetStyle() const {
		return m_style;
	}

	std::vector<std::unique_ptr<Panel>>& Panel::GetChildren() {
		return m_children;
	}

	const std::vector<std::unique_ptr<Panel>>& Panel::GetChildren() const {
		return m_children;
	}

	void Panel::SetState(State state) {
		m_state = state;
	}

}