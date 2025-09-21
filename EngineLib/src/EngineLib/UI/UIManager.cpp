#include <any>
#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/Input.h"
#include "EngineLib/UI/UITypes.h"
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

    bool UIManager::GetUIScaling() {
        return m_enableUIScaling;
    }

    Vector2 UIManager::GetReferenceScreenSize() {
        return m_referenceScreenSize;
    }

    float UIManager::GetUIScaleFactor() {
        return m_uiScaleFactor;
    }

	void UIManager::SetDebug(bool value) {
		m_isDebug = value;
	}

    void UIManager::SetUIScaling(bool value) {
        m_enableUIScaling = value;
        // resets scale factor to default value
        if (!value)
            m_uiScaleFactor = 1;
    }

    void UIManager::SetReferenceScreenSize(float x, float y) {
        m_referenceScreenSize.Set(x, y);
    }

    void UIManager::SetReferenceScreenSize(const Vector2& size) {
        m_referenceScreenSize.Set(size.x, size.y);
    }

    void UIManager::Update(int width, int height) {
        if(m_enableUIScaling)
            m_uiScaleFactor = CalculateUIScaleFactor(width, height);

        for (auto& element : m_roots) {
            // needs mouse down and up
            UpdateElementState(element.get(), Input::GetMousePosition(), false, false);
            element->Update();
            UpdateChild(element);
        }
    }

    void UIManager::UpdateChild(std::unique_ptr<UI::ElementBase>& element) {
        for (auto& child : element->GetChildren()) {
            child->Update();
            UpdateChild(child);
        }
    }

    void UIManager::UpdateElementState(UI::ElementBase* element, const Vector2& mousePos, bool mouseDown, bool mouseReleased) {
        using namespace UI;
        if (element->IsMouseOver(mousePos)) {
            if (mouseDown) {
                element->SetState(State::Pressed);
            }
            else if (mouseReleased) {
                if (element->GetState() == State::Pressed && element->onClick) {
                    element->onClick();
                }
                element->SetState(State::Hovered);
            }
            else {
                element->SetState(State::Hovered);
            }
        }
        else {
            element->SetState(State::Normal);
        }

        for (auto& child : element->GetChildren()) {
            UpdateElementState(child.get(), mousePos, mouseDown, mouseReleased);
        }
    }


	void UIManager::SendDrawCommands() {
		for (auto& element : m_roots) {
			element->SendDrawCommand();
			SendChildDrawCommands(element);
		}
	}

    void UIManager::SendChildDrawCommands(std::unique_ptr<UI::ElementBase>& element) {
        for (auto& child : element->GetChildren()) {
            child->SendDrawCommand();
            SendChildDrawCommands(child);
        }
    }

    float UIManager::CalculateUIScaleFactor(int width, int height) {
        Vector2 scaleDiff = Vector2{ static_cast<float>(width), static_cast<float>(height) } / m_referenceScreenSize;

        // Option A: Average of both axes (uniform scaling)
        return (scaleDiff.x + scaleDiff.y) * 0.5f;

        // Option B: Take the smaller factor to ensure the UI always fits inside the window
        // return std::min(scaleDiff.x, scaleDiff.y);

        // Option C: Take the larger factor to ensure the UI always fills the window
        // return std::max(scaleDiff.x, scaleDiff.y);
    }

	void UIManager::FreeIDsInternal(UI::ElementBase* element) {
		auto& childs = element->GetChildren();
		for (const auto& child : childs) {
			FreeIDsInternal(child.get());
		}
		m_idManager.FreeUniqueIdentifier(element->GetID().value);
	}

    void UIManager::BuildHierarchyString(const UI::ElementBase* elementPtr, std::string& outStr, int level) {
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