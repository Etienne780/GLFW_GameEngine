#pragma once
#include <memory>

#include "EngineTypes.h"

class Matrix4x4;

namespace EngineCore {

    class Engine;
    namespace Component {
        class Camera;
    }

    class Renderer {
    friend class Engine;
    public:
        Renderer() = default;

        static Renderer* GetInstance();
        void Submit(const RenderCommand& cmd);
        void ReserveCommands(size_t count);

    private:
        std::vector<RenderCommand> m_commands;// can be optimced with a ptr
        std::vector<Matrix4x4> m_instanceMatrices;

        void DrawAll();
        void SortDrawCommands(std::shared_ptr<Component::Camera> cameraPtr);
        void PrintCommands(bool displayOption);
    };

}
