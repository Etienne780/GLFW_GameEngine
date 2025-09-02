#pragma once
#include <ImGui/imgui.h>

#include "UI/ProjectManagerUI.h"
#include "UI/AssetManagerUI.h"

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
    ProjectManagerUI m_projectManagerUI{ nullptr };
    AssetManagerUI m_assetManagerUI{ nullptr };

    void BeginFrame();
};
