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