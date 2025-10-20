#pragma once
#include <stack>
#include <vector>
#include <memory>
#include <string>
#include <CoreLib/Log.h>
#include <CoreLib/Math/Vector2.h>
#include <CoreLib/Math/Matrix4x4.h>

#include "AttributeNames.h"
#include "UIElements.h"
#include "../IDManager.h"
#include "../EngineTypes.h"

namespace EngineCore {

	class Engine;
	class Renderer;
	class UIManager {
		friend class Engine;
		friend class Renderer;
	public:
		static void Init();
		static void Shutdown();

		UIManager(const UIManager&) = delete;
		UIManager& operator=(const UIManager&) = delete;

		/**
		* @brief Begins the creation of a new UI element. Must be paired with a corresponding End() call.
		* @tparam T The type of the UI element, must derive from UI::ElementBase.
		* @tparam Args Variadic template arguments forwarded to the element constructor.
		* @param args Arguments forwarded to the constructor of the element.
		* @return Pointer to the newly created element of type T, or nullptr if creation failed.
		*/
		template<typename T, typename... Args>
		static std::shared_ptr<T> Begin(Args&&... args);
		/**
		* @brief Ends the most recent Begin() call, finalizing the element on top of the stack.
		* Logs an error if End is called without a preceding Begin.
		*/
		static void End();
		/**
		* @brief Adds a new child element under the currently active Begin element.
		* Must be called between Begin() and End().
		* @tparam T The type of the UI element, must derive from UI::ElementBase.
		* @tparam Args Variadic template arguments forwarded to the element constructor.
		* @param args Arguments forwarded to the constructor of the element.
		* @return Pointer to the newly added element of type T, or nullptr if addition failed.
		*/
		template<typename T, typename... Args>
		static std::shared_ptr<T> Add(Args&&... args);

		/**
		* @brief Retrieves a constant pointer to a UI element by its ID.
		* @param elementID The unique identifier of the element to retrieve.
		* @return Shared pointer to the element if found, nullptr otherwise.
		*/
		static const std::shared_ptr<UI::ElementBase> GetElement(UIElementID elementID);
		/**
		* @brief Deletes a UI element by its ID, removing it from its parent or root list and freeing its ID recursively.
		* @param elementID The unique identifier of the element to delete.
		*/
		static void DeleteElement(UIElementID elementID);
		/**
		* @brief Gets all root elements
		* @returns a const referenc containing all root elements
		*/
		static const std::vector<std::shared_ptr<UI::ElementBase>>& GetAllRoots();
		/**
		* @brief deletes all root elements
		*/
		static void DeleteAllRoots();

		/**
		* @brief Generates a string representation of the entire UI hierarchy, starting from root elements.
		* @return A formatted string showing the hierarchy of all UI elements, their IDs, and style attributes.
		*/
		static std::string GetUIHierarchyString();
		static bool GetUIScaling();
		static Vector2 GetReferenceScreenSize();
		static Vector2 GetWindowSize();
		static float GetUIScaleFactor();
		static RenderLayerID GetRenderLayer();
		static size_t GetElementCount();

		static void SetRootElementTransDirty();

		static void SetUIRenderLayer(RenderLayerID layerID);
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

		static std::shared_ptr<UI::Style> GetElementBaseStyle();

		// ============== Debug stuff ==============

		static void SetDebug(bool value);
		static bool GetDebug();

		static void SetFreezUI(bool value);
		static bool GetFreezUI();
		static void StepUIForward();
		static void StepUIForward(int amount);
		static int GetStepUIAmount();

		static void SetForceState(UIElementID id, UI::State state);
		static void RemoveForceState(UIElementID id);
		static bool TryGetForceState(UIElementID id, UI::State& outState);

	private:
		UIManager() = default;
		static inline bool m_isDebug = false;
		static inline bool m_freezUI = false;
		static inline int m_stepUIByAmount = 0;

		static inline IDManager m_idManager;
		static inline Renderer* m_renderer = nullptr;
		static inline size_t m_elementCount = 0;
		static inline RenderLayerID m_renderLayerID{ ENGINE_INVALID_ID };

		static inline std::shared_ptr<UI::Style> m_rootStyle;
		static inline std::shared_ptr<UI::ElementBase> m_rootElement;
		static inline std::vector<std::shared_ptr<UI::ElementBase>> m_roots;
		static inline std::stack<std::shared_ptr<UI::ElementBase>> m_elementStack;// is used for creating ui hierarchy
		// the element what stat was last modified
		static inline std::shared_ptr<UI::ElementBase> m_lastChangeElement;
		// map containg all elemetns that have a force state
		static inline std::unordered_map<UIElementID, UI::State> m_forceStateMap;

		// If true, the UI is scaled relative to the reference screen size
		static inline bool m_enableUIScaling = false;
		// The reference screen size used as baseline for scaling
		static inline Vector2 m_referenceScreenSize{ 1920, 1080 };
		static inline Vector2 m_windowSize{ 0, 0 };
		// The current scale factor applied to the UI
		static inline float m_uiScaleFactor = 1.0f;

		static inline Vector2 m_mousePos{ 0, 0 };
		static inline Vector2 m_mouseDelta{ 0, 0 };
		static inline bool m_leftMouseDown = false;
		static inline bool m_leftMouseJustDown = false;
		static inline bool m_leftMouseJustReleased = false;

		static inline Matrix4x4 m_orthoMat;
		
		static void BeginRootElement();
		static void EndRootElement();
		
		static void WindowResize(int width, int height);

		static void Update(int width, int height);
		static void UpdateInput();
		static void UpdateElementState();
		
		static void ComputeLayout(std::shared_ptr<UI::ElementBase>& root);
		static void PrecomputeDesiredPixels(std::shared_ptr<UI::ElementBase>& root);
		static void UpdateScales(std::shared_ptr<UI::ElementBase>& root);
		static void UpdatePositions(std::shared_ptr<UI::ElementBase>& root);

		static void UpdateChild(std::shared_ptr<UI::ElementBase>& element);

		// Sends the Draw Commands of the UI::Elements
		static void SendDrawCommands();
		static void SendChildDrawCommands(std::shared_ptr<UI::ElementBase> element);
		static float CalculateUIScaleFactor(int width, int height);
		static void CalculateOrthograpicMatrix(int width, int height);
		/**
		* @brief Frees the unique identifiers of an element and all its children recursively.
		* @param element Pointer to the element whose IDs should be freed.
		*/
		static void FreeIDsInternal(std::shared_ptr<UI::ElementBase> element);
		/**
		* @brief Searches for a UI element in a given list and its children recursively.
		* @param list The list of elements to search in.
		* @param elementID The unique identifier of the element to search for.
		* @return Shared pointer to the element if found, nullptr otherwise.
		*/
		static std::shared_ptr<UI::ElementBase> SearchElementInternal(std::vector<std::shared_ptr<UI::ElementBase>>& list, UIElementID elementID);
		
		/*
		* @brief trys to get the current element that is hovered
		*/
		static bool TryGetHoverElement(std::shared_ptr<UI::ElementBase>& outElement);

		static void GetHoverElementInternal(std::shared_ptr<UI::ElementBase>& outElement);

		/**
		* @brief Recursively builds a string representation of a UI element and its children.
		* @param elementPtr Pointer to the element to process.
		* @param outStr Reference to the output string to append hierarchy information to.
		* @param level The current depth in the hierarchy, used for indentation.
		*/
		static void BuildHierarchyString(const UI::ElementBase* elementPtr, std::string& outStr);

		static Matrix4x4* GetOrthograpicMatrixPtr();
	};

}

#include "UIManager.inl"