#include <CoreLib/ConversionUtils.h>
#include <CoreLib/FormatUtils.h>


#include "EngineLib/UI/UIManager.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"

namespace EngineCore::UI {

	ElementBase::ElementBase(std::string name, UIElementID id, std::shared_ptr<Style> style, MaterialID matID)
		: m_elementName(std::move(name)), m_id(id), m_style(std::move(style)){
        m_cmd.isUI = true;
        m_cmd.type = RenderCommandType::Mesh;
        m_cmd.meshID = ASSETS::ENGINE::MESH::UIPlain();
        m_cmd.materialID = matID;
	}

    ElementBase::ElementBase(std::string name, UIElementID id, MaterialID matID, std::shared_ptr<Style> style) 
        : m_elementName(std::move(name)), m_id(id), m_style(std::move(style)) {
        m_cmd.isUI = true;
        m_cmd.type = RenderCommandType::Mesh;
        m_cmd.meshID = ASSETS::ENGINE::MESH::Plain();
        m_cmd.materialID = matID;
    }

    void ElementBase::Init() {
        RegisterAttributesImpl();
        SetStyleAttributes();
    }

    const std::string& ElementBase::GetName() const {
        return m_elementName; 
    }

    UIElementID ElementBase::GetID() const {
        return m_id; 
    }

    std::shared_ptr<Style> ElementBase::GetStyle() const {
        return m_style; 
    }

    Vector2 ElementBase::GetScreenPosition() const {
        Vector2 pos = m_localPosition;
        if (m_parentElementPtr) {
            pos += m_parentElementPtr->GetScreenPosition();
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

    float ElementBase::GetParentWidth() const {
        if (m_parentElementPtr) {
            return m_parentElementPtr->GetLocalSize().x;
        }
        else {
            return UIManager::GetWindowSize().x;
        }
    }

    float ElementBase::GetParentHeight() const {
        if (m_parentElementPtr) {
            return m_parentElementPtr->GetLocalSize().y;
        }
        else {
            return UIManager::GetWindowSize().y;
        }
    }

    float ElementBase::GetViewportWidth() const {
        return UIManager::GetWindowSize().x;
    }

    float ElementBase::GetViewportHeight() const {
        return UIManager::GetWindowSize().y;
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

    ElementBase* ElementBase::GetParent() const {
        return m_parentElementPtr;
    }

    void ElementBase::SetParent(ElementBase* elementPtr) {
        m_parentElementPtr = elementPtr;
    }

	void ElementBase::SetState(State state) {
        if (m_state == state)
            return;
        m_state = state;

        switch (m_state) {
        case State::Hovered: 
            if (m_onHover) m_onHover();
            break;
        case State::Pressed:
            if (m_onPress) m_onPress();
            break;
        case State::Normal:
            break;
        default:
            break;
        }

        SetStyleAttributes();
	}

    void ElementBase::SetLocalPosition(const Vector2& pos) {
        SetLocalPosition(pos.x, pos.y);
    }

    void ElementBase::SetLocalPosition(float x, float y) {
        m_localPosition.Set(x,y);
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
        SetLocalScale(scale.x, scale.y);
    }

    void ElementBase::SetLocalScale(float x, float y) {
        m_localScale.Set(x, y);
        m_sbo.SetParam("uSize", m_localScale);
        MarkDirty();
    }

    void ElementBase::SetLocalScaleX(float x) {
        m_localScale.x = x;
        m_sbo.SetParam("uSize", m_localScale);
        MarkDirty();
    }

    void ElementBase::SetLocalScaleY(float y) {
        m_localScale.y = y;
        m_sbo.SetParam("uSize", m_localScale);
        MarkDirty();
    }

    void ElementBase::setBackgroundColor(const Vector4& color) {
        m_backgroundColor = color;
        m_sbo.SetParam("uBackgroundColor", m_backgroundColor);
    }

    void ElementBase::setBorderColor(const Vector4& color) {
        m_borderColor = color;
        m_sbo.SetParam("uBorderColor", m_borderColor);
    }

    void ElementBase::setBorderRadius(const Vector4& radius) {
        m_borderRadius = radius;
        m_sbo.SetParam("uBorderRadius", m_borderRadius);
    }

    void ElementBase::setBorderWidth(float width) {
        if (width < 0.0f) width = 0.0f;
        m_borderWidth = width;
        m_sbo.SetParam("uBorderWidth", m_borderWidth);
    }

    void ElementBase::setDuration(float duration) {
        if (duration < 0.0f) duration = 0.0f;
        m_duration = duration;
    }

    const Vector4& ElementBase::getBackgroundColor() const {
        return m_backgroundColor;
    }

    const Vector4& ElementBase::getBorderColor() const {
        return m_borderColor;
    }

    const Vector4& ElementBase::getBorderRadius() const {
        return m_borderRadius;
    }

    float ElementBase::getBorderWidth() const {
        return m_borderWidth;
    }

    float ElementBase::getDuration() const {
        return m_duration;
    }

    bool ElementBase::IsMouseOver(const Vector2& mousePos) const {
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

    void ElementBase::RegisterAttribute(const std::string& name, std::function<void(ElementBase*, const StyleValue&)> func) {
        m_registeredAttributes[FormatUtils::toLowerCase(name)] = func;
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

        if (m_parentElementPtr) {
            m_worldMatrix = m_parentElementPtr->GetWorldModelMatrix() * m_localMatrix;
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

    void ElementBase::WindowResize(int width, int height) {
        // update position to new window position
        SetStyleAttributes();
    }

    void ElementBase::UpdateImpl() {
        Update();
    }

    void ElementBase::SendDrawCommandImpl(Renderer* renderer, RenderLayerID renderLayerID) {
        // MarkDirty();
        // Log::Info("local Pos: {}; screen pos: {}; size: {}", GetLocalPosition(), GetScreenPosition(), GetLocalSize());
        // Log::Print(*GetWorldModelMatrixPtr());

        m_cmd.renderLayerID = renderLayerID;
        m_cmd.modelMatrix = GetWorldModelMatrixPtr();
        m_cmd.shaderBindOverride = &m_sbo;
        std::string name = m_style->GetName();
        if (name == "style2")
            m_cmd.zOrder = 10;
        SendDrawCommand(renderer);
    }

    void ElementBase::RegisterAttributesImpl() {
        if (m_attributesRegistered)
            return;
        m_attributesRegistered = true;
        {
            namespace att = Attribute;
            RegisterAttribute(att::width, [](ElementBase* el, const StyleValue& val) {
                if (float f; val.TryGetValue<float>(f, att::width)) {
                    el->SetLocalScaleX(f);
                }
            });

            namespace att = Attribute;
            RegisterAttribute(att::height, [](ElementBase* el, const StyleValue& val) {
                if (float f; val.TryGetValue<float>(f, att::height)) {
                    el->SetLocalScaleY(f);
                }
            });

            RegisterAttribute(att::backgroundColor, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::backgroundColor)) {
                    el->setBackgroundColor(vec);
                }
            });

            RegisterAttribute(att::borderColor, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::borderColor)) {
                    el->setBorderColor(vec);
                }
            });

            RegisterAttribute(att::borderRadius, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::borderRadius)) {
                    el->setBorderRadius(vec);
                }
            });

            RegisterAttribute(att::borderWidth, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::borderWidth)) {
                    el->setBorderWidth(f);
                }
            });
        }
       
        RegisterAttributes();
    }

    void ElementBase::SetStyleAttributes() {
        const auto& attribute = m_style->GetAllState(m_state);

        for (auto& [name, valueStr] : attribute) {
            const StyleValue& value = StyleAttribute::GetAttributeValue(name, *this, valueStr);
            auto it = m_registeredAttributes.find(FormatUtils::toLowerCase(name));
            if (it != m_registeredAttributes.end()) {
                it->second(this, value);
            }
        }
    }

}