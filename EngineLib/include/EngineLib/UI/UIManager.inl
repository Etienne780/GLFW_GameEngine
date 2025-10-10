#include <CoreLib/Log.h>

#include "UIManager.h"

namespace EngineCore {

    template<typename T, typename... Args>
    static std::shared_ptr<T> UIManager::Begin(Args&&... args) {
        static_assert(std::is_base_of<UI::ElementBase, T>::value, "T must derive from EngineCore::UI::Element");

        UIElementID id = UIElementID(m_idManager.GetNewUniqueIdentifier());
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("UIManager: Could not begin ui, no free Element id found!");
            return nullptr;
        }

        m_elementCount++;
        if (m_elementStack.empty()) {
            // Add root element
            std::shared_ptr<UI::ElementBase> element = m_roots.emplace_back(std::make_shared<T>(id, std::forward<Args>(args)...));
            element->Init();
            m_elementStack.push(element);
#ifndef NDEBUG
            if (m_isDebug)
                Log::Debug("UIManager: Started element {}({})", element->GetName(), id.value);
#endif 
            return std::static_pointer_cast<T>(element);
        }
        else {
            // Add child element
            auto& parent = m_elementStack.top();
            std::shared_ptr<UI::ElementBase> element = parent->AddChild<T>(id, std::forward<Args>(args)...);
            m_elementStack.push(element);
#ifndef NDEBUG
            if (m_isDebug)
                Log::Debug("UIManager: Started element {}({})", element->GetName(), id.value);
#endif 
            return std::static_pointer_cast<T>(element);
        }
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> UIManager::Add(Args&&... args) {
        static_assert(std::is_base_of<UI::ElementBase, T>::value, "T must derive from EngineCore::UI::Element");
        if (m_elementStack.empty()) {
            Log::Error("UIManager: Add called without Begin element!");
            return nullptr;
        }

        m_elementCount++;
        UIElementID id = UIElementID(m_idManager.GetNewUniqueIdentifier());
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("UIManager: Could not begin UI, no free Element id found!");
            return nullptr;
        }

        auto& parent = m_elementStack.top();
        std::shared_ptr<UI::ElementBase> element = parent->AddChild<T>(id, std::forward<Args>(args)...);
#ifndef NDEBUG
        if (m_isDebug)
            Log::Debug("UIManager: Added element {}({})", element->GetName(), id.value);
#endif 
        return std::static_pointer_cast<T>(element);
    }

}