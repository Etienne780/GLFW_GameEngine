#include <any>
#include <CoreLib/Algorithm.h>
#include <CoreLib/FormatUtils.h>
#include <CoreLib/Log.h>

#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/Style.h"
#include "EngineLib/Renderer.h"
#include "EngineLib/Input.h"
#include "EngineLib/UI/UITypes.h"
#include "EngineLib/UI/Attribute/_Attributes.h"
#include "EngineLib/UI/UIManager.h"

namespace EngineCore {


	void UIManager::Init() {
        m_renderer = Renderer::GetInstance();
        UI::Init::InitAttributes();
       
        BeginRootElement();
	}

	void UIManager::Shutdown() {
        EndRootElement();
	}

	void UIManager::End() {
        if (!m_elementStack.empty()) {
            auto& ended = m_elementStack.top();
            m_elementStack.pop();
#ifndef NDEBUG
            if (m_isDebug)
                Log::Debug("UIManager: Ended element {}({})", ended->GetName(), ended->GetID().value);
#endif
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
        BeginRootElement();
	}

	std::string UIManager::GetUIHierarchyString() {
		std::string hierarchyString;
		hierarchyString.append("UI Hierarchy:\n");

		for (const auto& elementPtr : m_roots) {
			BuildHierarchyString(elementPtr.get(), hierarchyString);
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

    bool UIManager::GetDebug() {
        return m_isDebug;
    }

    void UIManager::SetRootElementTransDirty() {
        for (auto& element : m_roots) {
            element->MarkTransDirty();
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

    std::shared_ptr<UI::Style> UIManager::GetElementBaseStyle() {
        static std::shared_ptr<UI::Style> baseStyle = [] {
            auto s = Style::Create("Base");
            auto& allAttributes = StyleAttribute::GetAllAttributes();
            for (auto& [name, att] : allAttributes) {
                s->Set(name, att.GetFallbackStr());
            }
            return s;
        }();

        return baseStyle;
    }

    void UIManager::BeginRootElement() {
        // creates the base object and sets its base values
        m_rootElement = Begin<UI::Panel>();
        m_rootElement->SetLayoutSize(m_windowSize);

        // updates the root element once
        m_rootElement->UpdateLayoutSize();
        m_rootElement->UpdateLayoutPosition();
        m_rootElement->UpdateWorldTransform();
    }

    void UIManager::EndRootElement() {
        End();
        m_rootElement = nullptr;
    }

    void UIManager::WindowResize(int width, int height) {
        m_windowSize.Set(static_cast<float>(width), static_cast<float>(height));
        m_rootElement->SetLayoutSize(m_windowSize);
        // recalculates the root element
        m_rootElement->UpdateLayoutSize();
        m_rootElement->UpdateLayoutPosition();
        m_rootElement->UpdateWorldTransform();

        CalculateOrthograpicMatrix(width, height);
        SetRootElementTransDirty();
    }

    void UIManager::Update(int width, int height) {
        if (m_windowSize.x != width || m_windowSize.y != height)
            WindowResize(width, height);

        if(m_enableUIScaling)
            m_uiScaleFactor = CalculateUIScaleFactor(width, height);

        UpdateInput();
        UpdateElementState();

        ComputeLayout(m_rootElement);
        UpdateChild(m_rootElement);
    }

    void UIManager::ComputeLayout(std::shared_ptr<UI::ElementBase>& root) {
        // updates the scale of every element that is dirty
        UpdateScales(root);

        // updates the position of each element with the newly calculated size of each element
        UpdatePositions(root);
    }


    void UIManager::PrecomputeDesiredPixels(std::shared_ptr<UI::ElementBase>& root) {
        for (auto& child : root->GetChildren()) {
            child->CalculateDesiredPixels();
        }
    }

    void UIManager::UpdateScales(std::shared_ptr<UI::ElementBase>& root) {
        PrecomputeDesiredPixels(root);

        for (auto& child : root->GetChildren()) {
            if (child->IsTransformDirty()) {
                child->UpdateLayoutSize();
            }

            UpdateScales(child);
        }
    }

    void UIManager::UpdatePositions(std::shared_ptr<UI::ElementBase>& root) {
        for (auto& child : root->GetChildren()) {
            if (child->IsTransformDirty()) {
                child->UpdateLayoutPosition();
                child->UpdateWorldTransform();

                child->SetTransformDirty(false);
            }

            UpdatePositions(child);
        }
    }

    void UIManager::UpdateChild(std::shared_ptr<UI::ElementBase>& element) {
        for (auto& child : element->GetChildren()) {
            child->UpdateImpl();
            UpdateChild(child);
        }
    }

    void UIManager::UpdateInput() {
        m_mousePos = Input::GetMousePosition();
        m_mouseDelta = Input::GetMousePositionDelta();
        m_leftMouseDown = Input::MousePressed(MouseButton::LEFT);
        m_leftMouseJustDown = Input::MouseJustPressed(MouseButton::LEFT);
        m_leftMouseJustReleased = Input::MouseJustReleased(MouseButton::LEFT);
    }

    void UIManager::UpdateElementState() {
        using namespace UI;

        std::shared_ptr<UI::ElementBase> element = nullptr;
        if (TryGetHoverElement(element)) {
            if (m_leftMouseDown) {
                element->SetState(State::Pressed);
                element->CallOnPress();

                if (m_mouseDelta.SquaredMagnitude() > 0) {
                    element->CallOnDrag();
                }
            }
            else if (m_leftMouseJustReleased) {
                if (element->GetState() == State::Pressed) {
                    element->CallOnClick();
                }
                element->SetState(State::Hovered);
            }
            else {
                element->SetState(State::Hovered);
                element->CallOnHover();
            }
        }

        if (m_lastChangeElement && m_lastChangeElement != element) {
            m_lastChangeElement->SetState(State::Normal);
        }

        m_lastChangeElement = element;
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

    bool UIManager::TryGetHoverElement(std::shared_ptr<UI::ElementBase>& outElement) {
        bool found = false;
        for (auto& child : m_rootElement->GetChildren()) {
            if (child->IsMouseOver(m_mousePos)) {
                found = true;
                outElement = child;
                GetHoverElementInternal(outElement);
            }
        }

        return found;
    }

    void UIManager::GetHoverElementInternal(std::shared_ptr<UI::ElementBase>& outElement) {
        for (auto& child : outElement->GetChildren()) {
            if (child->IsMouseOver(m_mousePos)) {
                outElement = child;
                GetHoverElementInternal(outElement);
            }
        }
    }

    void UIManager::BuildHierarchyString(const UI::ElementBase* root, std::string& outStr) {
        struct StackEntry {
            const UI::ElementBase* element;
            int level;
            bool closing; // false = open tag, true = closing tag
        };

        std::vector<StackEntry> stack;
        stack.push_back({ root, 0, false });

        while (!stack.empty()) {
            StackEntry current = stack.back();
            stack.pop_back();

            std::string indent(current.level * 2, ' ');
            const auto* elementPtr = current.element;

            if (current.closing) {
                outStr.append(indent);
                outStr.append("</" + elementPtr->GetName() + ">\n");
                continue;
            }

            unsigned int id = elementPtr->GetID().value;
            std::string idStr = (id == ENGINE_INVALID_ID) ? "INVALID_ID" : std::to_string(id);

            bool hasChildren = (elementPtr->GetChildCount() > 0);

            std::string styleStr;
            std::shared_ptr<UI::Style> style = elementPtr->GetStyle();
            if (style) {
                try {
                    const auto& attributes = style->GetAllState(elementPtr->GetState());
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

            outStr.append(indent);
            outStr.append("<" + elementPtr->GetName() + " id=\"" + idStr + "\"" + styleStr);

            if (!hasChildren) {
                outStr.append(" />\n");
                continue;
            }

            outStr.append(">\n");

            stack.push_back({ elementPtr, current.level, true });
            const auto& children = elementPtr->GetChildren();
            for (auto it = children.rbegin(); it != children.rend(); ++it) {
                stack.push_back({ it->get(), current.level + 1, false });
            }
        }
    }

    Matrix4x4* UIManager::GetOrthograpicMatrixPtr() {
        return &m_orthoMat;
    }

}