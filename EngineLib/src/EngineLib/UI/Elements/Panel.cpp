#include "EngineLib/UI/Elements/Panel.h"

namespace EngineCore::UI {

	Panel::Panel(UIElementID id)
		: Element("Panel", id) {
	}

	Panel::Panel(UIElementID id, std::shared_ptr<Style> style)
		: Element("Panel", id, style) {
	}

}