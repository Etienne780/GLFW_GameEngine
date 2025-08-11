#pragma once

namespace EngineCore {

    class ResourceManager {
    public:
        static ResourceManager& Instance();

        ResourceManager(const ResourceManager&) = delete;
        void operator=(const ResourceManager&) = delete;

        void LoadMesh();
    private:
        ResourceManager() = default;
    };

}