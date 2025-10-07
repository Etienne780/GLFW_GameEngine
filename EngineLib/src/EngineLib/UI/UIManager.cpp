#include <any>
#include <CoreLib/Algorithm.h>
#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/Renderer.h"
#include "EngineLib/Input.h"
#include "EngineLib/UI/UITypes.h"
#include "EngineLib/UI/Attribute/_Attributes.h"
#include "EngineLib/UI/UIManager.h"

namespace EngineCore {


	void UIManager::Init() {
        m_renderer = Renderer::GetInstance();
        UI::Init::InitAttributes();
	}

	void UIManager::Shutdown() {
	}

	void UIManager::End() {
        if (!m_elementStack.empty()) {
            auto& ended = m_elementStack.top();
            m_elementStack.pop();
            if (m_isDebug)
                Log::Debug("UIManager: Ended element {}({})", ended->GetName(), ended->GetID().value);
        }
		else {
            Log::Error("UIManager: End called without Begin!");
		}
	}

    const std::shared_ptr<UI::ElementBase> UIManager::GetElement(UIElementID elementID) {
        return SearchElementInternal(m_roots, elementID);
    }

    void UIManager::DeleteElement(UIElementID elementID) {
        auto element = SearchElementInternal(m_roots, elementID);
        auto parent = element->GetParent();
        FreeIDsInternal(element);

        if (parent) {
            auto& parentChilds = parent->GetChildren();
            parentChilds.erase(
                std::remove_if(parentChilds.begin(), parentChilds.end(),
                    [element](const std::shared_ptr<UI::ElementBase>& ptr) { return ptr == element; }),
                parentChilds.end());
        }
        else {
            m_roots.erase(
                std::remove_if(m_roots.begin(), m_roots.end(),
                    [element](const std::shared_ptr<UI::ElementBase>& ptr) { return ptr == element; }),
                m_roots.end());
        }
    }

    const std::vector<std::shared_ptr<UI::ElementBase>>& UIManager::GetAllRoots() {
        return m_roots;
    }

	void UIManager::DeleteAllRoots() {
        m_elementCount = 0;
		for (const auto& element : m_roots) {
			FreeIDsInternal(element);
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

    Vector2 UIManager::GetWindowSize() {
        return m_windowSize;
    }

    float UIManager::GetUIScaleFactor() {
        return m_uiScaleFactor;
    }

    RenderLayerID UIManager::GetRenderLayer() {
        return m_renderLayerID;
    }

    size_t UIManager::GetElementCount() {
        return m_elementCount;
    }

    void UIManager::SetRootElementsDirty() {
        for (auto& element : m_roots) {
            element->MarkDirty();
        }
    }

    void UIManager::SetUIRenderLayer(RenderLayerID layerID) {
        m_renderLayerID = layerID;
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

    void UIManager::WindowResize(int width, int height) {
        m_windowSize.Set(static_cast<float>(width), static_cast<float>(height));
        CalculateOrthograpicMatrix(width, height);

        for (auto& el : m_roots) {
            el->WindowResize(width, height);
            WindowResizeChild(width, height, el);
        }
    }

    void UIManager::WindowResizeChild(int width, int height, std::shared_ptr<UI::ElementBase>& element) {
        for (auto& child : element->GetChildren()) {
            child->WindowResize(width, height);
            WindowResizeChild(width, height, child);
        }
    }

    void UIManager::Update(int width, int height) {
        if (m_windowSize.x != width || m_windowSize.y != height)
            WindowResize(width, height);

        if(m_enableUIScaling)
            m_uiScaleFactor = CalculateUIScaleFactor(width, height);


        for (auto& el : m_roots) {
            // needs mouse down and up
            UpdateElementState(el, 
                Input::GetMousePosition(), 
                Input::MouseJustPressed(MouseButton::LEFT), 
                Input::MouseJustReleased(MouseButton::LEFT));

            el->UpdateImpl();
            UpdateChild(el);
        }
    }

    void UIManager::UpdateChild(std::shared_ptr<UI::ElementBase>& element) {
        for (auto& child : element->GetChildren()) {
            child->UpdateImpl();
            UpdateChild(child);
        }
    }

    void UIManager::UpdateElementState(std::shared_ptr<UI::ElementBase> element, const Vector2& mousePos, bool mouseDown, bool mouseReleased) {
        using namespace UI;
        if (element->IsMouseOver(mousePos)) {
            if (mouseDown) {
                element->SetState(State::Pressed);
            }
            else if (mouseReleased) {
                if (element->GetState() == State::Pressed && element->m_onClick) {
                    element->m_onClick();
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
            UpdateElementState(child, mousePos, mouseDown, mouseReleased);
        }
    }


	void UIManager::SendDrawCommands() {
        if (m_renderLayerID.value == ENGINE_INVALID_ID) {
            Log::Warn("UIManager: UI Render layer is not set! Use UIManager::SetUIRenderLayer to set the render layer for the UI");
            return;
        }

        m_renderer->ReserveCommands(m_elementCount);
		for (auto& element : m_roots) {
			element->SendDrawCommandImpl(m_renderer, m_renderLayerID);
			SendChildDrawCommands(element);
		}
	}

    void UIManager::SendChildDrawCommands(std::shared_ptr<UI::ElementBase> element) {
        for (auto& child : element->GetChildren()) {
            child->SendDrawCommandImpl(m_renderer, m_renderLayerID);
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

    void UIManager::CalculateOrthograpicMatrix(int width, int height) {
        m_orthoMat = GLTransform4x4::Orthographic(
            0.0f, static_cast<float>(width),
            static_cast<float>(height), 0.0f,
            -1000.0f, 1000.0f);
    }

	void UIManager::FreeIDsInternal(std::shared_ptr<UI::ElementBase> element) {
		auto& childs = element->GetChildren();
		for (const auto& child : childs) {
			FreeIDsInternal(child);
		}
		m_idManager.FreeUniqueIdentifier(element->GetID().value);
	}

    std::shared_ptr<UI::ElementBase> UIManager::SearchElementInternal(std::vector<std::shared_ptr<UI::ElementBase>>& list, UIElementID id) {
        if (list.empty())
            return nullptr;

        // binary search if the ids are in order and size is large enough
        if (!m_idManager.IsIDFallback() && list.size() > 64) {
            auto element = Algorithm::Search::GetBinary<UI::ElementBase>(list, 
                [](UI::ElementBase& e) { return e.GetID().value; }, id.value);
            if (element)
                return element;
        }

        // lineare search
        return Algorithm::Search::GetLinearRecursive(list, 
            [id](UI::ElementBase& e) { return e.GetID() == id; }, 
            [](UI::ElementBase& e) -> std::vector<std::shared_ptr<UI::ElementBase>>& { return e.GetChildren(); });
    }

    void UIManager::BuildHierarchyString(const UI::ElementBase* elementPtr, std::string& outStr, int level) {
        std::string indent(level * 2, ' ');

        unsigned int id = elementPtr->GetID().value;
        std::string idStr = (id == ENGINE_INVALID_ID) ? "INVALID_ID" : std::to_string(id);

        bool hasChildren = (elementPtr->GetChildCount() > 0);

        // Collect style attributes
        std::string styleStr;
        std::shared_ptr<UI::Style> style = elementPtr->GetStyle();
        if (style) {
            try {
                const auto& attributes = style->GetAllState(elementPtr->GetState());
                // if style has to many attributes display the style name
                styleStr += " style=\"" + style->GetName() + "\"";
                if (attributes.size() <= 5) {
                    for (const auto& [name, styleVal] : attributes) {
                        styleStr += " " + name + "=\"" + styleVal + "\"";
                    }
                }
            }
            catch (const std::exception& e) {
                Log::Error("Style conversion error: {}", e.what());
            }
        }

        // Start Tag
        outStr.append(indent);
        outStr.append("<" + elementPtr->GetName() + " id=\"" + idStr + "\"" + styleStr + ((hasChildren) ? ">\n" : " />\n"));

        if (!hasChildren) return;

        // Children
        for (const auto& child : elementPtr->GetChildren()) {
            BuildHierarchyString(child.get(), outStr, level + 1);
        }

        // End Tag
        outStr.append(indent);
        outStr.append("</" + elementPtr->GetName() + ">\n");
    }

    Matrix4x4* UIManager::GetOrthograpicMatrixPtr() {
        return &m_orthoMat;
    }

}