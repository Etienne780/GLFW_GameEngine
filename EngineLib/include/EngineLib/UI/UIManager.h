#pragma once
#include <stack>
#include <vector>
#include <memory>
#include <string>
#include <CoreLib/Log.h>
#include <CoreLib/Math/Vector2.h>

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
		static T* Add(Args&&... args);

		static void DeleteAll();

		static std::string GetUIHierarchyString();
		static bool GetUIScaling();
		static Vector2 GetReferenceScreenSize();
		static float GetUIScaleFactor();

		static void SetDebug(bool value);
		/*
		* @brief Enables or disables automatic UI scaling relative to the reference screen size. (default true)
		* @param value True to enable scaling, false to disable.
		*/
		static void SetUIScaling(bool value);

		/*
		* @brief Sets the reference screen size used for automatic UI scaling. (default 1920x1080)
		* @param x Reference width in pixels.
		* @param y Reference height in pixels.
		*/
		static void SetReferenceScreenSize(float x, float y);

		/*
		* @brief Sets the reference screen size used for automatic UI scaling. (default 1920x1080)
		* @param size A 2D vector containing reference width and height in pixels.
		*/
		static void SetReferenceScreenSize(const Vector2& size);

	private:
		UIManager() = default;
		static inline bool m_isDebug = false;
		static inline IDManager m_idManager;

		static inline std::vector<std::unique_ptr<UI::ElementBase>> m_roots;
		static inline std::stack<UI::ElementBase*> m_elementStack;

		// If true, the UI is scaled relative to the reference screen size
		static inline bool m_enableUIScaling = true;
		// The reference screen size used as baseline for scaling
		static inline Vector2 m_referenceScreenSize{ 1920, 1080 };
		// The current scale factor applied to the UI
		static inline float m_uiScaleFactor = 1.0f;

		// Updates the scale and states of the UI::Element
		static void Update(int width, int height);
		static void UpdateChild(std::unique_ptr<UI::ElementBase>& element);
		static void UpdateElementState(UI::ElementBase* element, const Vector2& mousePos, bool mouseDown, bool mouseReleased);
		// Sends the Draw Commands of the UI::Elements
		static void SendDrawCommands();
		static void SendChildDrawCommands(std::unique_ptr<UI::ElementBase>& element);
		static float CalculateUIScaleFactor(int width, int height);
		static void FreeIDsInternal(UI::ElementBase* element);
		static void BuildHierarchyString(const UI::ElementBase* elementPtr, std::string& outStr, int level);
	};

}

#include "UIManager.inl"