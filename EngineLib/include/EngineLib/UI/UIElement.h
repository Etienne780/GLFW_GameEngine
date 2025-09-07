#pragma once
#include <vector>

namespace EngineCore {

	class UIElement {
	public:

	private:
		std::vector<UIElement> m_childElements;
	};

}

/*
algemeien atribute:
- layout von childs
- overflow
- color
- radius
- duration

Element:
- Panel
- Image
- (svg)
- Text
- Button
- Checkmark
- Slider/Loading bar
- Inputfield
- Tabs
- Custom elements
*/