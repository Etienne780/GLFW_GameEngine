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
#include "EngineLib/DebuggerWindows.h"

namespace EngineCore {

	namespace DebugWindow {

        ImFont* smallIconFont = nullptr;
        ImFont* largeIconFont = nullptr;

        void SetIconFonts(ImFont* smallIcon, ImFont* largeIcon) {
            smallIconFont = smallIcon;
            largeIconFont = largeIcon;
        }

        void MenuSidebar(float sidebarRatio, int windowWidth, int windowHeight) {
            float sidebarWidth = windowWidth * sidebarRatio;

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(sidebarWidth, static_cast<float>(windowHeight)));
            ImGui::Begin("Sidebar", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
            {

                ImGuiIO& io = ImGui::GetIO();

                io.FontGlobalScale = 1.0f;
                const float minTextWidth = 103.0f;

                auto SidebarButton = [&](const char* icon, const char* text) {
                    bool showText = sidebarWidth > minTextWidth;
                    std::string label = showText ? (std::string(icon) + " " + text) : std::string(icon);
                    return ImGui::Button(label.c_str(), ImVec2(-1, 30));
                    };

                if (SidebarButton(ICON_FA_POLL, "Info/Stats")) {
                }

                if (SidebarButton(ICON_FA_VIDEO, "Camera")) {
                }

                if (SidebarButton(ICON_FA_CUBE, "Objects")) {
                }

                if (SidebarButton(ICON_FA_COG, "Settings")) {
                }

                static bool iconWin = false;
                if (SidebarButton(ICON_FA_ICONS, "Icon")) {
                    iconWin = !iconWin;
                }

                if (iconWin) IconDisplayWindow(sidebarWidth + 10, 10);
            }
            ImGui::End();
        }

        void IconDisplayWindow(float startX, float startY) {
            static bool first = true;
            static int currentIndex = 0;

            if (first) {
                ImGui::SetNextWindowPos(ImVec2(startX, startY));
                ImGui::SetNextWindowSize(ImVec2(375, 250));
            }

            ImGui::Begin("Icon Preview", nullptr, ImGuiWindowFlags_NoResize);

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

            ImGui::PushFont(largeIconFont);
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
            ImGui::End();

            first = false;
        }

	}

}