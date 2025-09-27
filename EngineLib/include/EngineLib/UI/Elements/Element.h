#pragma once
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <CoreLib/Math/Vector2.h>

#include "../../Renderer.h"
#include "../../EngineTypes.h"
#include "../Style.h"

namespace EngineCore {
    class UIManager;
}

namespace EngineCore::UI {

    class ElementBase {
    friend class UIManager;
    public:
        using Callback = std::function<void()>;

        ElementBase(std::string name, UIElementID id, std::shared_ptr<Style> style = std::make_shared<Style>());
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
        Vector2 GetScreenPosition() const;
        Vector2 GetLocalPosition() const;
        Vector2 GetScreenSize() const;
        Vector2 GetLocalSize() const;

        State GetState() const;

        std::vector<std::unique_ptr<ElementBase>>& GetChildren();
        const std::vector<std::unique_ptr<ElementBase>>& GetChildren() const;

    protected:
        std::string m_elementName;
        UIElementID m_id;
        std::shared_ptr<Style> m_elementStyle;
        std::shared_ptr<Style> m_style = nullptr;
        ElementBase* m_parentElement = nullptr;
        std::vector<std::unique_ptr<ElementBase>> m_children;
        State m_state = State::Normal;
        RenderCommand m_cmd;

        Callback onClick;
        Callback onHover;
        Callback onPress;

        void Update();
        virtual void UpdateImpl() {};
        void SendDrawCommand(Renderer* renderer, RenderLayerID renderLayerID);
        virtual void SendDrawCommandImpl(Renderer* renderer) {};

        ElementBase* GetParent() const;
        void SetParent(ElementBase* elementPtr);
        void SetState(State state);

        void SetLocalPosition(const Vector2& position);
        void SetLocalPosition(float x, float y);
        void SetLocalRotation(const Vector3& rotation);
        void SetLocalRotation(float x, float y, float z);
        void SetLocalScale(const Vector2& scale);
        void SetLocalScale(float x, float y);

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

    private:
        // if the transform has changed
        bool m_localMatrixDirty = true;
        bool m_worldMatrixDirty = true;
        // position 0,0 is top left of screen or parent element
        Vector2 m_localPosition{ 0, 0 };
        // Local rotation
        Vector3 m_localRotation;
        // size is in pixels (could get scaled by UIManager, so the value is not absolute)
        Vector2 m_localScale{ 500, 500 };

        // Local model matrix
        Matrix4x4 m_localMatrix;
        Matrix4x4 m_worldMatrix;

        void CalculateLocalModelMat();
        void CalculateWorldModelMat();
        /**
        * @brief Marks this and children dirty
        */
        void MarkDirty();
    };

    template <typename Derived>
    class Element : public ElementBase {
    public:
        using ElementBase::ElementBase;

        Derived* SetOnClick(Callback cb) {
            onClick = std::move(cb);
            return static_cast<Derived*>(this);
        }

        Derived* SetOnHover(Callback cb) {
            onHover = std::move(cb);
            return static_cast<Derived*>(this);
        }

        Derived* SetOnPress(Callback cb) {
            onPress = std::move(cb);
            return static_cast<Derived*>(this);
        }
    };

}

/*
Element:
- Panel
- Image
- (svg) maybe
- Text
- Button
- Checkmark
- Slider/Loading bar
- Inputfield
- Tabs
- Custom elements

*/