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
        ElementBase(std::string name, UIElementID id,
            MaterialID matID = ASSETS::ENGINE::MATERIAL::DefaultUI(),
            std::shared_ptr<Style> style = std::make_shared<Style>());
        virtual ~ElementBase() = default;

        template<typename T, typename... Args>
        T* AddChild(UIElementID id, Args&&... args) {
            static_assert(std::is_base_of<ElementBase, T>::value, "T must derive from EngineCore::UI::ElementBase");

            auto& ptr = m_children.emplace_back(
                std::make_unique<T>(id, std::forward<Args>(args)...)
            );
            return static_cast<T*>(ptr.get());
        }

        const std::string& GetName() const;
        UIElementID GetID() const;
        std::shared_ptr<Style> GetStyle() const;
        Vector2 GetScreenPosition();
        Vector2 GetLocalPosition();
        Vector2 GetScreenSize();
        Vector2 GetLocalSize();

        float GetParentWidth() const;
        float GetParentHeight() const;
        float GetViewportWidth() const;
        float GetViewportHeight() const;

        State GetState() const;

        std::vector<std::unique_ptr<ElementBase>>& GetChildren();
        const std::vector<std::unique_ptr<ElementBase>>& GetChildren() const;

    protected:
        std::string m_elementName;
        UIElementID m_id;
        // Style m_elementStyle; needs to be later used for clean transition between styles
        // element should probably have a default style that can be modified with a function that can be overridden
        std::shared_ptr<Style> m_style = nullptr;
        std::shared_ptr<Style> m_baseStyle = nullptr; // element base style
        ElementBase* m_parentElementPtr = nullptr;
        std::vector<std::unique_ptr<ElementBase>> m_children;
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
        /*
        * @brief Called after base style attributes are set. Can override or add attributes.
        */
        virtual void ExtendElementBaseStyle(std::shared_ptr<Style> baseStyle) {};

        ElementBase* GetParent() const;
        void SetParent(ElementBase* elementPtr);
        void SetState(State state);

        void SetLayoutDirection(LayoutDirection d);
        void SetLayoutWrap(LayoutWrap w);
        void SetLayoutContentHor(LayoutAlign a);
        void SetLayoutContentVer(LayoutAlign a);

        void SetLocalPosition(const Vector2& position);
        void SetLocalPosition(float x, float y);
        void SetLocalRotation(const Vector3& rotation);
        void SetLocalRotation(float x, float y, float z);
        void SetLocalSize(const Vector2& size);
        void SetLocalSize(float width, float height);
        void SetLocalWidth(float width);
        void SetLocalHeight(float height);

        void SetBackgroundColor(const Vector4& color);
        void SetBorderColor(const Vector4& color);
        void SetBorderRadius(const Vector4& radius);
        void SetBorderWidth(float width);
        void SetBorderTop(float top);
        void SetBorderLeft(float left);
        void SetBorderBottom(float bottom);
        void SetBorderRight(float right);
        void SetBorderSize(float width, float height);
        void SetBorderSize(const Vector4& vec);
        void SetBorderSize(float top, float right, float bottom, float left);
        void SetDuration(float duration);

        LayoutDirection GetLayoutDirection() const;
        LayoutWrap GetLayoutWrap() const;
        LayoutAlign GetLayoutContentHor() const;
        LayoutAlign GetLayoutContentVer() const;

        // only inner size
        Vector2 GetContentSize();   
        // content + padding
        Vector2 GetPaddingSize();   
        // content + padding + border
        Vector2 GetBorderSize();    
        // content + padding + border + margin
        Vector2 GetMarginSize();    

        const Vector4& GetBackgroundColor() const;
        const Vector4& GetBorderColor() const;
        const Vector4& GetBorderRadius() const;
        const Vector4& GetBorderSize() const;
        float GetBorderTop() const;
        float GetBorderLeft() const;
        float GetBorderBottom() const;
        float GetBorderRight() const;
        float GetDuration() const;

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
        /*
        * @brief Map of attribute names to behavior callbacks.
        */
        static inline std::unordered_map<std::string, std::function<void(ElementBase*, const StyleValue&)>> m_registeredAttributes;

        // layout dirty flags
        bool m_positionDirty = true;
        bool m_sizeDirty = true;
        // transform dirty flags
        bool m_localTransformDirty = true;
        bool m_worldTransformDirty = true;

        // position (0,0) is top left of screen or parent element
        Vector2 m_innerPosition{ 0, 0 };
        // Local rotation
        Vector3 m_rotation;
        // Size in pixels (content box, without padding/border/margin).
        Vector2 m_innerSize{ 800, 500 };
        Vector2 m_minSize{ 0, 0 };
        Vector2 m_maxSize{ FLT_MAX, FLT_MAX };

        // Local and world transforms
        Matrix4x4 m_localTransform;
        Matrix4x4 m_worldTransform;

        // styling props
        LayoutDirection m_layoutDirection = LayoutDirection::RowStart;
        LayoutWrap m_layoutWrap = LayoutWrap::Wrap;
        LayoutAlign m_layoutContentHor = LayoutAlign::Start;
        LayoutAlign m_layoutContentVer = LayoutAlign::Start;

        Vector4 m_padding{ 0, 0, 0, 0 }; // top, right, bottom, left
        Vector4 m_margin{ 0, 0, 0, 0 };  // top, right, bottom, left

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
        * @brief Updates the local transform matrix from position, rotation, and size.
        */
        void UpdateLocalTransform();

        /**
        * @brief Updates the world transform matrix including parent transforms.
        */
        void UpdateWorldTransform();

        /**
        * @brief Marks this element and its children dirty for matrix/layout recalculation.
        */
        void MarkDirty();

        void WindowResize(int width, int height);
        void UpdateImpl();
        void SendDrawCommandImpl(Renderer* renderer, RenderLayerID renderLayerID);
        void RegisterAttributesImpl();
        void SetStyleAttributes();
        void SetAttributes(const std::unordered_map<std::string, std::string>& attribute);
        std::shared_ptr<Style> GetElementBaseStyle();
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