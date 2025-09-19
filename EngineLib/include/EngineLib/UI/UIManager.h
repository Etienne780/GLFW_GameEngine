#pragma once
#include <stack>
#include <vector>
#include <memory>
#include <string>
#include <CoreLib/Log.h>

#include "UIElements.h"
#include "../IDManager.h"

namespace EngineCore {

	class Engine;
	class UIManager {
	friend class Engine;
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
		static void DeleteAll();

		static std::string GetUIHierarchyString();

		static void SetDebug(bool value);

	private:
		UIManager() = default;
		static inline bool m_isDebug = false;
		static inline IDManager m_idManager;

		static inline std::vector<std::unique_ptr<UI::Element>> m_roots;
		static inline std::stack<UI::Element*> m_elementStack;

		void FreeIDsInternal(const UI::Element* element);
		static void SendDrawCommands();
		static void SendChildDrawCommands(std::unique_ptr<UI::Element>& element);
		static void FreeIDsInternal(UI::Element* element);
		static void BuildHierarchyString(const UI::Element* elementPtr, std::string& outStr, int level);
	};

}

#include "UIManager.inl"