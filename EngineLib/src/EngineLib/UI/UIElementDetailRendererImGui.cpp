#include "EngineLib/UI/UIElementDetailRendererImGui.h"
#include <ImGUI/imgui.h>

namespace EngineCore::UI {

    // --- Labels ---
    void UIElementDetailRendererImGui::DrawLabel(const std::string& text) {
        ImGui::TextUnformatted(text.c_str());
    }

    void UIElementDetailRendererImGui::DrawLabelDisabled(const std::string& text) {
        ImGui::TextDisabled("%s", text.c_str());
    }

    // --- Separators ---
    void UIElementDetailRendererImGui::DrawSeparator() {
        ImGui::Separator();
    }

    // --- Collapsing headers ---
    bool UIElementDetailRendererImGui::DrawCollapsingHeader(const std::string& text, bool defaultOpen) {
        return ImGui::CollapsingHeader(text.c_str(), defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0);
    }

    // --- Indentation ---
    void UIElementDetailRendererImGui::Indent(float width) {
        ImGui::Indent(width);
    }

    void UIElementDetailRendererImGui::Unindent(float width) {
        ImGui::Unindent(width);
    }

    // --- Child regions ---
    void UIElementDetailRendererImGui::BeginChild(const std::string& id, float width, float height, bool border) {
        ImGui::BeginChild(id.c_str(), ImVec2(width, height), border);
    }

    void UIElementDetailRendererImGui::EndChild() {
        ImGui::EndChild();
    }

    // --- Spacing ---
    void UIElementDetailRendererImGui::AddSpacing(float height) {
        if (height <= 0.0f) {
            ImGui::Spacing();
        }
        else {
            ImGui::Dummy(ImVec2(0.0f, height));
        }
    }

} // namespace EngineCore::UI
