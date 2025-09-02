#pragma once
#include <memory>
#include <ImGui/imgui.h>

class ProjectManager;
class AssetManager;

class ProjectManagerUI;
class AssetManagerUI;

class UIManager {
public:
    UIManager() = default;
    ~UIManager() = default;

    void Init(void* window, ProjectManager* projectManager, AssetManager* assetManager);
    void Render();
    void Shutdown();

private:
    std::unique_ptr<ProjectManagerUI> m_projectManagerUI = nullptr;
    std::unique_ptr<AssetManagerUI> m_assetManagerUI = nullptr;

    void BeginFrame();
};
