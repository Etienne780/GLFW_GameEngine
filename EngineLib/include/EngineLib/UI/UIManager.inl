#include <CoreLib/Log.h>

#include "UIManager.h"

namespace EngineCore {

    template<typename T, typename... Args>
    static T* UIManager::Begin(Args&&... args) {
        UIElementID id = UIElementID(m_idManager.GetNewUniqueIdentifier());
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("UIManager: Could not begin ui, no free Element id found!");
            return nullptr;
        }

        Log::Debug("UIManager: Started element {}", id.value);
        if (m_elementStack.empty()) {
            auto& element = m_roots.emplace_back(std::make_unique<T>(id, std::forward<Args>(args)...));
            m_elementStack.push(element.get());
            return element.get();
        }
        else {
            auto* elementPtr = m_elementStack.top()->AddChild<T>(id, std::forward<Args>(args)...);
            m_elementStack.push(elementPtr);
            return elementPtr;
        }
    }

    template<typename T, typename... Args>
    void UIManager::Add(Args&&... args) {
        if (m_elementStack.empty()) {
            Log::Error("UIManager: Add called without Begin element!");
            return;
        }

        UIElementID id = UIElementID(m_idManager.GetNewUniqueIdentifier());
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("UIManager: Could not begin UI, no free Element id found!");
            return;
        }
        m_elementStack.top()->AddChild<T>(id, args);
    }

}