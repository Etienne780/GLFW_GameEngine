#include <CoreLib/Log.h>

#include "UIManager.h"

namespace EngineCore {

    template<typename T, typename... Args>
    T* UIManager::Begin(Args&&... args) {
        UIElementID id = UIElementID(m_idManager.GetNewUniqueIdentifier());
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("UIManager: Could not begin ui, no free Element id found!");
            return;
        }

        auto element = std::make_unique<T>(id, std::forward<Args>(args)...);
        T* rawPtr = element.get();

        if (m_elementStack.empty()) {
            m_roots.push_back(std::move(element));
        }
        else {
            m_elementStack.top()->AddChild(std::move(element));
        }

        m_elementStack.push(rawPtr);
        return rawPtr;
    }

    template<typename T, typename... Args>
    T* UIManager::Add(Args&&... args) {
        if (m_elementStack.empty()) {
            Log::Error("UIManager: Add called without Begin element!");
            return nullptr;
        }

        UIElementID id = UIElementID(m_idManager.GetNewUniqueIdentifier());
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("UIManager: Could not begin ui, no free Element id found!");
            return;
        }

        auto element = std::make_unique<T>(id, std::forward<Args>(args)...);
        T* rawPtr = element.get();

        m_elementStack.top()->AddChild(std::move(element));
        return rawPtr;
    }


}