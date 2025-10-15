#pragma once
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <CoreLib/Math/Vector2.h>
#include <CoreLib/Math/Vector4.h>

#include "EngineLib/AssetRepository.h"
#include "EngineLib/ShaderBindObject.h"
#include "EngineLib/Renderer.h"
#include "EngineLib/EngineTypes.h"
#include "EngineLib/UI/LayoutCalc/FlexLayoutCalculator.h"
#include "EngineLib/UI/LayoutCalc/GridLayoutCalculator.h"
#include "../Style.h"

namespace EngineCore {
    class UIManager;
    class Material;
}

namespace EngineCore::UI {

    class ElementBase {
    friend class UIManager;
    public:
        using Callback = std::function<void()>;

        ElementBase(std::string name, UIElementID id,
            std::shared_ptr<Style> style = std::make_shared<Style>(),
            MaterialID matID = ASSETS::ENGINE::MATERIAL::DefaultUI());
        ~ElementBase();

        template<typename T, typename... Args>
        std::shared_ptr<T> AddChild(UIElementID id, Args&&... args) {
            static_assert(std::is_base_of<ElementBase, T>::value,
                "T must derive from EngineCore::UI::ElementBase");

            auto basePtr = m_children.emplace_back(
                std::make_shared<T>(id, std::forward<Args>(args)...)
            );

            basePtr->SetParent(this, m_children.size());
            basePtr->Init();

            return std::static_pointer_cast<T>(basePtr);
        }

        const std::string& GetName() const;
        UIElementID GetID() const;
        std::shared_ptr<Style> GetStyle() const;
        std::shared_ptr<Style> GetElementStyle() const;
        /**
        * @brief Gets the position of the element in world coordinates, taking parent positions into account.
        * @return The world position of the element.
        */
        Vector2 GetWorldPosition();

        /**
        * @brief Gets the local position of the element relative to its parent.
        * @return The local position of the element.
        */
        Vector2 GetLocalPosition() const;

        /**
        * @brief Gets the size of the element in screen coordinates, including any scaling applied by UIManager.
        * @return The scaled screen size of the element.
        */
        Vector2 GetScreenSize();

        /**
        * @brief Gets the local size of the element (final layout size without screen scaling).
        * @return The local size of the element.
        */
        Vector2 GetLocalSize();

        float GetParentWidth() const;
        float GetParentHeight() const;
        float GetViewportWidth() const;
        float GetViewportHeight() const;

        LayoutType GetLayoutType() const;
        Flex::LayoutDirection GetLayoutDirection() const;
        Flex::LayoutWrap GetLayoutWrap() const;
        Flex::LayoutAlign GetLayoutMajor() const;
        Flex::LayoutAlign GetLayoutMinor() const;
        Flex::LayoutAlign GetLayoutItem() const;

        Vector2 GetDesiredPosition() const;
        // aviable
        Vector2 GetAviableSize() const;
        // size - padding
        Vector2 GetContentSize();
        // style size
        Vector2 GetDesiredSize() const;
        // content + border
        Vector2 GetSizeWithBorder();
        // content + border + margin
        Vector2 GetMarginSize();

        Vector3 GetWorldRotation() const;

        ElementBase* GetParent() const;
        /**
        * @biref gets the position of this children in the parent list
        */
        size_t GetListPosition() const;

        const Vector4& GetMargin() const;
        float GetMarginTop() const;
        float GetMarginRight() const;
        float GetMarginBottom() const;
        float GetMarginLeft() const;

        const Vector4& GetPadding() const;
        float GetPaddingTop() const;
        float GetPaddingRigth() const;
        float GetPaddingBottom() const;
        float GetPaddingLeft() const;

        const Vector4& GetBackgroundColor() const;
        const Vector4& GetBorderColor() const;
        const Vector4& GetBorderRadius() const;

        const Vector4& GetBorderSize() const;
        float GetBorderTop() const;
        float GetBorderRight() const;
        float GetBorderBottom() const;
        float GetBorderLeft() const;
        float GetDuration() const;

        /**
        * @brief Computes the total (Desired + border + margin) size of all sibling elements except this element.
        * @return A Vector2 representing the summed margin size of the siblings.
        */
        Vector2 ComputeSiblingsTotalDesiredSize() const;

        /**
        * @brief Computes the total (Layout + border + margin)  size of all sibling elements except this element.
        * @return A Vector2 representing the summed margin size of the siblings.
        */
        Vector2 ComputeSiblingsTotalLayoutSize() const;

        State GetState() const;

        size_t GetChildCount() const;
        std::shared_ptr<ElementBase> GetChild(size_t index);
        std::vector<std::shared_ptr<ElementBase>>& GetChildren();
        const std::vector<std::shared_ptr<ElementBase>>& GetChildren() const;
    protected:
        std::string m_elementName;
        UIElementID m_id;
        // Style m_elementStyle; needs to be later used for clean transition between styles
        // element should probably have a default style that can be modified with a function that can be overridden
        // maybe need to add limit to how often per frame style updates can be applyd
        Style::SubscriberID m_styleDirtyCallbackID = 0;
        Style::SubscriberID m_baseStyleDirtyCallbackID = 0;
        std::shared_ptr<Style> m_style = nullptr;
        std::shared_ptr<Style> m_baseStyle = nullptr; // element base style
        ElementBase* m_parentElementPtr = nullptr;
        // position of this child in the parent child list
        size_t m_listPosition = 0;
        std::vector<std::shared_ptr<ElementBase>> m_children;
        State m_state = State::Normal;
        RenderCommand m_cmd;
        MaterialID m_materialID{ ENGINE_INVALID_ID };
        ShaderBindObject m_sbo;

        Callback m_onClick;
        Callback m_onHover;
        Callback m_onPress;

        /*
        * @brief Called every frame.
        */
        virtual void Update() {};
        /*
        * @brief Called to prepare rendering. renderLayerID, modelMatrix, isUI, type, meshID are already set.
        */
        virtual void SendDrawCommand(Renderer* renderer) {};
        /*
        * @brief This function is called only once for every class. It registers all attributes this element can have and what they do.
        */
        virtual void RegisterAttributes() {};

        void SetState(State state);

        void SetLayoutType(LayoutType layoutType);
        void SetLayoutDirection(Flex::LayoutDirection direction);
        void SetLayoutWrap(Flex::LayoutWrap wrap);
        void SetLayoutMajor(Flex::LayoutAlign align);
        void SetLayoutMinor(Flex::LayoutAlign align);
        void SetLayoutItem(Flex::LayoutAlign align);


        /**
        * @brief Sets the local position of the element using a Vector2.
        * @param position The new local position.
        */
        void SetLocalPosition(const Vector2& position);

        /**
        * @brief Sets the local position of the element using individual x and y values.
        * @param x The new x-coordinate.
        * @param y The new y-coordinate.
        */
        void SetLocalPosition(float x, float y);

        /**
        * @brief Sets the local rotation of the element using a Vector3.
        * @param rotation The new rotation vector (pitch, yaw, roll).
        */
        void SetLocalRotation(const Vector3& rotation);

        /**
        * @brief Sets the local rotation of the element using individual x, y, z values.
        * @param x Rotation around X-axis.
        * @param y Rotation around Y-axis.
        * @param z Rotation around Z-axis.
        */
        void SetLocalRotation(float x, float y, float z);

        /**
        * @brief Sets the desired size of the element using a Vector2. size cannot be negative, will be clamped to 0
        * @param size The new size (width, height).
        */
        void SetDesiredSize(const Vector2& size);

        /**
        * @brief Sets the desired size of the element using individual width and height values. size cannot be negative, will be clamped to 0
        * @param width The new width.
        * @param height The new height.
        */
        void SetDesiredSize(float width, float height);

        /**
        * @brief Sets only the desired width of the element. width cannot be negative, will be clamped to 0
        * @param width The new width.
        */
        void SetDesiredWidth(float width);

        /**
        * @brief Sets only the desired height of the element. height cannot be negative, will be clamped to 0
        * @param height The new height.
        */
        void SetDesiredHeight(float height);

        void SetBackgroundColor(const Vector4& color);
        void SetBorderColor(const Vector4& color);
        void SetBorderRadius(const Vector4& radius);

        void SetBorderSize(const Vector4& vec);
        void SetBorderSize(float horizontal, float vertical);
        void SetBorderSize(float top, float right, float bottom, float left);
        void SetBorderWidth(float width);
        void SetBorderTop(float top);
        void SetBorderRight(float right);
        void SetBorderBottom(float bottom);
        void SetBorderLeft(float left);

        void SetMargin(const Vector4& margin);
        void SetMargin(float top, float right, float bottom, float left);
        void SetMarginTop(float top);
        void SetMarginRight(float right);
        void SetMarginBottom(float bottom);
        void SetMarginLeft(float left);

        void SetPadding(const Vector4& padding);
        void SetPadding(float top, float right, float bottom, float left);
        void SetPaddingTop(float top);
        void SetPaddingRight(float right);
        void SetPaddingBottom(float bottom);
        void SetPaddingLeft(float left);

        void SetDuration(float duration);

        /*
        * @brief Checks whether a given point lies within the bounding box of this element.
        * @param mousePos The point (e.g. mouse position) to test, in the same coordinate space as the element.
        * @return True if the point is inside the bounding box, false otherwise.
        */
        bool IsMouseOver(const Vector2& mousePos);

        /**
        * @brief Gets the world Model-Matrix (local to world origin).
        */
        Matrix4x4* GetWorldModelMatrixPtr();
        /**
        * @brief Gets the world Model-Matrix (local to world origin).
        */
        const Matrix4x4& GetWorldModelMatrix();

        /*
        * @brief Registers an attribute and its behavior.
        * Example:
        * RegisterAttribute(att::borderRadius, [](ElementBase* el, const StyleValue& val) {
        *    val.TryGetValue<Vector4>(el->m_borderRadius, att::borderRadius);
        * });
        */
        void RegisterAttribute(const std::string& name, std::function<void(ElementBase*, const StyleValue&)> func);

    private:
        // Gets called after construction of this element
        void Init();

        static inline bool m_attributesRegistered = false;
        static inline FlexLayoutCalculator s_flexCalculator;
        static inline GridLayoutCalculator s_gridCalculator;
        /*
        * @brief Map of attribute names to behavior callbacks.
        */
        static inline std::unordered_map<std::string, std::function<void(ElementBase*, const StyleValue&)>> m_registeredAttributes;
        
        mutable bool m_transformDirty = true;
        mutable bool m_styleDirty = true;

        // Calculated final position of the element including layout effects (major/minor alignment, parent constraints, etc.)
        Vector2 m_layoutPosition{ 0.0f, 0.0f };
        // Local rotation
        Vector3 m_rotation{ 0.0f, 0.0f, 0.0f };
        // Calculated final size of the element including padding, border (content + padding + border)
        Vector2 m_layoutSize{ 0.0f, 0.0f };
        Vector2 m_minSize{ 0.0f, 0.0f };
        Vector2 m_maxSize{ FLT_MAX, FLT_MAX };
        Vector2 m_aviableSize{ -1.0f, -1.0f };

        // Local and world transforms
        Matrix4x4 m_worldTransform;

        // styling props
        LayoutType m_layoutType = LayoutType::Flex;
        Flex::LayoutDirection m_layoutDirection = Flex::LayoutDirection::Row;
        Flex::LayoutWrap m_layoutWrap = Flex::LayoutWrap::Wrap;
        Flex::LayoutAlign m_layoutMajor = Flex::LayoutAlign::Start;
        Flex::LayoutAlign m_layoutMinor = Flex::LayoutAlign::Start;
        Flex::LayoutAlign m_layoutItem = Flex::LayoutAlign::Start;

        // Style-driven position (absolute or relative, as defined in style)
        Vector2 m_desiredPosition{ 0, 0 };

        // Style-driven size (content box size from style, before layout adjustments)
        Vector2 m_desiredSize{ 800, 500 };
        // order: top, right, bottom, left
        Vector4 m_padding{ 0, 0, 0, 0 };
        // order: top, right, bottom, left
        Vector4 m_margin{ 0, 0, 0, 0 };  

        Vector4 m_backgroundColor{ 1, 1, 1, 1 };
        Vector4 m_borderColor{ 0.75f, 0.75f, 0.75f, 1 };
        Vector4 m_borderRadius{ 25, 5, 100, 50 }; // top-left, top-right, bottom-right, bottom-left
        Vector4 m_borderSize{ 0, 0, 0, 0 };
        float m_duration = 0.0f;

        /**
        * @brief Calculates the layout-relative position (taking into account margin and alignment).
        */
        void UpdateLayoutPosition();

        /**
        * @brief Calculates the layout-relative size (taking into account padding and borders).
        */
        void UpdateLayoutSize();

        /**
        * @brief Updates the world transform matrix including parent transforms.
        */
        void UpdateWorldTransform();

        /**
        * @brief Marks the parent and its children dirty for matrix/layout recalculation.
        */
        void MarkTransDirtyParent() const;
        /**
        * @brief Marks this element and its children dirty for matrix/layout recalculation.
        */
        void MarkTransDirty() const;

        void UpdateImpl();
        void SendDrawCommandImpl(Renderer* renderer, RenderLayerID renderLayerID);
        void RegisterAttributesImpl();
        void SetParent(ElementBase* elementPtr, size_t indexPos);
        void SetAttributes(const std::unordered_map<std::string, std::string>& attribute);
        void SetStyleAttributes();
        void SetAvailableWidth(float width);
        void SetAvailableHeight(float height);
    };

    template <typename Derived>
    class Element : public ElementBase {
    public:
        using ElementBase::ElementBase;

        Derived* SetOnClick(Callback cb) {
            m_onClick = std::move(cb);
            return static_cast<Derived*>(this);
        }

        Derived* SetOnHover(Callback cb) {
            m_onHover = std::move(cb);
            return static_cast<Derived*>(this);
        }

        Derived* SetOnPress(Callback cb) {
            m_onPress = std::move(cb);
            return static_cast<Derived*>(this);
        }
    };

}

/*
Element:
- Panel 
- Image
- (svg) maybe (probably not)
- Text
- Button
- Checkmark
- Slider/Loading bar
- Inputfield
- Tabs
- Custom elements

*/