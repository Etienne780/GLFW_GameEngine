#pragma once
#include <vector>
#include <memory>

#include "../EngineTypes.h"
#include "Style.h"

namespace EngineCore::UI {

	class Element {
	public:
		Element(UIElementID id);
		Element(UIElementID id, std::shared_ptr<Style> style);
		virtual ~Element() = default;

		template<typename T, typename... Args>
		T* AddChild(UIElementID id, Args&&... args) {
			auto& ptr = m_children.emplace_back(std::make_unique<T>(id, std::forward<Args>(args)...));
			return ptr.get();
		}

		UIElementID GetID() const;
		std::vector<std::unique_ptr<Element>>& GetChildren();

		virtual void SendDrawCommand() {};

	private:
		UIElementID m_id = UIElementID(ENGINE_INVALID_ID);
		std::shared_ptr<Style> m_elementStyle = std::make_shared<Style>();
		std::shared_ptr<Style> m_style = nullptr;
		std::vector<std::unique_ptr<Element>> m_children;
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