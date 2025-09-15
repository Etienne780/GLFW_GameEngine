#pragma once
#include <memory>

#include "EngineTypes.h"

class Matrix4x4;

namespace EngineCore {

    class GameObjectManager;
    namespace Component {
        class Camera;
    }

    class Renderer {
    friend class GameObjectManager;
    public:
        Renderer() = default;

        static Renderer& GetInstance();
        void Submit(const RenderCommand& cmd);
        void ReserveCommands(size_t count);

    private:
        std::vector<RenderCommand> m_commands;
        std::vector<Matrix4x4> m_instanceMatrices;

        void DrawAll();
        void SortDrawCommands(std::shared_ptr<Component::Camera> cameraPtr);
        void PrintCommands(bool displayOption);
    };

}
