#pragma once
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <CoreLib/Math/Vector2.h>

#include "../../EngineTypes.h"
#include "../Style.h"

namespace EngineCore::UI {

	class Panel {
	public:
		using Callback = std::function<void()>;

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

		State GetState() const;
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
		State m_state = State::Normal;

		(void)(*name) = nullptr;

		Vector2 m_position;
		Vector2 m_Size;

		void SetState(State state);
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