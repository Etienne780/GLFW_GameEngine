#include "EngineLib/UI/UIManager.h"
#include "CoreLib/ConversionUtils.h"
#include "EngineLib/UI/Elements/Element.h"

namespace EngineCore::UI {

	ElementBase::ElementBase(std::string name, UIElementID id, std::shared_ptr<Style> style)
		: m_elementName(std::move(name)), m_id(id), m_elementStyle(std::move(style)) {
        m_cmd.isUI = true;
	}

    const std::string& ElementBase::GetName() const {
        return m_elementName; 
    }

    UIElementID ElementBase::GetID() const {
        return m_id; 
    }

    std::shared_ptr<Style> ElementBase::GetStyle() const {
        return m_elementStyle; 
    }

    Vector2 ElementBase::GetScreenPosition() const {
        Vector2 pos = m_localPosition;
        if (UIManager::GetUIScaling()) {
            pos *= UIManager::GetUIScaleFactor();
        }

        if (m_parentElement) {
            pos += m_parentElement->GetScreenPosition();
        }
        return pos;
    }

    Vector2 ElementBase::GetLocalPosition() const {
        return m_localPosition;
    }
    
    Vector2 ElementBase::GetScreenSize() const {
        Vector2 size = m_localScale;
        if (UIManager::GetUIScaling()) {
            size *= UIManager::GetUIScaleFactor();
        }
        return size;
    }

    Vector2 ElementBase::GetLocalSize() const {
        return m_localScale;
    }

    State ElementBase::GetState() const { 
        return m_state; 
    }

    std::vector<std::unique_ptr<ElementBase>>& ElementBase::GetChildren() { 
        return m_children; 
    }

    const std::vector<std::unique_ptr<ElementBase>>& ElementBase::GetChildren() const { 
        return m_children; 
    }

    void ElementBase::Update() {
        UpdateImpl();
    }

    void ElementBase::SendDrawCommand(Renderer* renderer, RenderLayerID renderLayerID) {
        MarkDirty();

        m_cmd.renderLayerID = renderLayerID;
        SendDrawCommandImpl(renderer);
    }

    ElementBase* ElementBase::GetParent() const {
        return m_parentElement;
    }

    void ElementBase::SetParent(ElementBase* elementPtr) {
        m_parentElement = elementPtr;
    }

	void ElementBase::SetState(State state) {
        if (m_state == state)
            return;
        m_state = state;

        switch (m_state) {
        case State::Hovered: 
            if (onHover) onHover();
            break;
        case State::Pressed:
            if (onPress) onPress();
            break;
        case State::Normal:
            break;
        default:
            break;
        }
	}

    void ElementBase::SetLocalPosition(const Vector2& position) {
        m_localPosition = position;
        MarkDirty();
    }

    void ElementBase::SetLocalPosition(float x, float y) {
        m_localPosition.Set(x, y);
        MarkDirty();
    }

    void ElementBase::SetLocalRotation(const Vector3& rotation) {
        m_localRotation = rotation;
        MarkDirty();
    }

    void ElementBase::SetLocalRotation(float x, float y, float z) {
        m_localRotation.Set(x, y, z);
        MarkDirty();
    }

    void ElementBase::SetLocalScale(const Vector2& scale) {
        m_localScale = scale;
        MarkDirty();
    }

    void ElementBase::SetLocalScale(float x, float y) {
        m_localScale.Set(x, y);
        MarkDirty();
    }

    bool ElementBase::IsMouseOver(const Vector2& mousePos) {
        Vector2 pos = GetScreenPosition();
        Vector2 size = GetScreenSize();
        return (mousePos.x > pos.x && pos.x + size.x > mousePos.x &&
                mousePos.y > pos.y && pos.y + size.y > mousePos.y);
    }

    Matrix4x4* ElementBase::GetWorldModelMatrixPtr() {
        if (m_worldMatrixDirty) {
            CalculateWorldModelMat();
            m_worldMatrixDirty = false;
        }

        return &m_worldMatrix;
    }

    const Matrix4x4& ElementBase::GetWorldModelMatrix() {
        if (m_worldMatrixDirty) {
            CalculateWorldModelMat();
            m_worldMatrixDirty = false;
        }

        return m_worldMatrix;
    }

    void ElementBase::CalculateLocalModelMat() {
        using namespace GLTransform4x4;

        Vector3 radians = {
            ConversionUtils::ToRadians(m_localRotation.x),
            ConversionUtils::ToRadians(m_localRotation.y),
            ConversionUtils::ToRadians(m_localRotation.z)
        };

        m_localMatrix = Scale(m_localScale.x, m_localScale.y, 1.0f);
        MakeRotateXYZ(m_localMatrix, radians);
        MakeTranslate(m_localMatrix, m_localPosition.x, m_localPosition.y, 0.0f);
    }

    void ElementBase::CalculateWorldModelMat() {
        using namespace GLTransform4x4;

        if (m_localMatrixDirty) {
            CalculateLocalModelMat();
            m_localMatrixDirty = false;
        }

        if (m_parentElement) {
            m_worldMatrix = m_parentElement->GetWorldModelMatrix() * m_localMatrix;
        }
        else {
            m_worldMatrix = m_localMatrix;
        }
    }

    void ElementBase::MarkDirty() {
        m_localMatrixDirty = true;
        m_worldMatrixDirty = true;
        for (auto& child : m_children) {
            child->MarkDirty();
        }
    }

}