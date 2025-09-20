#include <any>
#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/UIManager.h"

namespace EngineCore {


	void UIManager::Init() {
	}

	void UIManager::Shutdown() {
	}

	void UIManager::End() {
        if (!m_elementStack.empty()) {
            auto* ended = m_elementStack.top();
            m_elementStack.pop();
            if (m_isDebug)
                Log::Debug("UIManager: Ended element {}({})", ended->GetName(), ended->GetID().value);
        }
		else {
            Log::Error("UIManager: End called without Begin!");
		}
	}

	void UIManager::DeleteAll() {
		for (const auto& element : m_roots) {
			FreeIDsInternal(element.get());
		}

		m_roots.clear();
	}

	std::string UIManager::GetUIHierarchyString() {
		std::string hierarchyString;
		hierarchyString.append("UI Hierarchy:\n");

		for (const auto& elementPtr : m_roots) {
			BuildHierarchyString(elementPtr.get(), hierarchyString, 0);
		}

		return hierarchyString;
	}

	void UIManager::SetDebug(bool value) {
		m_isDebug = value;
	}

    void UIManager::Update() {
        for (auto& element : m_roots) {
            element->Update();
            UpdateChild(element);
        }
    }

    void UIManager::UpdateChild(std::unique_ptr<UI::Panel>& element) {
        for (auto& child : element->GetChildren()) {
            child->Update();
            UpdateChild(child);
        }
    }

	void UIManager::SendDrawCommands() {
		for (auto& element : m_roots) {
			element->SendDrawCommand();
			SendChildDrawCommands(element);
		}
	}

    void UIManager::SendChildDrawCommands(std::unique_ptr<UI::Panel>& element) {
        for (auto& child : element->GetChildren()) {
            child->SendDrawCommand();
            SendChildDrawCommands(child);
        }
    }

	void UIManager::FreeIDsInternal(UI::Panel* element) {
		auto& childs = element->GetChildren();
		for (const auto& child : childs) {
			FreeIDsInternal(child.get());
		}
		m_idManager.FreeUniqueIdentifier(element->GetID().value);
	}

    void UIManager::BuildHierarchyString(const UI::Panel* elementPtr, std::string& outStr, int level) {
        std::string indent(level * 2, ' ');

        unsigned int id = elementPtr->GetID().value;
        std::string idStr = (id == ENGINE_INVALID_ID) ? "INVALID_ID" : std::to_string(id);

        bool hasChildren = !elementPtr->GetChildren().empty();

        // Collect style attributes
        std::string styleStr;
        std::shared_ptr<UI::Style> style = elementPtr->GetStyle();
        if (style) {
            try {
                const auto& attributes = style->GetAllState(UI::State::Normal);
                for (const auto& [name, styleVal] : attributes) {
                    std::string valStr;

                    std::visit([&](auto&& v) {
                        using T = std::decay_t<decltype(v)>;
                        if constexpr (std::is_same_v<T, float>) {
                            valStr = FormatUtils::toString(v);
                        }
                        else if constexpr (std::is_same_v<T, std::string>) {
                            valStr = "\"" + v + "\"";
                        }
                        else if constexpr (std::is_same_v<T, Vector3>) {
                            valStr = v.ToString();
                        }
                        else if constexpr (std::is_same_v<T, Vector4>) {
                            valStr = v.ToString();
                        }
                        else {
                            valStr = "\"<unsupported>\"";
                        }
                    }, styleVal.value);

                    switch (styleVal.unit) {
                    case UI::Unit::Px: valStr += "px"; break;
                    case UI::Unit::Percent: valStr += "%"; break;
                    case UI::Unit::None: break;
                    }

                    styleStr += " " + name + "=" + valStr;
                }
            }
            catch (const std::exception& e) {
                Log::Error("Style conversion error: {}", e.what());
            }
        }

        // Start Tag
        outStr.append(indent);
        outStr.append("<" + elementPtr->GetName() + " id=" + idStr + styleStr + ((hasChildren) ? ">\n" : " />\n"));

        if (!hasChildren) return;

        // Children
        for (const auto& child : elementPtr->GetChildren()) {
            BuildHierarchyString(child.get(), outStr, level + 1);
        }

        // End Tag
        outStr.append(indent);
        outStr.append("</" + elementPtr->GetName() + ">\n");
    }

}