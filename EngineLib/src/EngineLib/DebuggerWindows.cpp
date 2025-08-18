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

        void MenuSidebar(float sidebarRatio, float windowWidth, float windowHeight) {
            float sidebarWidth = windowWidth * sidebarRatio;

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(sidebarWidth, windowHeight));
            ImGui::Begin("Sidebar", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            ImGuiIO& io = ImGui::GetIO();

            io.FontGlobalScale = 1.0f;
            const float minTextWidth = 103.0f;

            auto SidebarButton = [&](const char* icon, const char* text) {
                bool showText = sidebarWidth > minTextWidth;
                std::string label = showText ? (std::string(icon) + " " + text) : std::string(icon);
                return ImGui::Button(label.c_str(), ImVec2(-1, 40));
            };

            ImGui::Text(FormatUtils::toString(sidebarWidth).c_str());
            if (SidebarButton(ICON_FA_CAMERA, "Camera")) {
            }

            if (SidebarButton(ICON_FA_CUBE, "Objects")) {
            }

            if (SidebarButton(ICON_FA_COG, "Settings")) {
            }

            ImGui::End();
        }

	}

}