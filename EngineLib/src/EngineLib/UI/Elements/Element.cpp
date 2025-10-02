#include <CoreLib/ConversionUtils.h>
#include <CoreLib/FormatUtils.h>


#include "EngineLib/UI/UIManager.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"

namespace EngineCore::UI {

    // Returns a style with default values. NEEDS to be extended if new default attributes are added
    std::shared_ptr<Style> ElementBase::GetElementBaseStyle() {
        static std::shared_ptr<Style> baseStyle = [] {
            auto s = std::make_shared<Style>("Base");
            namespace a = Attribute;

            #pragma region Layout

            s->Set(a::layoutHor, "start");
            s->Set(a::layoutVer, "start");

            s->Set(a::layoutContentHor, "start");
            s->Set(a::layoutContentVer, "start");

            s->Set(a::layoutDirection, "row");
            s->Set(a::layoutWrap, "none");

            #pragma endregion

            #pragma region Display

            s->Set(a::visibility, "visible");
            s->Set(a::display, "display");
            s->Set(a::overflow, "none");

            #pragma endregion

            #pragma region Style

            s->Set(a::width, "0px");
            s->Set(a::height, "0px");

            s->Set(a::marginTop, "0px");
            s->Set(a::marginBottom, "0px");
            s->Set(a::marginLeft, "0px");
            s->Set(a::marginRight, "0px");

            s->Set(a::paddingTop, "0px");
            s->Set(a::paddingBottom, "0px");
            s->Set(a::paddingLeft, "0px");
            s->Set(a::paddingRight, "0px");

            s->Set(a::backgroundColor, "#ff");
            s->Set(a::textColor, "#ff");
            s->Set(a::borderColor, "#00");
            s->Set(a::borderWidth, "0px");
            s->Set(a::borderRadius, "0px");
            s->Set(a::duration, "0s");

            #pragma endregion
            return s;
        }();

        ExtendElementBaseStyle(baseStyle);
        return baseStyle;
    }

	ElementBase::ElementBase(std::string name, UIElementID id, std::shared_ptr<Style> style, MaterialID matID)
		: m_elementName(std::move(name)), m_id(id), m_style(std::move(style)){
        m_cmd.isUI = true;
        m_cmd.type = RenderCommandType::Mesh;
        m_cmd.meshID = ASSETS::ENGINE::MESH::UIPlain();
        m_cmd.materialID = matID;
        m_baseStyle = GetElementBaseStyle();
	}

    ElementBase::ElementBase(std::string name, UIElementID id, MaterialID matID, std::shared_ptr<Style> style) 
        : m_elementName(std::move(name)), m_id(id), m_style(std::move(style)) {
        m_cmd.isUI = true;
        m_cmd.type = RenderCommandType::Mesh;
        m_cmd.meshID = ASSETS::ENGINE::MESH::Plain();
        m_cmd.materialID = matID;
        m_baseStyle = GetElementBaseStyle();
    }

    void ElementBase::Init() {
        // inits the start propetys
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

    Vector2 ElementBase::GetScreenPosition() {
        if (m_positionDirty) {
            UpdateLayoutPosition();
            m_positionDirty = false;
        }

        Vector2 pos = m_innerPosition;
        if (m_parentElementPtr) {
            pos += m_parentElementPtr->GetScreenPosition();
        }
        return pos;
    }

    Vector2 ElementBase::GetLocalPosition() {
        if (m_positionDirty) {
            UpdateLayoutPosition();
            m_positionDirty = false;
        }
        return m_innerPosition;
    }
    
    Vector2 ElementBase::GetScreenSize() {
        if (m_sizeDirty) {
            UpdateLayoutSize();
            m_sizeDirty = false;
        }

        Vector2 size = m_innerPosition;
        if (UIManager::GetUIScaling()) {
            size *= UIManager::GetUIScaleFactor();
        }
        return size;
    }

    Vector2 ElementBase::GetLocalSize() {
        if (m_sizeDirty) {
            UpdateLayoutSize();
            m_sizeDirty = false;
        }
        return m_innerPosition;
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

    void ElementBase::SetLayoutDirection(LayoutDirection d) { 
        m_layoutDirection = d;
        MarkDirty();
    }

    void ElementBase::SetLayoutWrap(LayoutWrap w) { 
        m_layoutWrap = w;
        MarkDirty();
    }

    void ElementBase::SetLayoutContentHor(LayoutAlign a) { 
        m_layoutContentHor = a; 
        MarkDirty();
    }

    void ElementBase::SetLayoutContentVer(LayoutAlign a) { 
        m_layoutContentVer = a;
        MarkDirty();
    }

    void ElementBase::SetLocalPosition(const Vector2& pos) {
        SetLocalPosition(pos.x, pos.y);
    }

    void ElementBase::SetLocalPosition(float x, float y) {
        m_innerPosition.Set(x,y);
        MarkDirty();
    }

    void ElementBase::SetLocalRotation(const Vector3& rotation) {
        m_rotation = rotation;
        MarkDirty();
    }

    void ElementBase::SetLocalRotation(float x, float y, float z) {
        m_rotation.Set(x, y, z);
        MarkDirty();
    }

    void ElementBase::SetLocalSize(const Vector2& scale) {
        SetLocalSize(scale.x, scale.y);
    }

    void ElementBase::SetLocalSize(float x, float y) {
        m_innerSize.Set(x, y);
        m_sbo.SetParam("uSize", m_innerSize);
        MarkDirty();
    }

    void ElementBase::SetLocalWidth(float x) {
        m_innerSize.x = x;
        m_sbo.SetParam("uSize", m_innerSize);
        MarkDirty();
    }

    void ElementBase::SetLocalHeight(float y) {
        m_innerSize.y = y;
        m_sbo.SetParam("uSize", m_innerSize);
        MarkDirty();
    }

    void ElementBase::SetBackgroundColor(const Vector4& color) {
        m_backgroundColor = color;
        m_sbo.SetParam("uBackgroundColor", m_backgroundColor);
    }

    void ElementBase::SetBorderColor(const Vector4& color) {
        m_borderColor = color;
        m_sbo.SetParam("uBorderColor", m_borderColor);
    }

    void ElementBase::SetBorderRadius(const Vector4& radius) {
        m_borderRadius = radius;
        m_sbo.SetParam("uBorderRadius", m_borderRadius);
    }

    void ElementBase::SetBorderWidth(float width) {
        if (width < 0.0f) width = 0.0f;
        m_borderSize.Set(width, width, width, width);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
    }

    void ElementBase::SetBorderTop(float top) {
        if (top < 0.0f) top = 0.0f;
        m_borderSize.x = top;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
    }

    void ElementBase::SetBorderLeft(float left) {
        if (left < 0.0f) left = 0.0f;
        m_borderSize.y = left;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
    }

    void ElementBase::SetBorderBottom(float bottom) {
        if (bottom < 0.0f) bottom = 0.0f;
        m_borderSize.z = bottom;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
    }

    void ElementBase::SetBorderRight(float right) {
        if (right < 0.0f) right = 0.0f;
        m_borderSize.w = right;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
    }

    void ElementBase::SetBorderSize(float width, float height) {
        if (width < 0.0f) width = 0.0f;
        if (height < 0.0f) height = 0.0f;
        m_borderSize.Set(height, width, height, width);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
    }

    void ElementBase::SetBorderSize(const Vector4& vec) {
        SetBorderSize(vec.x, vec.y, vec.z, vec.w);
    }

    void ElementBase::SetBorderSize(float top, float right, float bottom, float left) {
        if (top < 0.0f) top = 0.0f;
        if (right < 0.0f) right = 0.0f;
        if (bottom < 0.0f) bottom = 0.0f;
        if (left < 0.0f) left = 0.0f;
        m_borderSize.Set(top, right, bottom, left);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
    }

    void ElementBase::SetDuration(float duration) {
        if (duration < 0.0f) duration = 0.0f;
        m_duration = duration;
    }

    LayoutDirection ElementBase::GetLayoutDirection() const {
        return m_layoutDirection;
    }

    LayoutWrap ElementBase::GetLayoutWrap() const {
        return m_layoutWrap;
    }

    LayoutAlign ElementBase::GetLayoutContentHor() const {
        return m_layoutContentHor;
    }

    LayoutAlign ElementBase::GetLayoutContentVer() const {
        return m_layoutContentVer;
    }

    Vector2 ElementBase::GetContentSize() {
        if (m_sizeDirty) {
            UpdateLayoutSize();
        }
        return m_innerSize;
    }

    Vector2 ElementBase::GetPaddingSize() {
        Vector2 content = GetContentSize();
        return Vector2(
            content.x + m_padding.x + m_padding.z, // left + right
            content.y + m_padding.y + m_padding.w  // top + bottom
        );
    }

    Vector2 ElementBase::GetBorderSize() {
        Vector2 padded = GetPaddingSize();
        return Vector2(
            padded.x + m_borderSize.x + m_borderSize.z, // left + right
            padded.y + m_borderSize.y + m_borderSize.w  // top + bottom
        );
    }

    Vector2 ElementBase::GetMarginSize() {
        Vector2 bordered = GetBorderSize();
        return Vector2(
            bordered.x + m_margin.x + m_margin.z, // left + right
            bordered.y + m_margin.y + m_margin.w  // top + bottom
        );
    }

    const Vector4& ElementBase::GetBackgroundColor() const {
        return m_backgroundColor;
    }

    const Vector4& ElementBase::GetBorderColor() const {
        return m_borderColor;
    }

    const Vector4& ElementBase::GetBorderRadius() const {
        return m_borderRadius;
    }

    const Vector4& ElementBase::GetBorderSize() const {
        return m_borderSize;
    }

    float ElementBase::GetBorderTop() const {
        return m_borderSize.x;
    }

    float ElementBase::GetBorderLeft() const {
        return m_borderSize.y;
    }

    float ElementBase::GetBorderBottom() const {
        return m_borderSize.z;
    }

    float ElementBase::GetBorderRight() const {
        return m_borderSize.w;
    }

    float ElementBase::GetDuration() const {
        return m_duration;
    }

    bool ElementBase::IsMouseOver(const Vector2& mousePos) {
        Vector2 pos = GetScreenPosition();
        Vector2 size = GetScreenSize();
        return (mousePos.x > pos.x && pos.x + size.x > mousePos.x &&
                mousePos.y > pos.y && pos.y + size.y > mousePos.y);
    }

    Matrix4x4* ElementBase::GetWorldModelMatrixPtr() {
        if (m_worldTransformDirty) {
            UpdateWorldTransform();
        }

        return &m_worldTransform;
    }

    const Matrix4x4& ElementBase::GetWorldModelMatrix() {
        if (m_worldTransformDirty) {
            UpdateWorldTransform();
        }

        return m_worldTransform;
    }

    void ElementBase::RegisterAttribute(const std::string& name, std::function<void(ElementBase*, const StyleValue&)> func) {
        m_registeredAttributes[FormatUtils::toLowerCase(name)] = func;
    }

    void ElementBase::UpdateLayoutPosition() {
        m_positionDirty = false;
    }

    void ElementBase::UpdateLayoutSize() {
        if (m_parentElementPtr) {
            switch (m_parentElementPtr->GetLayoutDirection()) {
            case LayoutDirection::RowStart:
            case LayoutDirection::RowEnd:
                // In Row-Layout strecken Kinder ggf. in Y-Richtung
                if (m_innerSize.y <= 0.0f &&
                    m_parentElementPtr->GetLayoutContentVer() == LayoutAlign::Stretch) {
                    m_innerSize.y = m_parentElementPtr->GetContentSize().y;
                }
                break;
            case LayoutDirection::ColumnStart:
            case LayoutDirection::ColumnEnd:
                // In Column-Layout strecken Kinder ggf. in X-Richtung
                if (m_innerSize.x <= 0.0f &&
                    m_parentElementPtr->GetLayoutContentHor() == LayoutAlign::Stretch) {
                    m_innerSize.x = m_parentElementPtr->GetContentSize().x;
                }
                break;
            }
        }

        m_sizeDirty = false;
    }

    void ElementBase::UpdateLocalTransform() {
        using namespace GLTransform4x4;

        if (m_positionDirty) {
            UpdateLayoutPosition();
        }

        if (m_sizeDirty) {
            UpdateLayoutSize();
        }

        Vector3 radians = {
            ConversionUtils::ToRadians(m_rotation.x),
            ConversionUtils::ToRadians(m_rotation.y),
            ConversionUtils::ToRadians(m_rotation.z)
        };

        m_localTransform = Scale(m_innerSize.x, m_innerSize.y, 1.0f);
        MakeRotateXYZ(m_localTransform, radians);
        MakeTranslate(m_localTransform, m_innerPosition.x, m_innerPosition.y, 0.0f);

        m_localTransformDirty = false;
    }

    void ElementBase::UpdateWorldTransform() {
        using namespace GLTransform4x4;

        if (m_positionDirty) {
            UpdateLayoutPosition();
        }

        if (m_sizeDirty) {
            UpdateLayoutSize();
        }

        if (m_localTransformDirty) {
            UpdateLocalTransform();
        }

        if (m_parentElementPtr) {
            using namespace GLTransform4x4;

            Vector3 radians = {
                ConversionUtils::ToRadians(m_rotation.x),
                ConversionUtils::ToRadians(m_rotation.y),
                ConversionUtils::ToRadians(m_rotation.z)
            };

            auto vec = m_parentElementPtr->GetLocalSize();
            m_worldTransform = Scale(m_innerSize.x / vec.x, m_innerSize.y / vec.y, 1.0f);
            MakeRotateXYZ(m_worldTransform, radians);
            MakeTranslate(m_worldTransform, m_innerPosition.x, m_innerPosition.y, 0.0f);

            m_worldTransform = m_parentElementPtr->GetWorldModelMatrix() * m_worldTransform;
        }
        else {
            m_worldTransform = m_localTransform;
        }

        m_worldTransformDirty = false;
    }

    void ElementBase::MarkDirty() {
        m_positionDirty = true;
        m_sizeDirty = true;
        m_localTransformDirty = true;
        m_worldTransformDirty = true;
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
        m_cmd.renderLayerID = renderLayerID;
        m_cmd.modelMatrix = GetWorldModelMatrixPtr();
        m_cmd.shaderBindOverride = &m_sbo;
        SendDrawCommand(renderer);
    }

    void ElementBase::RegisterAttributesImpl() {
        if (m_attributesRegistered)
            return;
        m_attributesRegistered = true;
        {
            namespace att = Attribute;
            RegisterAttribute(att::layoutDirection, [](ElementBase* el, const StyleValue& val) {
                if (std::string dir; val.TryGetValue<std::string>(dir, att::layoutDirection)) {
                    el->SetLayoutDirection(ToLayoutDirection(dir));
                }
            });

            RegisterAttribute(att::layoutWrap, [](ElementBase* el, const StyleValue& val) {
                if (std::string wrap; val.TryGetValue<std::string>(wrap, att::layoutWrap)) {
                    el->SetLayoutWrap(ToLayoutWrap(wrap));
                }
            });

            RegisterAttribute(att::layoutContentHor, [](ElementBase* el, const StyleValue& val) {
                if (std::string align; val.TryGetValue<std::string>(align, att::layoutContentHor)) {
                    el->SetLayoutContentHor(ToLayoutAlign(align));
                }
            });

            RegisterAttribute(att::layoutContentVer, [](ElementBase* el, const StyleValue& val) {
                if (std::string align; val.TryGetValue<std::string>(align, att::layoutContentVer)) {
                    el->SetLayoutContentVer(ToLayoutAlign(align));
                }
            });

            RegisterAttribute(att::width, [](ElementBase* el, const StyleValue& val) {
                if (float f; val.TryGetValue<float>(f, att::width)) {
                    el->SetLocalWidth(f);
                }
            });

            RegisterAttribute(att::height, [](ElementBase* el, const StyleValue& val) {
                if (float f; val.TryGetValue<float>(f, att::height)) {
                    el->SetLocalHeight(f);
                }
            });

            RegisterAttribute(att::backgroundColor, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::backgroundColor)) {
                    el->SetBackgroundColor(vec);
                }
            });

            RegisterAttribute(att::borderColor, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::borderColor)) {
                    el->SetBorderColor(vec);
                }
            });

            RegisterAttribute(att::borderRadius, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::borderRadius)) {
                    el->SetBorderRadius(vec);
                }
            });

            RegisterAttribute(att::borderSize, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::borderSize)) {
                    el->SetBorderSize(vec);
                }
            });

            RegisterAttribute(att::borderWidth, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::borderWidth)) {
                    el->SetBorderWidth(f);
                }
            });

            RegisterAttribute(att::borderTop, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::borderTop)) {
                    el->SetBorderTop(f);
                }
            });

            RegisterAttribute(att::borderLeft, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::borderLeft)) {
                    el->SetBorderLeft(f);
                }
            });

            RegisterAttribute(att::borderBottom, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::borderBottom)) {
                    el->SetBorderBottom(f);
                }
            });

            RegisterAttribute(att::borderRight, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::borderRight)) {
                    el->SetBorderRight(f);
                }
            });
        }
       
        RegisterAttributes();
    }

    void ElementBase::SetStyleAttributes() {
        SetAttributes(m_baseStyle->GetAllState(m_state));
        SetAttributes(m_style->GetAllState(m_state));
    }

    void ElementBase::SetAttributes(const std::unordered_map<std::string, std::string>& attribute) {
        for (auto& [name, valueStr] : attribute) {
            const StyleValue& value = StyleAttribute::GetAttributeValue(name, *this, valueStr);
            auto it = m_registeredAttributes.find(FormatUtils::toLowerCase(name));
            if (it != m_registeredAttributes.end()) {
                it->second(this, value);
            }
        }
    }

}