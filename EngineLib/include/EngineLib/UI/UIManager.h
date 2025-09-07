#pragma once
#include <stack>
#include <vector>
#include <memory>
#include <string>

#include "UIElement.h"
#include "../IDManager.h"

namespace EngineCore {

	class UIManager {
	public:
		static void Init();
		static void Shutdown();

		UIManager(const UIManager&) = delete;
		UIManager& operator=(const UIManager&) = delete;

		static UIManager* GetInstance();

		template<typename T, typename... Args>
		T* Begin(Args&&... args);
		void End();

		template<typename T, typename... Args>
		T* Add(Args&&... args);

		void Render();
		void DeleteAll();

	private:
		UIManager() = default;

		IDManager m_idManager;

		std::vector<std::unique_ptr<UIElement>> m_roots;
		std::stack<UIElement*> m_elementStack;

		void FreeIDsInternal(const UIElement* element);
	};

}

#include "UIManager.inl"