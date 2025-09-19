#pragma once

#include "Element.h"

namespace EngineCore::UI {
	
	class Panel : public Element{
	public:
		Panel(UIElementID id);
		Panel(UIElementID id, std::shared_ptr<Style> style);
		~Panel() = default;

	private:
		
	};

}