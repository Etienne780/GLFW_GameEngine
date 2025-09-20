#pragma once
#include <vector>
#include <memory>
#include <string>
#include <CoreLib/Math/Vector2.h>

#include "../../EngineTypes.h"
#include "../Style.h"

namespace EngineCore::UI {

	class Panel {
	public:
		Panel(UIElementID id);
		Panel(UIElementID id, std::shared_ptr<Style> style);
		Panel(std::string name, UIElementID id);
		Panel(std::string name, UIElementID id, std::shared_ptr<Style> style);
		virtual ~Panel() = default;

		template<typename T, typename... Args>
		T* AddChild(UIElementID id, Args&&... args) {
			static_assert(std::is_base_of<Panel, T>::value, "T must derive from EngineCore::UI::Element");

			auto& ptr = m_children.emplace_back(
				std::make_unique<T>(id, std::forward<Args>(args)...)
			);
			return static_cast<T*>(ptr.get());
		}

		const std::string& GetName() const;
		UIElementID GetID() const;
		std::shared_ptr<Style> GetStyle() const;
		std::vector<std::unique_ptr<Panel>>& GetChildren();
		const std::vector<std::unique_ptr<Panel>>& GetChildren() const;

		virtual void Update() {};
		virtual void SendDrawCommand() {};

	private:
		std::string m_elementName = "Panel";
		UIElementID m_id = UIElementID(ENGINE_INVALID_ID);
		std::shared_ptr<Style> m_elementStyle = std::make_shared<Style>();
		std::shared_ptr<Style> m_style = nullptr;
		std::vector<std::unique_ptr<Panel>> m_children;

		Vector2 m_position;
		Vector2 m_Size;
	};

}

/*
Element:
- states
	- nothing
	- hover
	- active
	-

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