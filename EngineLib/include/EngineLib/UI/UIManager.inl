#include <CoreLib/Log.h>

#include "UIManager.h"

namespace EngineCore {

    template<typename T, typename... Args>
    static T* UIManager::Begin(Args&&... args) {
        static_assert(std::is_base_of<UI::Panel, T>::value, "T must derive from EngineCore::UI::Element");

        UIElementID id = UIElementID(m_idManager.GetNewUniqueIdentifier());
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("UIManager: Could not begin ui, no free Element id found!");
            return nullptr;
        }

        if (m_elementStack.empty()) {
            auto& element = m_roots.emplace_back(std::make_unique<T>(id, std::forward<Args>(args)...));
            m_elementStack.push(element.get());
            if (m_isDebug)
                Log::Debug("UIManager: Started element {}({})", element->GetName(), id.value);
            return static_cast<T*>(element.get());
        }
        else {
            auto* elementPtr = m_elementStack.top()->AddChild<T>(id, std::forward<Args>(args)...);
            m_elementStack.push(elementPtr);
            if (m_isDebug)
                Log::Debug("UIManager: Started element {}({})", elementPtr->GetName(), id.value);
            return  static_cast<T*>(elementPtr);
        }
    }

    template<typename T, typename... Args>
    void UIManager::Add(Args&&... args) {
        static_assert(std::is_base_of<UI::Panel, T>::value, "T must derive from EngineCore::UI::Element");
        if (m_elementStack.empty()) {
            Log::Error("UIManager: Add called without Begin element!");
            return;
        }

        UIElementID id = UIElementID(m_idManager.GetNewUniqueIdentifier());
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("UIManager: Could not begin UI, no free Element id found!");
            return;
        }

        auto ptr = m_elementStack.top()->AddChild<T>(id, std::forward<Args>(args)...);
        if (m_isDebug)
            Log::Debug("UIManager: Added element {}({})", ptr->GetName(), id.value);
    }

}