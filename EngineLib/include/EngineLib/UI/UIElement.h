#pragma once
#include "../EngineTypes.h"
#include "UIStyle.h"
#include <vector>

namespace EngineCore {

	class UIElement {
	public:
		UIElement(UIElementID id);
		UIElement(UIElementID id, std::shared_ptr<UIStyle> style);
		virtual ~UIElement() = default;

		void AddChild(std::unique_ptr<UIElement> child);

		UIElementID GetID() const;
		const std::vector<std::unique_ptr<UIElement>>& GetChildren() const;

		virtual void Render() = 0;

	private:
		UIElementID m_id = UIElementID(ENGINE_INVALID_ID);
		std::shared_ptr<UIStyle> m_elementStyle = std::make_shared<UIStyle>();
		std::shared_ptr<UIStyle> m_style = nullptr;
		std::vector<std::unique_ptr<UIElement>> m_children;
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