#include <CoreLib/ConversionUtils.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/Time.h"
#include "EngineLib/UI/UIManager.h"
#include "EngineLib/UI/StyleAttribute.h"
#include "EngineLib/UI/AttributeNames.h"
#include "EngineLib/UI/Elements/Element.h"

namespace EngineCore::UI {

	ElementBase::ElementBase(std::string name, UIElementID id, std::shared_ptr<Style> style, MaterialID matID)
		: m_elementName(std::move(name)), m_id(id), m_style(std::move(style)) {
        m_cmd.isUI = true;
        m_cmd.type = RenderCommandType::Mesh;
        m_cmd.meshID = ASSETS::ENGINE::MESH::UIPlain();
        m_cmd.materialID = matID;
        m_baseStyle = UIManager::GetElementBaseStyle();

        // subs to style dirty events
        m_styleDirtyCallbackID = m_style->SubDirtCallback([this]() { m_styleDirty = true; });
        m_baseStyleDirtyCallbackID = m_baseStyle->SubDirtCallback([this]() { m_styleDirty = true; });

        m_mergedStyle = Style::Create("Merged");
        m_mergedStyle->Extend(m_baseStyle);
        m_mergedStyle->Extend(m_style);
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
        InitShaderBindObject();
    }

    void ElementBase::InitShaderBindObject() {
        m_sbo.SetParam("uBackgroundColor", m_backgroundColor);
        m_sbo.SetParam("uBorderColor", m_borderColor);
        m_sbo.SetParam("uBorderRadius", m_borderRadius);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        m_sbo.SetParam("uSize", m_layoutSize);
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

    Vector2 ElementBase::GetLocalPosition() const {
        return m_layoutPosition;
    }

    Vector2 ElementBase::GetScreenSize() {
        Vector2 size = m_layoutSize;
        if (UIManager::GetUIScaling()) {
            size *= UIManager::GetUIScaleFactor();
        }
        return size;
    }

    Vector2 ElementBase::GetLocalSize() {
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

        m_styleDirty = true;
	}

    void ElementBase::SetLayoutType(LayoutType type) {
        if (m_layoutType == type)
            return;
        m_layoutType = type;
        MarkTransDirtyParent();
    }

    void ElementBase::SetLayoutDirection(Flex::LayoutDirection dir) {
        if (m_layoutDirection == dir)
            return;
        m_layoutDirection = dir;
        MarkTransDirtyParent();
    }

    void ElementBase::SetLayoutWrap(Flex::LayoutWrap wrap) {
        if (m_layoutWrap == wrap)
            return;
        m_layoutWrap = wrap;
        MarkTransDirtyParent();
    }

    void ElementBase::SetLayoutMajor(Flex::LayoutAlign ali) {
        if (m_layoutMajor == ali)
            return;
        m_layoutMajor = ali;
        MarkTransDirtyParent();
    }

    void ElementBase::SetLayoutMinor(Flex::LayoutAlign ali) {
        if (m_layoutMinor == ali)
            return;
        m_layoutMinor = ali;
        MarkTransDirtyParent();
    }

    void ElementBase::SetLayoutItem(Flex::LayoutAlign ali) {
        if (m_layoutItem == ali)
            return;
        m_layoutItem = ali;
        MarkTransDirtyParent();
    }

    void ElementBase::SetLocalPosition(const Vector2& pos) {
        SetLocalPosition(pos.x, pos.y);
    }

    void ElementBase::SetLocalPosition(float x, float y) {
        if (m_desiredPosition.x == x && 
            m_desiredPosition.y == y)
            return;
        m_desiredPosition.Set(x, y);
        MarkTransDirtyParent();
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
        MarkTransDirtyParent();
    }

    void ElementBase::SetDesiredSize(const Vector2& scale, StyleUnit::Unit unitX, StyleUnit::Unit unitY) {
        SetDesiredSize(scale.x, scale.y, unitX, unitY);
    }

    void ElementBase::SetDesiredSize(float x, float y, StyleUnit::Unit unitX, StyleUnit::Unit unitY) {
        x = std::max(0.0f, x);
        y = std::max(0.0f, y);
        if (m_desiredSize.x == x &&
            m_desiredSize.y == y &&
            m_sizeUnits[0] == unitX &&
            m_sizeUnits[1] == unitY)
            return;
        m_desiredSize.Set(x, y);
        m_sizeUnits[0] = unitX;
        m_sizeUnits[1] = unitY;
        MarkTransDirtyParent();
    }

    void ElementBase::SetDesiredWidth(float x, StyleUnit::Unit unitX) {
        x = std::max(0.0f, x);
        if (m_desiredSize.x == x &&
            m_sizeUnits[0] == unitX)
            return;
        m_desiredSize.x = x;
        m_sizeUnits[0] = unitX;
        MarkTransDirtyParent();
    }

    void ElementBase::SetDesiredHeight(float y, StyleUnit::Unit unitY) {
        y = std::max(0.0f, y);
        if (m_desiredSize.y == y &&
            m_sizeUnits[1] == unitY)
            return;
        m_desiredSize.y = y;
        m_sizeUnits[1] = unitY;
        MarkTransDirtyParent();
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
        width = std::max(0.0f, width);
        if (m_borderSize == Vector4(width))
            return;
        m_borderSize.Set(width);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkTransDirtyParent();
    }

    void ElementBase::SetBorderTop(float top) {
        top = std::max(0.0f, top);
        if (m_borderSize.x == top)
            return;
        m_borderSize.x = top;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkTransDirtyParent();
    }

    void ElementBase::SetBorderRight(float right) {
        right = std::max(0.0f, right);
        if (m_borderSize.y == right)
            return;
        m_borderSize.y = right;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkTransDirtyParent();
    }

    void ElementBase::SetBorderBottom(float bottom) {
        bottom = std::max(0.0f, bottom);
        if (m_borderSize.z == bottom)
            return;
        m_borderSize.z = bottom;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkTransDirtyParent();
    }

    void ElementBase::SetBorderLeft(float left) {
        left = std::max(0.0f, left);
        if (m_borderSize.w == left)
            return;
        m_borderSize.w = left;
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkTransDirtyParent();
    }

    void ElementBase::SetBorderSize(const Vector4& vec) {
        SetBorderSize(vec.x, vec.y, vec.z, vec.w);
        MarkTransDirtyParent();
    }

    void ElementBase::SetBorderSize(float hor, float ver) {
        hor = std::max(0.0f, hor);
        ver = std::max(0.0f, ver);
        if (m_borderSize.x == ver &&
            m_borderSize.y == hor &&
            m_borderSize.z == ver &&
            m_borderSize.w == hor)
            return;
        m_borderSize.Set(ver, hor, ver, hor);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkTransDirtyParent();
    }

    void ElementBase::SetBorderSize(float top, float right, float bottom, float left) {
        top = std::max(0.0f, top);
        right = std::max(0.0f, right);
        bottom = std::max(0.0f, bottom);
        left = std::max(0.0f, left);
        if (m_borderSize.x == top &&
            m_borderSize.y == right &&
            m_borderSize.z == bottom &&
            m_borderSize.w == left)
            return;
        m_borderSize.Set(top, right, bottom, left);
        m_sbo.SetParam("uBorderWidth", m_borderSize);
        MarkTransDirtyParent();
    }

    void ElementBase::SetMargin(const Vector4& mar) {
        SetMargin(mar.x, mar.y, mar.z, mar.w);
    }

    void ElementBase::SetMargin(float top, float right, float bottom, float left) {
        top = std::max(0.0f, top);
        right = std::max(0.0f, right);
        bottom = std::max(0.0f, bottom);
        left = std::max(0.0f, left);
        if (m_margin.x == top &&
            m_margin.y == right &&
            m_margin.z == bottom &&
            m_margin.w == left)
            return;
        m_margin.Set(top, right, bottom, left);
        MarkTransDirtyParent();
    }

    void ElementBase::SetMarginTop(float top) {
        top = std::max(0.0f, top);
        if (m_margin.x == top)
            return;
        m_margin.x = top;
        MarkTransDirtyParent();
    }

    void ElementBase::SetMarginRight(float right) {
        right = std::max(0.0f, right);
        if (m_margin.y == right)
            return;
        m_margin.y = right;
        MarkTransDirtyParent();
    }

    void ElementBase::SetMarginBottom(float bottom) {
        bottom = std::max(0.0f, bottom);
        if (m_margin.z == bottom)
            return;
        m_margin.z = bottom;
        MarkTransDirtyParent();
    }

    void ElementBase::SetMarginLeft(float left) {
        left = std::max(0.0f, left);
        if (m_margin.w == left)
            return;
        m_margin.w = left;
        MarkTransDirtyParent();
    }

    void ElementBase::SetPadding(const Vector4& pad) {
        SetPadding(pad.x, pad.y, pad.z, pad.w);
    }

    void ElementBase::SetPadding(float top, float right, float bottom, float left) {
        top = std::max(0.0f, top);
        right = std::max(0.0f, right);
        bottom = std::max(0.0f, bottom);
        left = std::max(0.0f, left);
        if (m_padding.x == top &&
            m_padding.y == right &&
            m_padding.z == bottom &&
            m_padding.w == left)
            return;
        m_padding.Set(top, right, bottom, left);
        MarkTransDirtyParent();
    }

    void ElementBase::SetPaddingTop(float top) {
        top = std::max(0.0f, top);
        if (m_padding.x == top)
            return;
        m_padding.x = top;
        MarkTransDirtyParent();
    }

    void ElementBase::SetPaddingRight(float right) {
        right = std::max(0.0f, right);
        if (m_padding.y == right)
            return;
        m_padding.y = right;
        MarkTransDirtyParent();
    }

    void ElementBase::SetPaddingBottom(float bottom) {
        bottom = std::max(0.0f, bottom);
        if (m_padding.z == bottom)
            return;
        m_padding.z = bottom;
        MarkTransDirtyParent();
    }

    void ElementBase::SetPaddingLeft(float left) {
        left = std::max(0.0f, left);
        if (m_padding.w == left)
            return;
        m_padding.w = left;
        MarkTransDirtyParent();
    }

    void ElementBase::SetDuration(float duration) {
        duration = std::max(0.0f, duration);
        if (m_duration == duration)
            return;
        m_duration = duration;
    }

    void ElementBase::SetLayoutSize(const Vector2& size) {
        SetLayoutSize(size.x, size.y);
    }

    void ElementBase::SetLayoutSize(float x, float y) {
        m_layoutSize.Set(x, y);
        MarkTransDirtyParent();
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

    Vector2 ElementBase::GetDesiredPixelSize() const {
        return m_desiredPixelSize;
    }

    const std::array<StyleUnit::Unit, 2>& ElementBase::GetSizeUnits() const {
        return m_sizeUnits;
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

    void ElementBase::CallOnClick() {
        if (m_onClick)
            m_onClick();
    }

    void ElementBase::CallOnHover() {
        if (m_onHover)
            m_onHover();
    }

    void ElementBase::CallOnPress() {
        if (m_onPress)
            m_onPress();
    }

    void ElementBase::CallOnDrag() {
        if (m_onDrag)
            m_onDrag();
    }

    void ElementBase::RegisterAttribute(const std::string& name, std::function<void(ElementBase*, const StyleValue&)> func) {
        m_registeredAttributes[FormatUtils::toLowerCase(name)] = func;
    }

    void ElementBase::UpdateLayoutPosition() {
        if (!m_parentElementPtr)
            return;

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
    }

    void ElementBase::UpdateLayoutSize() {
        if (!m_parentElementPtr)
            return;

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
    }

    void ElementBase::MarkTransDirtyParent() const {
        //Marke the parent as dirty so it updates alle the child elements
        // e.g. if width/height of this el changed. than the positions needs to recalculatet
        if (m_parentElementPtr) {
            m_parentElementPtr->MarkTransDirty();
            return;
        }

        UIManager::SetRootElementTransDirty();
    }

    void ElementBase::MarkTransDirty() const {
        m_transformDirty = true;
        for (auto& child : m_children) {
            child->MarkTransDirty();
        }
    }

    void ElementBase::UpdateImpl() {
        if (m_styleDirty) {
            SetStyleAttributes();
            m_styleDirty = false;
        }

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
                    el->SetLayoutItem(Flex::ToLayoutAlign(align));
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
                    el->SetDesiredWidth(f, val.GetUnit(0));
                }
            });

            RegisterAttribute(att::height, [](ElementBase* el, const StyleValue& val) {
                if (float f; val.TryGetValue<float>(f, att::height)) {
                    el->SetDesiredHeight(f, val.GetUnit(0));
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

            RegisterAttributes();
        }
    }

    void ElementBase::SetParent(ElementBase* elementPtr, size_t indexPos) {
        m_parentElementPtr = elementPtr;
        m_listPosition = indexPos - 1;
    }

    void ElementBase::SetStyleAttributes() {

        // SetAttributes(m_baseStyle->GetAllState(m_state));
        // // if state is not normal use normal state as a base
        // if(m_state != State::Normal)
        //     SetAttributes(m_style->GetAllState(State::Normal));
        // SetAttributes(m_style->GetAllState(m_state));


        //SetAttributes(m_baseStyle->GetAllState(m_state));
        if (m_state != State::Normal)
            SetAttributes(m_mergedStyle->GetAllState(State::Normal));
        SetAttributes(m_mergedStyle->GetAllState(m_state));
    }

    void ElementBase::SetAttributes(const std::unordered_map<std::string, std::string>& attribute) {
        for (auto& [name, valueStr] : attribute) {
            const StyleValue& value = StyleAttribute::GetAttributeValue(name, valueStr);
            auto it = m_registeredAttributes.find(FormatUtils::toLowerCase(name));
            if (it != m_registeredAttributes.end()) {
                it->second(this, value);
            }
        }
    }

    Vector2 ElementBase::ComputeSiblingsTotalDesiredPixelSize() const {
        if (!m_parentElementPtr) return Vector2();

        auto& siblings = m_parentElementPtr->GetChildren();

        Vector2 totalSize;
        for (auto& child : siblings) {
            // ignors this element 
            if (child->GetID() != this->m_id) {
                totalSize += child->m_desiredPixelSize + child->m_borderSize + child->m_margin;
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

    void ElementBase::CalculateDesiredPixels() {
        if (m_sizeUnits[0] != StyleUnit::Unit::Percent_A)
            m_desiredPixelSize.x = StyleUnit::EvaluateSizeUnit(m_desiredSize.x, m_sizeUnits[0], *this);
        else
            m_desiredPixelSize.x = 0;

        if (m_sizeUnits[1] != StyleUnit::Unit::Percent_A)
            m_desiredPixelSize.y = StyleUnit::EvaluateSizeUnit(m_desiredSize.y, m_sizeUnits[1], *this);
        else
            m_desiredPixelSize.y = 0;
    }


    bool ElementBase::IsTransformDirty() {
        return m_transformDirty;
    }

    void ElementBase::SetTransformDirty(bool value) {
        m_transformDirty = value;
    }

}