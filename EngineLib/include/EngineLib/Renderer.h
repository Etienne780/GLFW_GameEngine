#pragma once
#include "EngineTypes.h"

namespace EngineCore {
    
    class Renderer {
    public:
        Renderer() = default;

        static Renderer& GetInstance();

        void Submit(const RenderCommand& cmd);

        void DrawAll();

    private:
        std::vector<RenderCommand> m_commands;
    };

}
