#ifndef NDEBUG

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <CoreLib/Math/Vector3.h>
#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/IconsFontAwesome5Pro.h"
#include "EngineLib/Time.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/GameObjectManager.h"
#include "EngineLib/Application.h"
#include "EngineLib/RenderLayerManager.h"
#include "EngineLib/Debugger.h"
#include "EngineLib/Engine.h"
#include "EngineLib/DebuggerWindows.h"

namespace EngineCore {

    DebuggerWindows::DebuggerWindows(Debugger* debugger) {
        m_debugger = debugger;

        m_subscriberId = Log::Subscribe([this](Log::Level logLevel, const std::string& msg) {
            m_log.push_back({ logLevel, msg });
        });
    }

    DebuggerWindows::~DebuggerWindows() {
        Log::Unsubscribe(m_subscriberId);
    }

    void DebuggerWindows::Shutdown() {
        m_firstStatsWin = true;
        m_firstCameraWin = true;
        m_firstHierarchyWin = true;
        m_firstInspectorWin = true;
        m_firstIconWin = true;
    }

    void DebuggerWindows::SetIconFonts(ImFont* smallIcon, ImFont* largeIcon) {
        m_smallIconFont = smallIcon;
        m_largeIconFont = largeIcon;
    }

    void DebuggerWindows::MenuSidebar(float sidebarRatio, int windowWidth, int windowHeight) {
        float sidebarWidth = windowWidth * sidebarRatio;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(sidebarWidth, static_cast<float>(windowHeight)));
        ImGui::Begin("Sidebar", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
        {

            ImGuiIO& io = ImGui::GetIO();

            float buttonHeight = 30;
            io.FontGlobalScale = 1.0f;
            const float minTextWidth = 103.0f;

            auto SidebarButton = [&](const char* icon, const char* text) {
                bool showText = sidebarWidth > minTextWidth;
                std::string label = showText ? (std::string(icon) + " " + text) : std::string(icon);
                return ImGui::Button(label.c_str(), ImVec2(-1, buttonHeight));
                };

            if (SidebarButton(ICON_FA_POLL, "Stats")) {
                m_statsWin = !m_statsWin;
            }

            if (SidebarButton(ICON_FA_VIDEO, "Camera")) {
                m_cameraWin = !m_cameraWin;
            }

            if (SidebarButton(ICON_FA_CUBE, "Objects")) {
                m_hierarchyWin = !m_hierarchyWin;
            }

            if (SidebarButton(ICON_FA_TERMINAL, "Console")) {
                m_consoleWin = !m_consoleWin;
            }

            if (SidebarButton(ICON_FA_ICONS, "Icon")) {
                m_iconWin = !m_iconWin;
            }

            if (SidebarButton(ICON_FA_COG, "Settings")) {
            }

            if (m_statsWin) StatsWindow();
            if (m_cameraWin) CameraWindow();
            if (m_hierarchyWin) HierarchyWindow();
            if (m_hierarchyWin) InspectorWindow();
            if (m_consoleWin) ConsoleWindow();
            if (m_iconWin) IconDisplayWindow();
        }
        ImGui::End();
    }

    void DebuggerWindows::StatsWindow() {
        if (m_firstStatsWin) {
            ImGui::SetNextWindowPos(ImVec2(m_statsWinState.x, m_statsWinState.y));
            ImGui::SetNextWindowSize(ImVec2(m_statsWinState.z, m_statsWinState.w));
        }

        ImGui::Begin("Stats", &m_statsWin, ImGuiWindowFlags_NoResize);
        {
            int fps = (m_debugger->GetApp()) ? m_debugger->GetApp()->App_Application_Get_FramesPerSecond() : -1;
            ImGui::Text("FPS: %d", fps);
            ImGui::Text("Delta time: %.4f", Time::GetDeltaTime());
            ImGui::Text("Frame count: %llu", Time::GetFrameCount());
            ImGui::Text("GameObject count: %zu", m_debugger->GetGameObjectManager()->m_gameObjects.size());
            ImGui::Text("Camera count: %zu", m_debugger->GetGameObjectManager()->m_cameras.size());

            ImGui::Separator();

            if (ImGui::CollapsingHeader("Render Layers")) {
                std::vector<std::string> layerNames = RenderLayerManager::GetAllRenderLayerNames();
                std::vector<RenderLayerID> layerIndices = RenderLayerManager::GetAllRenderLayerIDs();
                RenderLayerID rID;

                for (size_t i = 0; i < layerNames.size() && i < layerIndices.size(); i++) {
                    rID = layerIndices[i];
                    ImGui::BulletText("%s (%u Priority: %u)", layerNames[i].c_str(), rID.value, RenderLayerManager::GetLayerPriority(rID));
                }
            }
        }

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();
        m_statsWinState.Set(pos.x, pos.y, size.x, -1);
        ImGui::End();

        m_firstStatsWin = false;
    }

    void DebuggerWindows::CameraWindow() {
        if (m_firstCameraWin) {
            ImGui::SetNextWindowPos(ImVec2(m_cameraWinState.x, m_cameraWinState.y));
            ImGui::SetNextWindowSize(ImVec2(m_cameraWinState.z, m_cameraWinState.w));
        }

        ImGui::Begin("Camera", &m_cameraWin);
        {
            auto& cameras = m_debugger->GetGameObjectManager()->m_cameras;
            if (cameras.empty()) {
                ImGui::Text("No cameras available");
                ImGui::End();
                return;
            }

            // list for drop down
            std::vector<std::shared_ptr<EngineCore::Component::Camera>> sortedCameras;
            sortedCameras.reserve(cameras.size());

            std::shared_ptr<Component::Camera> mainCameraPtr = GameObject::GetMainCamera();
            std::shared_ptr<Component::Camera> debugCam = m_debugger->m_debugCameraGO->GetComponent<Component::Camera>();

            if (mainCameraPtr) {
                sortedCameras.push_back(mainCameraPtr);
            }

            if (debugCam && (mainCameraPtr && mainCameraPtr->GetGameObject()->GetID() != debugCam->GetGameObject()->GetID())) {
                sortedCameras.push_back(debugCam);
            }

            for (auto& weakCam : cameras) {
                auto cam = weakCam.lock();
                if (!cam) continue;

                if (mainCameraPtr && mainCameraPtr->GetGameObject()->GetID() == cam->GetGameObject()->GetID() ||
                    debugCam && debugCam->GetGameObject()->GetID() == cam->GetGameObject()->GetID())
                    continue;

                sortedCameras.push_back(cam);
            }

            int currentIndex = 0;
            std::string currentName = sortedCameras[currentIndex]
                ? sortedCameras[currentIndex]->GetGameObject()->GetName()
                : "<invalid>";

            // Dropdown
            if (ImGui::BeginCombo("Active Camera", currentName.c_str())) {
                for (int i = 0; i < (int)sortedCameras.size(); ++i) {
                    auto cam = sortedCameras[i];
                    if (!cam) continue;

                    // Debug-Cam markieren
                    std::string label = cam->GetGameObject()->GetName();
                    if (cam->GetGameObject()->GetID() == m_debugger->m_debugCameraGO->GetID()) {
                        label = "[DEBUG] " + label;
                    }

                    bool isSelected = (currentIndex == i);
                    if (ImGui::Selectable(label.c_str(), isSelected)) {
                        currentIndex = i;
                    }
                    if (isSelected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // aktive Kamera anzeigen & editieren
            auto activeCam = sortedCameras[currentIndex];
            if (activeCam) {
                m_debugger->SetMainCamera(activeCam);
                activeCam->OnInspectorGUIImpl(m_uiRenderer);
            }
        }

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();
        m_cameraWinState.Set(pos.x, pos.y, size.x, size.y);;
        ImGui::End();

        m_firstCameraWin = false;
    }

    void DebuggerWindows::DrawGameObjectNode(std::shared_ptr<GameObject>& obj) {
        if (!obj) return;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (obj->GetChildren().empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        // if disabled text gray or if persistent yellow
        if (obj->IsDisabled() || obj->IsPersistent()) {
            Vector3 color(255, 255, 255);

            if (obj->IsDisabled())
                color -= 135;

            if (obj->IsPersistent())
                color.z = 55;

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z), 255));
        }

        bool open = ImGui::TreeNodeEx((void*)(intptr_t)obj->GetID().value, flags, "%s", obj->GetName().c_str());

        if (obj->IsDisabled() || obj->IsPersistent()) {
            ImGui::PopStyleColor();
        }

        if (ImGui::IsItemClicked()) {
            m_debugger->m_hierarchySelectedGO = obj;
        }

        if (open) {
            auto childs = obj->GetChildren();
            for (std::shared_ptr<GameObject>& child : childs) {
                DrawGameObjectNode(child);
            }
            ImGui::TreePop();
        }
    }

    void DebuggerWindows::HierarchyWindow() {
        if (m_firstHierarchyWin) {
            ImGui::SetNextWindowPos(ImVec2(m_hierarchyWinState.x, m_hierarchyWinState.y));
            ImGui::SetNextWindowSize(ImVec2(m_hierarchyWinState.z, m_hierarchyWinState.w));
        }

        ImGui::Begin("Hierarchy", &m_hierarchyWin);
        {
            auto& gameObjects = m_debugger->GetGameObjectManager()->m_gameObjects;

            for (auto& go : gameObjects) {
                if (!go) continue;

                auto parent = go->GetParent();
                if (!parent) {
                    DrawGameObjectNode(go);
                }
            }
        }

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();
        m_hierarchyWinState.Set(pos.x, pos.y, size.x, size.y);
        ImGui::End();

        m_firstHierarchyWin = false;
    }

    void DebuggerWindows::InspectorWindow() {
        if (m_firstInspectorWin) {
            ImGui::SetNextWindowPos(ImVec2(m_inspectorWinState.x, m_inspectorWinState.y));
            ImGui::SetNextWindowSize(ImVec2(m_inspectorWinState.z, m_inspectorWinState.w));
        }

        ImGui::Begin("Inspector", &m_hierarchyWin);
        {
            auto selectedGO = m_debugger->m_hierarchySelectedGO;
            if (selectedGO && selectedGO->IsAlive()) {
                // ----- GameObject small checkbox -----
                bool goDisabled = !selectedGO->IsDisabled();
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2)); // smaller checkbox
                ImGui::Checkbox("##enabled", &goDisabled);
                ImGui::PopStyleVar();
                selectedGO->Disable(!goDisabled);

                ImGui::SameLine();
                ImGui::Text("GameObject: %s(%u)", selectedGO->GetName().c_str(), selectedGO->GetID());
                ImGui::Text("Persistent: %s", selectedGO->IsPersistent() ? "true" : "false");
                ImGui::Text(FormatUtils::formatString("Render-Layer: {}({})", RenderLayerManager::GetLayerName(selectedGO->GetRenderLayer()), selectedGO->GetRenderLayer().value).c_str());

                ImGui::Separator();

                // ----- Components -----
                for (auto& comp : selectedGO->GetAllComponents()) {
                    if (!comp) continue;

                    ImGui::PushID(comp.get());

                    bool compDisabled = !comp->IsDisable();
                    if (comp->CanDisalbe()) {
                        ImGui::Checkbox("##enabled", &compDisabled);
                        comp->Disable(!compDisabled);
                        ImGui::SameLine();
                    }

                    if (ImGui::CollapsingHeader(comp->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                        comp->OnInspectorGUIImpl(m_uiRenderer);
                    }

                    ImGui::PopID();
                }
            }
            else {
                m_debugger->m_hierarchySelectedGO = nullptr;
                ImGui::Text("No GameObject selected");
            }
        }

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();
        m_inspectorWinState.Set(pos.x, pos.y, size.x, size.y);
        ImGui::End();

        m_firstInspectorWin = false;
    }

    void DebuggerWindows::ConsoleWindow() {
        if (m_firstConsoleWin) {
            ImGui::SetNextWindowPos(ImVec2(m_consoleWinState.x, m_consoleWinState.y));
            ImGui::SetNextWindowSize(ImVec2(m_consoleWinState.z, m_consoleWinState.w));
        }

        ImGui::Begin("Console", &m_consoleWin);
        {
            if (ImGui::Button("Clear")) {
                m_log.clear();
            }
            ImGui::SameLine();
            static bool autoScroll = true;
            ImGui::Checkbox("Auto-scroll", &autoScroll);

            ImGui::Separator();

            // Filter options (check boxes for log levels)
            ImGui::Text("Filter:");
            ImGui::SameLine();
            bool showInfo = (m_logMask & 0b0001) != 0;
            bool showWarning = (m_logMask & 0b0010) != 0;
            bool showError = (m_logMask & 0b0100) != 0;
            bool showDebug = (m_logMask & 0b1000) != 0;

            if (ImGui::Checkbox("Info", &showInfo)) {
                if (showInfo) m_logMask |= 0b0001;
                else          m_logMask &= ~0b0001;
            }
            ImGui::SameLine();
            if (ImGui::Checkbox("Warning", &showWarning)) {
                if (showWarning) m_logMask |= 0b0010;
                else             m_logMask &= ~0b0010;
            }
            ImGui::SameLine();
            if (ImGui::Checkbox("Error", &showError)) {
                if (showError) m_logMask |= 0b0100;
                else           m_logMask &= ~0b0100;
            }
            ImGui::SameLine();
            if (ImGui::Checkbox("Debug", &showDebug)) {
                if (showDebug) m_logMask |= 0b1000;
                else           m_logMask &= ~0b1000;
            }

            ImGui::Separator();

            ImGui::BeginChild("ConsoleScrollRegion", ImVec2(0, 0), false,
                ImGuiWindowFlags_HorizontalScrollbar);

            for (auto& log : m_log) {
                if ((log.m_logLevel == Log::levelInfo && (m_logMask & 0b0001)) ||
                    (log.m_logLevel == Log::levelWarning && (m_logMask & 0b0010)) ||
                    (log.m_logLevel == Log::levelError && (m_logMask & 0b0100)) ||
                    (log.m_logLevel == Log::levelDebug && (m_logMask & 0b1000)))
                {
                    ImGui::TextUnformatted(log.m_msg.c_str());
                }
            }

            if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::EndChild();
        }

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();
        m_consoleWinState.Set(pos.x, pos.y, size.x, size.y);

        ImGui::End();

        m_firstConsoleWin = false;
    }

    void DebuggerWindows::IconDisplayWindow() {
        static int currentIndex = 0;

        if (m_firstIconWin) {
            ImGui::SetNextWindowPos(ImVec2(m_iconWinState.x, m_iconWinState.y));
            ImGui::SetNextWindowSize(ImVec2(m_iconWinState.z, m_iconWinState.w));
        }

        ImGui::Begin("Icon Preview", &m_iconWin, ImGuiWindowFlags_NoResize);
        {
            if (currentIndex < 0) currentIndex = FA5_ICONS_COUNT - 1;
            if (currentIndex >= FA5_ICONS_COUNT) currentIndex = 0;

            FontAwesome5Icon currentIcon = FA5_ICONS[currentIndex];
            ImGuiIO& io = ImGui::GetIO();

            // 2 Spalten: links Icon, rechts Text + Buttons
            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.3f);

            // ----- Linke Spalte: Icon -----
            // Vertikal zentrieren
            float colHeight = ImGui::GetContentRegionAvail().y;
            ImVec2 textSize = ImGui::CalcTextSize(currentIcon.utf8_data);
            ImGui::SetCursorPosY((colHeight - textSize.y) * 0.5f);

            if(m_largeIconFont)
                ImGui::PushFont(m_largeIconFont);
            ImGui::Text("%s", currentIcon.utf8_data);
            if (m_largeIconFont)
                ImGui::PopFont();

            ImGui::NextColumn();

            // ----- Rechte Spalte -----
            ImGui::Text(FormatUtils::formatString("Icon name: {}", currentIcon.name).c_str());
            ImGui::Text(FormatUtils::formatString("Icon unicode: {}", currentIcon.unicode).c_str());
            ImGui::Text(FormatUtils::formatString("Icon index: {}/{}", (currentIndex + 1), FA5_ICONS_COUNT).c_str());
            ImGui::Dummy(ImVec2(0, 10));
            if (ImGui::Button(ICON_FA_ARROW_ALT_CIRCLE_UP, ImVec2(-1, 30))) currentIndex--;
            if (ImGui::Button(ICON_FA_ARROW_ALT_CIRCLE_DOWN, ImVec2(-1, 30))) currentIndex++;

            ImGui::Columns(1);
        }
        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();
        m_iconWinState.Set(pos.x, pos.y, size.x, size.y);
        ImGui::End();

        m_firstIconWin = false;
    }

}

#endif