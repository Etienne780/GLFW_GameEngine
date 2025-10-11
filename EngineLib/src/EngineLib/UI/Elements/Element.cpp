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
        m_baseStyle = UIManager::GetElementBaseStyle();

        // subs to style dirty events
        m_styleDirtyCallbackID = m_style->SubDirtCallback([this]() { SetStyleAttributes(); });
        m_baseStyleDirtyCallbackID = m_baseStyle->SubDirtCallback([this]() { SetStyleAttributes(); });
	}

    ElementBase::~ElementBase() {
        // unsubs to style dirty events
        m_style->UnsubDirtyCallback(m_styleDirtyCallbackID);
        m_baseStyle->UnsubDirtyCallback(m_baseStyleDirtyCallbackID);
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

    std::shared_ptr<Style> ElementBase::GetElementStyle() const {
        return m_baseStyle;
    }

    Vector2 ElementBase::GetWorldPosition() {
        Vector2 pos = m_layoutPosition;
        if (m_parentElementPtr) {
            pos += m_parentElementPtr->GetWorldPosition();
        }
        return pos;
    }

    Vector2 ElementBase::GetLocalPosition() {
        return m_layoutPosition;
    }

    Vector2 ElementBase::GetScreenSize() {
        if (m_sizeDirty) {
            UpdateLayoutSize();
        }

        Vector2 size = m_layoutSize;
        if (UIManager::GetUIScaling()) {
            size *= UIManager::GetUIScaleFactor();
        }
        return size;
    }

    Vector2 ElementBase::GetLocalSize() {
        if (m_sizeDirty) {
            UpdateLayoutSize();
        }
        return m_layoutSize;
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


    size_t ElementBase::GetChildCount() const {
        return m_children.size();
    }

    std::shared_ptr<ElementBase> ElementBase::GetChild(size_t index) {
        if (index >= m_children.size()) {
            Log::Error("ElementBase: Child index '{}' was out of bounds for list({})", 
                index, m_children.size());
            static std::shared_ptr<ElementBase> dummy = std::make_shared<ElementBase>("dummy", UIElementID(ENGINE_INVALID_ID));
            return dummy;
        }
        return m_children[index];
    }

    std::vector<std::shared_ptr<ElementBase>>& ElementBase::GetChildren() {
        return m_children; 
    }

    const std::vector<std::shared_ptr<ElementBase>>& ElementBase::GetChildren() const {
        return m_children;
    }

    ElementBase* ElementBase::GetParent() const {
        return m_parentElementPtr;
    }

    size_t ElementBase::GetListPosition() const {
        return m_listPosition;
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

    void ElementBase::SetLayoutType(LayoutType type) {
        if (m_layoutType == type)
            return;
        m_layoutType = type;
        MarkDirtyParent();
    }

    void ElementBase::SetLayoutDirection(Flex::LayoutDirection dir) {
        if (m_layoutDirection == dir)
            return;
        m_layoutDirection = dir;
        MarkDirtyParent();
    }

    void ElementBase::SetLayoutWrap(Flex::LayoutWrap wra) {
        if (m_layoutWrap == wra)
            return;
        m_layoutWrap = wra;
        MarkDirtyParent();
    }

    void ElementBase::SetLayoutMajor(Flex::LayoutAlign ali) {
        if (m_layoutMajor == ali)
            return;
        m_layoutMajor = ali;
        MarkDirtyParent();
    }

    void ElementBase::SetLayoutMinor(Flex::LayoutAlign ali) {
        if (m_layoutMinor == ali)
            return;
        m_layoutMinor = ali;
        MarkDirtyParent();
    }

    void ElementBase::SetLayoutItem(Flex::LayoutAlign ali) {
        if (m_layoutItem == ali)
            return;
        m_layoutItem = ali;
        MarkDirtyParent();
    }

    void ElementBase::SetLocalPosition(const Vector2& pos) {
        SetLocalPosition(pos.x, pos.y);
    }

    void ElementBase::SetLocalPosition(float x, float y) {
        if (m_desiredPosition.x == x && 
            m_desiredPosition.y == y)
            return;
        m_desiredPosition.Set(x, y);
        MarkDirtyParent();
    }

    void ElementBase::SetLocalRotation(const Vector3& rotation) {
        SetLocalRotation(rotation.x, rotation.y, rotation.z);
    }

    void ElementBase::SetLocalRotation(float x, float y, float z) {
        if (m_rotation.x == x &&
            m_rotation.y == y &&
            m_rotation.z == z)
            return;
        m_rotation.Set(x, y, z);
        MarkDirtyParent();
    }

    void ElementBase::SetDesiredSize(const Vector2& scale) {
        SetDesiredSize(scale.x, scale.y);
    }

    void ElementBase::SetDesiredSize(float x, float y) {
        if (x < 0.0f) x = 0.0f;
        if (y < 0.0f) y = 0.0f;
        if (m_desiredSize.x == x &&
            m_desiredSize.y == y)
            return;
        m_desiredSize.Set(x, y);
        m_aviableSize.Set(-1, -1);
        MarkDirtyParent();
    }

    void ElementBase::SetDesiredWidth(float x) {
        if (x < 0.0f) x = 0.0f;
        if (m_desiredSize.x == x)
            return;
        m_desiredSize.x = x;
        m_aviableSize.x = -1;
        MarkDirtyParent();
    }

    void ElementBase::SetDesiredHeight(float y) {
        if (y < 0.0f) y = 0.0f;
        if (m_desiredSize.y == y)
            return;
        m_desiredSize.y = y;
        m_aviableSize.y = -1;
        MarkDirtyParent();
    }

    void ElementBase::SetBackgroundColor(const Vector4& color) {
        if (m_backgroundColor == color)
            return;
        m_backgroundColor = color;
        m_sbo.SetParam("uBackgroundColor", m_backgroundColor);
    }

    void ElementBase::SetBorderColor(const Vector4& color) {
        if (m_borderColor == color)
            return;
        m_borderColor = color;
        m_sbo.SetParam("uBorderColor", m_borderColor);
    }

    void ElementBase::SetBorderRadius(const Vector4& radius) {
        if (m_borderRadius == radius)
            return;
        m_borderRadius = radius;
        m_sbo.SetParam("uBorderRadius", m_borderRadius);
    }

    void ElementBase::SetBorderWidth(float width) {
        if (width < 0.0f) width = 0.0f;
        if (m_borderSize == Vector4(width))
            return;
        m_borderSize.Set(width);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkDirtyParent();
    }

    void ElementBase::SetBorderTop(float top) {
        if (top < 0.0f) top = 0.0f;
        if (m_borderSize.x == top)
            return;
        m_borderSize.x = top;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkDirtyParent();
    }

    void ElementBase::SetBorderRight(float right) {
        if (right < 0.0f) right = 0.0f;
        if (m_borderSize.y == right)
            return;
        m_borderSize.y = right;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkDirtyParent();
    }

    void ElementBase::SetBorderBottom(float bottom) {
        if (bottom < 0.0f) bottom = 0.0f;
        if (m_borderSize.z == bottom)
            return;
        m_borderSize.z = bottom;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkDirtyParent();
    }

    void ElementBase::SetBorderLeft(float left) {
        if (left < 0.0f) left = 0.0f;
        if (m_borderSize.w == left)
            return;
        m_borderSize.w = left;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkDirtyParent();
    }

    void ElementBase::SetBorderSize(const Vector4& vec) {
        SetBorderSize(vec.x, vec.y, vec.z, vec.w);
        MarkDirtyParent();
    }

    void ElementBase::SetBorderSize(float hor, float ver) {
        if (hor < 0.0f) hor = 0.0f;
        if (ver < 0.0f) ver = 0.0f;
        if (m_borderSize == Vector4(ver, hor, ver, hor))
            return;
        m_borderSize.Set(ver, hor, ver, hor);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkDirtyParent();
    }

    void ElementBase::SetBorderSize(float top, float right, float bottom, float left) {
        if (top < 0.0f) top = 0.0f;
        if (right < 0.0f) right = 0.0f;
        if (bottom < 0.0f) bottom = 0.0f;
        if (left < 0.0f) left = 0.0f;
        if (m_borderSize == Vector4(top, right, bottom, left))
            return;
        m_borderSize.Set(top, right, bottom, left);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkDirtyParent();
    }

    void ElementBase::SetMargin(const Vector4& mar) {
        SetMargin(mar.x, mar.y, mar.z, mar.w);
    }

    void ElementBase::SetMargin(float top, float right, float bottom, float left) {
        if (top < 0.0f) top = 0.0f;
        if (right < 0.0f) right = 0.0f;
        if (bottom < 0.0f) bottom = 0.0f;
        if (left < 0.0f) left = 0.0f;
        if (m_margin == Vector4(top, right, bottom, left))
            return;
        m_margin.Set(top, right, bottom, left);
        MarkDirtyParent();
    }

    void ElementBase::SetMarginTop(float top) {
        if (top < 0.0f) top = 0.0f;
        if (m_margin.x == top)
            return;
        m_margin.x = top;
        MarkDirtyParent();
    }

    void ElementBase::SetMarginRight(float right) {
        if (right < 0.0f) right = 0.0f;
        if (m_margin.y == right)
            return;
        m_margin.y = right;
        MarkDirtyParent();
    }

    void ElementBase::SetMarginBottom(float bottom) {
        if (bottom < 0.0f) bottom = 0.0f;
        if (m_margin.z == bottom)
            return;
        m_margin.z = bottom;
        MarkDirtyParent();
    }

    void ElementBase::SetMarginLeft(float left) {
        if (left < 0.0f) left = 0.0f;
        if (m_margin.w == left)
            return;
        m_margin.w = left;
        MarkDirtyParent();
    }

    void ElementBase::SetPadding(const Vector4& pad) {
        SetPadding(pad.x, pad.y, pad.z, pad.w);
    }

    void ElementBase::SetPadding(float top, float right, float bottom, float left) {
        if (top < 0.0f) top = 0.0f;
        if (right < 0.0f) right = 0.0f;
        if (bottom < 0.0f) bottom = 0.0f;
        if (left < 0.0f) left = 0.0f;
        if (m_padding == Vector4(top, right, bottom, left))
            return;
        m_padding.Set(top, right, bottom, left);
        MarkDirtyParent();
    }

    void ElementBase::SetPaddingTop(float top) {
        if (top < 0.0f) top = 0.0f;
        if (m_padding.x == top)
            return;
        m_padding.x = top;
        MarkDirtyParent();
    }

    void ElementBase::SetPaddingRight(float right) {
        if (right < 0.0f) right = 0.0f;
        if (m_padding.y == right)
            return;
        m_padding.y = right;
        MarkDirtyParent();
    }

    void ElementBase::SetPaddingBottom(float bottom) {
        if (bottom < 0.0f) bottom = 0.0f;
        if (m_padding.z == bottom)
            return;
        m_padding.z = bottom;
        MarkDirtyParent();
    }

    void ElementBase::SetPaddingLeft(float left) {
        if (left < 0.0f) left = 0.0f;
        if (m_padding.w == left)
            return;
        m_padding.w = left;
        MarkDirtyParent();
    }

    void ElementBase::SetDuration(float duration) {
        if (duration < 0.0f) duration = 0.0f;
        if (m_duration == duration)
            return;
        m_duration = duration;
    }

    LayoutType ElementBase::GetLayoutType() const {
        return m_layoutType;
    }

    Flex::LayoutDirection ElementBase::GetLayoutDirection() const {
        return m_layoutDirection;
    }

    Flex::LayoutWrap ElementBase::GetLayoutWrap() const {
        return m_layoutWrap;
    }

    Flex::LayoutAlign ElementBase::GetLayoutMajor() const {
        return m_layoutMajor;
    }

    Flex::LayoutAlign ElementBase::GetLayoutMinor() const {
        return m_layoutMinor;
    }

    Flex::LayoutAlign ElementBase::GetLayoutItem() const {
        return m_layoutItem;
    }

    Vector2 ElementBase::GetDesiredPosition() const {
        return m_desiredPosition;
    }

    Vector2 ElementBase::GetAviableSize() const {
        return m_aviableSize;
    }

    Vector2 ElementBase::GetContentSize() {
        Vector2 size = GetLocalSize();
        return Vector2(
            size.x - m_borderSize.y - m_borderSize.w,
            size.y - m_borderSize.x - m_borderSize.z
        );
    }

    Vector2 ElementBase::GetDesiredSize() const {
        return m_desiredSize;
    }

    Vector2 ElementBase::GetSizeWithBorder() {
        Vector2 size = GetLocalSize();
        return Vector2(
            size.x + m_borderSize.y + m_borderSize.w, // left + right
            size.y + m_borderSize.x + m_borderSize.z  // top + bottom
        );
    }

    Vector2 ElementBase::GetMarginSize() {
        Vector2 bordered = GetSizeWithBorder();
        return Vector2(
            bordered.x + m_margin.y + m_margin.w, // left + right
            bordered.y + m_margin.x + m_margin.z  // top + bottom
        );
    }

    Vector3 ElementBase::GetWorldRotation() const {
        return m_rotation;
    }

    const Vector4& ElementBase::GetMargin() const {
        return m_margin;
    }

    float ElementBase::GetMarginTop() const {
        return m_margin.x;
    }

    float ElementBase::GetMarginRight() const {
        return m_margin.y;
    }

    float ElementBase::GetMarginBottom() const {
        return m_margin.z;
    }

    float ElementBase::GetMarginLeft() const {
        return m_margin.w;
    }

    const Vector4& ElementBase::GetPadding() const {
        return m_padding;
    }

    float ElementBase::GetPaddingTop() const {
        return m_padding.x;
    }

    float ElementBase::GetPaddingRigth() const {
        return m_padding.y;
    }

    float ElementBase::GetPaddingBottom() const {
        return m_padding.z;
    }

    float ElementBase::GetPaddingLeft() const {
        return m_padding.w;
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

    float ElementBase::GetBorderRight() const {
        return m_borderSize.y;
    }

    float ElementBase::GetBorderBottom() const {
        return m_borderSize.z;
    }

    float ElementBase::GetBorderLeft() const {
        return m_borderSize.w;
    }

    float ElementBase::GetDuration() const {
        return m_duration;
    }

    bool ElementBase::IsMouseOver(const Vector2& mousePos) {
        Vector2 pos = GetWorldPosition();
        Vector2 size = GetScreenSize();
        return (mousePos.x > pos.x && pos.x + size.x > mousePos.x &&
                mousePos.y > pos.y && pos.y + size.y > mousePos.y);
    }

    Matrix4x4* ElementBase::GetWorldModelMatrixPtr() {
        return &m_worldTransform;
    }

    const Matrix4x4& ElementBase::GetWorldModelMatrix() {
        return m_worldTransform;
    }

    void ElementBase::RegisterAttribute(const std::string& name, std::function<void(ElementBase*, const StyleValue&)> func) {
        m_registeredAttributes[FormatUtils::toLowerCase(name)] = func;
    }

    void ElementBase::UpdateLayoutPosition() {
        if (!m_parentElementPtr) {
            m_layoutPosition = m_desiredPosition;
            return;
        }
        // resets position for consistency
        m_layoutPosition.Set(0, 0);

        switch (m_parentElementPtr->GetLayoutType()) {
        case LayoutType::Flex:
            m_layoutPosition = s_flexCalculator.CalculatePosition(this);
            break;
        case LayoutType::Grid:
            m_layoutPosition = s_gridCalculator.CalculatePosition(this);
            break;
        case LayoutType::None:
            break;
        }

        m_positionDirty = false;
    }

    void ElementBase::UpdateLayoutSize() {
        if (!m_parentElementPtr) {
            m_layoutSize = m_desiredSize;
            return;
        }
        // resets size for consistency
        m_layoutSize.Set(0, 0);

        switch (m_parentElementPtr->GetLayoutType()) {
        case LayoutType::Flex:
            m_layoutSize = s_flexCalculator.CalculateSize(this);
            break;
        case LayoutType::Grid:
            m_layoutSize = s_gridCalculator.CalculateSize(this);
            break;
        case LayoutType::None:
            m_padding.Set(0, 0, 0, 0);
            m_margin.Set(0, 0, 0, 0);
            break;
        }

        m_sbo.SetParam("uSize", m_layoutSize);

        // update position if size changes
        m_positionDirty = true;
    }

    void ElementBase::UpdateWorldTransform() {
        using namespace GLTransform4x4;

        Vector2 parentPosition;
        Vector3 parentRotation;
        if (m_parentElementPtr) {
            parentPosition = m_parentElementPtr->GetWorldPosition();
            parentRotation = m_parentElementPtr->GetWorldRotation();
        }

        Vector3 radians = {
            ConversionUtils::ToRadians(m_rotation.x + parentRotation.x),
            ConversionUtils::ToRadians(m_rotation.y + parentRotation.y),
            ConversionUtils::ToRadians(m_rotation.z + parentRotation.z)
        };

        // m_borderSize;
        m_worldTransform = Scale(
            m_layoutSize.x, 
            m_layoutSize.y, 1.0f
        );
        MakeRotateXYZ(m_worldTransform, radians);
        MakeTranslate(m_worldTransform, 
            m_layoutPosition.x + parentPosition.x + m_margin.y,
            m_layoutPosition.y + parentPosition.y + m_margin.x, 0.0f);

        m_worldTransformDirty = false;
    }

    void ElementBase::MarkDirtyParent() const {
        //Marke the parent as dirty so it updates alle the child elements
        // z.b. if width of this el changed. than the positions needs to recalculatet
        if (m_parentElementPtr) {
            m_parentElementPtr->MarkDirty();
            return;
        }

        UIManager::SetRootElementsDirty();
    }

    void ElementBase::MarkDirty() const {
        m_positionDirty = true;
        m_sizeDirty = true;
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
        // Update ui elements
        // IMPORTANT: update position after size
        if (m_sizeDirty) {
            UpdateLayoutSize();
            m_sizeDirty = false;
        }

        if (m_positionDirty) {
            UpdateLayoutPosition();
            m_positionDirty = false;
        }

        if (m_worldTransformDirty) {
            UpdateWorldTransform();
            m_worldTransformDirty = false;
        }

        Update();
    }

    void ElementBase::SendDrawCommandImpl(Renderer* renderer, RenderLayerID renderLayerID) {

        // Log::Info("ID: {}, style: {}, baseStyle: {}", m_id.value, m_style.get(), m_baseStyle.get());

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
            m_layoutType;
            RegisterAttribute(att::layoutType, [](ElementBase* el, const StyleValue& val) {
                if (std::string type; val.TryGetValue<std::string>(type, att::layoutType)) {
                    el->SetLayoutType(ToLayoutType(type));
                }
            });

            RegisterAttribute(att::layoutDirection, [](ElementBase* el, const StyleValue& val) {
                if (std::string dir; val.TryGetValue<std::string>(dir, att::layoutDirection)) {
                    el->SetLayoutDirection(Flex::ToLayoutDirection(dir));
                }
            });

            RegisterAttribute(att::layoutWrap, [](ElementBase* el, const StyleValue& val) {
                if (std::string wrap; val.TryGetValue<std::string>(wrap, att::layoutWrap)) {
                    el->SetLayoutWrap(Flex::ToLayoutWrap(wrap));
                }
            });


            RegisterAttribute(att::layoutItem, [](ElementBase* el, const StyleValue& val) {
                if (std::string align; val.TryGetValue<std::string>(align, att::layoutItem)) {
                    el->SetLayoutMinor(Flex::ToLayoutAlign(align));
                }
            });

            RegisterAttribute(att::layout, [](ElementBase* el, const StyleValue& val) {
                if (std::vector<StyleValue> atts; val.TryGetValue<std::vector<StyleValue>>(atts, att::layout)) {  
                    if (atts.size() == 1) {
                        if (std::string str; atts[0].TryGetValue(str, att::layout)) {
                            auto align = Flex::ToLayoutAlign(str);
                            el->SetLayoutMajor(align);
                            el->SetLayoutMinor(align);
                        }
                    }
                    else if(atts.size() == 2) {
                        if (std::string str1, str2; 
                            atts[0].TryGetValue(str1, att::layout) && 
                            atts[1].TryGetValue(str2, att::layout)) {
                            el->SetLayoutMajor(Flex::ToLayoutAlign(str1));
                            el->SetLayoutMinor(Flex::ToLayoutAlign(str2));
                        }
                    }

                }
            });

            RegisterAttribute(att::layoutMajor, [](ElementBase* el, const StyleValue& val) {
                if (std::string align; val.TryGetValue<std::string>(align, att::layoutMajor)) {
                    el->SetLayoutMajor(Flex::ToLayoutAlign(align));
                }
            });

            RegisterAttribute(att::layoutMinor, [](ElementBase* el, const StyleValue& val) {
                if (std::string align; val.TryGetValue<std::string>(align, att::layoutMinor)) {
                    el->SetLayoutMinor(Flex::ToLayoutAlign(align));
                }
            });

            RegisterAttribute(att::width, [](ElementBase* el, const StyleValue& val) {
                if (float f; val.TryGetValue<float>(f, att::width)) {
                    StyleUnit::Unit u = val.GetUnit(0);

                    if (u == StyleUnit::Unit::Percent_A)
                        el->SetAvailableWidth(f);
                    else 
                        el->SetDesiredWidth(f);
                }
            });

            RegisterAttribute(att::height, [](ElementBase* el, const StyleValue& val) {
                if (float f; val.TryGetValue<float>(f, att::height)) {
                    StyleUnit::Unit u = val.GetUnit(0);

                    if (u == StyleUnit::Unit::Percent_A)
                        el->SetAvailableHeight(f);
                    else
                        el->SetDesiredHeight(f);
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

            #pragma region borderSize

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

            #pragma endregion

            #pragma region margin

            RegisterAttribute(att::margin, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::margin)) {
                    el->SetMargin(vec);
                }
            });

            RegisterAttribute(att::marginTop, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::marginTop)) {
                    el->SetMarginTop(f);
                }
            });

            RegisterAttribute(att::marginRight, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::marginRight)) {
                    el->SetMarginRight(f);
                }
            });

            RegisterAttribute(att::marginBottom, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::marginBottom)) {
                    el->SetMarginBottom(f);
                }
            });

            RegisterAttribute(att::marginLeft, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::marginLeft)) {
                    el->SetMarginLeft(f);
                }
            });

            #pragma endregion

            #pragma region padding

            RegisterAttribute(att::padding, [](ElementBase* el, const StyleValue& val) {
                if (Vector4 vec;  val.TryGetValue<Vector4>(vec, att::padding)) {
                    el->SetPadding(vec);
                }
            });

            RegisterAttribute(att::paddingTop, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::paddingTop)) {
                    el->SetPaddingTop(f);
                }
            });

            RegisterAttribute(att::paddingRight, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::paddingRight)) {
                    el->SetPaddingRight(f);
                }
            });

            RegisterAttribute(att::paddingBottom, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::paddingBottom)) {
                    el->SetPaddingBottom(f);
                }
            });

            RegisterAttribute(att::paddingLeft, [](ElementBase* el, const StyleValue& val) {
                if (float f;  val.TryGetValue<float>(f, att::paddingLeft)) {
                    el->SetPaddingLeft(f);
                }
            });

            #pragma endregion
        }
       
        RegisterAttributes();
    }


    void ElementBase::SetParent(ElementBase* elementPtr, size_t indexPos) {
        m_parentElementPtr = elementPtr;
        m_listPosition = indexPos - 1;
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

    Vector2 ElementBase::ComputeSiblingsTotalDesiredSize() const {
        if (!m_parentElementPtr) return Vector2();

        auto& siblings = m_parentElementPtr->GetChildren();

        Vector2 totalSize;
        for (auto& child : siblings) {
            // ignors this element 
            if (child->GetID() != this->m_id) {
                totalSize += child->m_desiredSize + child->m_borderSize + child->m_margin;
            }
        }
        return totalSize;
    }

    Vector2 ElementBase::ComputeSiblingsTotalLayoutSize() const {
        if (!m_parentElementPtr) return Vector2();

        auto& siblings = m_parentElementPtr->GetChildren();

        Vector2 totalSize;
        for (auto& child : siblings) {
            // ignors this element 
            if (child->GetID() != this->m_id) {
                totalSize += child->m_layoutSize + child->m_borderSize + child->m_margin;
            }
        }
        return totalSize;
    }

    void ElementBase::SetAvailableWidth(float width) {
        m_aviableSize.x = width;
        m_desiredSize.x = 0;
        MarkDirtyParent();
    }

    void ElementBase::SetAvailableHeight(float height) {
        m_aviableSize.y = height;
        m_desiredSize.y = 0;
        MarkDirtyParent();
    }

}