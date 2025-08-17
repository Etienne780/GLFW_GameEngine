#pragma once
#include "EngineTypes.h"

class Matrix4x4;

namespace EngineCore {
    
    class Renderer {
    public:
        Renderer() = default;

        static Renderer& GetInstance();

        void Submit(const RenderCommand& cmd);
        void ReserveCommands(size_t count);
        void DrawAll();

    private:
        std::vector<RenderCommand> m_commands;
        std::vector<Matrix4x4> m_instanceMatrices;

    };

}
