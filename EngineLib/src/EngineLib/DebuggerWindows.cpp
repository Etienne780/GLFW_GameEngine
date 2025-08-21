#ifndef NDEBUG

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/IconsFontAwesome5Pro.h"
#include "EngineLib/Time.h"
#include "EngineLib/Engine.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/GameObjectManager.h"
#include "EngineLib/Application.h"
#include "EngineLib/Debugger.h"
#include "EngineLib/DebuggerWindows.h"

namespace EngineCore {

    DebuggerWindows::DebuggerWindows(Debugger* debugger) {
        m_debugger = debugger;
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

            if (SidebarButton(ICON_FA_COG, "Settings")) {
            }

            if (SidebarButton(ICON_FA_ICONS, "Icon")) {
                m_iconWin = !m_iconWin;
            }

            if (m_statsWin) StatsWindow(sidebarWidth + 10, buttonHeight * 1);
            if (m_cameraWin) CameraWindow(sidebarWidth + 10, buttonHeight * 1);
            if (m_hierarchyWin) HierarchyWindow(sidebarWidth + 10, buttonHeight * 1);
            if (m_iconWin) IconDisplayWindow(sidebarWidth + 10, buttonHeight * 5);
        }
        ImGui::End();
    }

    void DebuggerWindows::StatsWindow(float startX, float startY) {
        static bool first = true;
        if (first) {
            ImGui::SetNextWindowPos(ImVec2(startX, startY));
            ImGui::SetNextWindowSize(ImVec2(250, -1));
        }

        ImGui::Begin("Stats", &m_statsWin, ImGuiWindowFlags_NoResize);
        {
            int fps = (m_debugger->GetApp()) ? m_debugger->GetApp()->App_Application_Get_FramesPerSecond() : -1;
            ImGui::Text(FormatUtils::formatString("FPS: {}", fps).c_str());
            ImGui::Text(FormatUtils::formatString("Delta time: {}", Time::GetDeltaTime()).c_str());
            ImGui::Text(FormatUtils::formatString("Frame count: {}", Time::GetFrameCount()).c_str());
            ImGui::Text(FormatUtils::formatString("GameObject count: {}", m_debugger->GetGameObjectManager()->m_gameObjects.size()).c_str());
            ImGui::Text(FormatUtils::formatString("Camera count: {}", m_debugger->GetGameObjectManager()->m_cameras.size()).c_str());
        }
        ImGui::End();

        first = false;
    }

    void DebuggerWindows::CameraWindow(float startX, float startY) {
        static bool first = true;
        if (first) {
            ImGui::SetNextWindowPos(ImVec2(startX, startY));
            ImGui::SetNextWindowSize(ImVec2(250, 375));
        }

        static bool currentState = m_debugger->IsDebugCameraActive();
        ImGui::Begin("Camera", &m_cameraWin);
        {
            if (ImGui::Checkbox("Debug camera", &currentState)) {
                // only update if it changes
                m_debugger->SetDebugCameraActive(currentState);
            }
        }
        ImGui::End();

        first = false;
    }

    void DebuggerWindows::HierarchyWindow(float startX, float startY) {
        static bool first = true;
        if (first) {
            ImGui::SetNextWindowPos(ImVec2(startX, startY));
            ImGui::SetNextWindowSize(ImVec2(250, 375));
        }

        ImGui::Begin("Hierarchy", &m_hierarchyWin);
        {
           ImGui::Text(GameObject::GetHierarchyString().c_str());
        }
        ImGui::End();

        first = false;
    }

    void DebuggerWindows::IconDisplayWindow(float startX, float startY) {
        static bool first = true;
        static int currentIndex = 0;

        if (first) {
            ImGui::SetNextWindowPos(ImVec2(startX, startY));
            ImGui::SetNextWindowSize(ImVec2(375, 250));
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

            ImGui::PushFont(m_largeIconFont);
            ImGui::Text("%s", currentIcon.utf8_data);
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
        ImGui::End();

        first = false;
    }

}

#endif