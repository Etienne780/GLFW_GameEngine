#include <ImGUI/imgui.h>

#include "EngineLib/UI/UIElementDetailRendererImGui.h"

namespace EngineCore::UI {

    // --- Labels ---
    void ImGuiUIElementDetailRenderer::DrawLabel(const std::string& text) {
        ImGui::TextUnformatted(text.c_str());
    }

    void ImGuiUIElementDetailRenderer::DrawLabelDisabled(const std::string& text) {
        ImGui::TextDisabled("%s", text.c_str());
    }

    // --- Separators ---
    void ImGuiUIElementDetailRenderer::DrawSeparator() {
        ImGui::Separator();
    }

    void ImGuiUIElementDetailRenderer::DrawSeparatorText(const std::string& label) {
        ImGui::SeparatorText(label.c_str());
    }

    // --- Collapsing headers ---
    bool ImGuiUIElementDetailRenderer::DrawCollapsingHeader(const std::string& text, bool defaultOpen) {
        return ImGui::CollapsingHeader(text.c_str(), defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0);
    }

    // --- Indentation ---
    void ImGuiUIElementDetailRenderer::Indent(float width) {
        ImGui::Indent(width);
    }

    void ImGuiUIElementDetailRenderer::Unindent(float width) {
        ImGui::Unindent(width);
    }

    // --- Child regions ---
    void ImGuiUIElementDetailRenderer::BeginChild(const std::string& id, float width, float height, bool border) {
        ImGui::BeginChild(id.c_str(), ImVec2(width, height), border);
    }

    void ImGuiUIElementDetailRenderer::EndChild() {
        ImGui::EndChild();
    }

    // --- Spacing ---
    void ImGuiUIElementDetailRenderer::AddSpacing(float height) {
        if (height <= 0.0f) {
            ImGui::Spacing();
        }
        else {
            ImGui::Dummy(ImVec2(0.0f, height));
        }
    }

} // namespace EngineCore::UI
