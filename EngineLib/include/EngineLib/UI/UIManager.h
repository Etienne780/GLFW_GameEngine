#pragma once
#include <stack>
#include <vector>
#include <memory>
#include <string>
#include <CoreLib/Log.h>

#include "Element.h"
#include "../IDManager.h"

namespace EngineCore {

	class UIManager {
	public:
		static void Init();
		static void Shutdown();

		UIManager(const UIManager&) = delete;
		UIManager& operator=(const UIManager&) = delete;

		template<typename T, typename... Args>
		static T* Begin(Args&&... args);
		static void End();

		template<typename T, typename... Args>
		static void Add(Args&&... args);

		static void SendDrawCommands();
		static void DeleteAll();

	private:
		UIManager() = default;

		static inline IDManager m_idManager;

		static inline std::vector<std::unique_ptr<UI::Element>> m_roots;
		static inline std::stack<UI::Element*> m_elementStack;

		static void SendChildDrawCommands(std::unique_ptr<UI::Element>& element);
		static void FreeIDsInternal(UI::Element* element);
	};

}

#include "UIManager.inl"