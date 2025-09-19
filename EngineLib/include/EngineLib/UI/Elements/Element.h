#pragma once
#include <vector>
#include <memory>
#include <string>

#include "../../EngineTypes.h"
#include "../Style.h"

namespace EngineCore::UI {

	class Element {
	public:
		Element(std::string name, UIElementID id);
		Element(std::string name, UIElementID id, std::shared_ptr<Style> style);
		virtual ~Element() = default;

		template<typename T, typename... Args>
		T* AddChild(UIElementID id, Args&&... args) {
			static_assert(std::is_base_of<Element, T>::value, "T must derive from EngineCore::UI::Element");

			auto& ptr = m_children.emplace_back(
				std::make_unique<T>(id, std::forward<Args>(args)...)
			);
			return static_cast<T*>(ptr.get());
		}

		const std::string& GetName() const;
		UIElementID GetID() const;
		std::shared_ptr<Style> GetStyle() const;
		std::vector<std::unique_ptr<Element>>& GetChildren();
		const std::vector<std::unique_ptr<Element>>& GetChildren() const;

		virtual void SendDrawCommand() {};

	private:
		std::string m_elementName;
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