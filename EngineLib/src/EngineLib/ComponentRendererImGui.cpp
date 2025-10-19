#include <ImGUI/imgui.h>
#include <CoreLib/Math/Vector2.h>
#include <CoreLib/Math/Vector3.h>
#include <CoreLib/Math/Vector4.h>

#include "EngineLib/ComponentRendererImGui.h"

namespace EngineCore {

    // Labels
    void ImGuiRenderer::DrawLabel(const std::string& text) {
        ImGui::Text("%s", text.c_str());
    }

    void ImGuiRenderer::DrawLabelDisabled(const std::string& text) {
        ImGui::TextDisabled("%s", text.c_str());
    }

    // Input
    void ImGuiRenderer::DrawInputText(const std::string& label, std::string* value) {
        if (value) {
            char buffer[1024];
            strncpy_s(buffer, sizeof(buffer), value->c_str(), _TRUNCATE);
            buffer[sizeof(buffer) - 1] = '\0';
            if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer))) {
                *value = std::string(buffer);
            }
        }
    }

    void ImGuiRenderer::DrawInputMultilineText(const std::string& label, std::string* value) {
        if (value) {
            char buffer[4096];
            strncpy_s(buffer, sizeof(buffer), value->c_str(), _TRUNCATE);
            buffer[sizeof(buffer) - 1] = '\0';
            if (ImGui::InputTextMultiline(label.c_str(), buffer, sizeof(buffer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8))) {
                *value = std::string(buffer);
            }
        }
    }

    void ImGuiRenderer::DrawInputFloat(const std::string& label, float* value) {
        ImGui::InputFloat(label.c_str(), value);
    }

    void DrawInputFloat2(const std::string& label, Vector2* value) {
        float values[2] = { value->x, value->y };
        if (ImGui::InputFloat2(label.c_str(), values)) {
            value->x = values[0];
            value->y = values[1];
        }
    }

    void ImGuiRenderer::DrawInputFloat3(const std::string& label, Vector3* value) {
        float values[3] = { value->x, value->y, value->z };
        if (ImGui::InputFloat3(label.c_str(), values)) {
            value->x = values[0];
            value->y = values[1];
            value->z = values[2];
        }
    }

    void ImGuiRenderer::DrawInputFloat4(const std::string& label, Vector4* value) {
        float values[4] = { value->x, value->y, value->z, value->w };
        if (ImGui::InputFloat4(label.c_str(), values)) {
            value->x = values[0];
            value->y = values[1];
            value->z = values[2];
            value->w = values[3];
        }
    }

    void ImGuiRenderer::DrawInputInt(const std::string& label, int* value) {
        ImGui::InputInt(label.c_str(), value);
    }

    // Drag
    void ImGuiRenderer::DrawDragFloat(const std::string& label, float* value, float speed, float min, float max) {
        ImGui::DragFloat(label.c_str(), value, speed, min, max);
    }

    void DrawDragFloat2(const std::string& label, Vector2* value, float speed, float min, float max) {
        float values[2] = { value->x, value->y };
        if (ImGui::DragFloat2(label.c_str(), values, speed, min, max)) {
            value->Set(values[0], values[1]);
        }
    }

    void ImGuiRenderer::DrawDragFloat3(const std::string& label, Vector3* value, float speed, float min, float max) {
        float values[3] = { value->x, value->y, value->z };
        if (ImGui::DragFloat3(label.c_str(), values, speed, min, max)) {
            value->Set(values[0], values[1], values[2]);
        }
    }

    void ImGuiRenderer::DrawDragFloat4(const std::string& label, Vector4* value, float speed, float min, float max) {
        float values[4] = { value->x, value->y, value->z, value->w };
        if (ImGui::DragFloat4(label.c_str(), values, speed, min, max)) {
            value->Set(values[0], values[1], values[2], values[3]);
        }
    }

    void ImGuiRenderer::DrawDragInt(const std::string& label, int* value, float speed, int min, int max) {
        ImGui::DragInt(label.c_str(), value, speed, min, max);
    }

    // Slider
    void ImGuiRenderer::DrawSliderFloat(const std::string& label, float* value, float min, float max) {
        ImGui::SliderFloat(label.c_str(), value, min, max);
    }

    void ImGuiRenderer::DrawSliderFloat2(const std::string& label, Vector2* value, float min, float max) {
        float values[2] = { value->x, value->y };
        if (ImGui::SliderFloat2(label.c_str(), values, min, max)) {
            value->Set(values[0], values[1]);
        }
    }

    void ImGuiRenderer::DrawSliderFloat3(const std::string& label, Vector3* value, float min, float max) {
        float values[3] = { value->x, value->y, value->z };
        if (ImGui::SliderFloat3(label.c_str(), values, min, max)) {
            value->Set(values[0], values[1], values[2]);
        }
    }

    void ImGuiRenderer::DrawSliderFloat4(const std::string& label, Vector4* value, float min, float max) {
        float values[4] = { value->x, value->y, value->z, value->w };
        if (ImGui::SliderFloat4(label.c_str(), values, min, max)) {
            value->Set(values[0], values[1], values[2], values[3]);
        }
    }

    void ImGuiRenderer::DrawSliderInt(const std::string& label, int* value, int min, int max) {
        ImGui::SliderInt(label.c_str(), value, min, max);
    }

    // Misc
    void ImGuiRenderer::DrawCheckbox(const std::string& label, bool* value) {
        ImGui::Checkbox(label.c_str(), value);
    }

    void ImGuiRenderer::DrawSeparatorText(const std::string& label) {
        ImGui::SeparatorText(label.c_str());
    }

    void ImGuiRenderer::DrawSeparator() {
        ImGui::Separator();
    }

    bool ImGuiRenderer::DrawCollapsingHeader(const std::string& label, bool open) {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));

        bool result;
        if (open)
            result = ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
        else
            result = ImGui::CollapsingHeader(label.c_str());

        ImGui::PopStyleColor(3); // drei Farben wiederherstellen
        return result;
    }

    // Color
    void ImGuiRenderer::DrawColorEdit3(const std::string& label, Vector3* color) {
        float col[3] = { color->x, color->y, color->z };
        if (ImGui::ColorEdit3(label.c_str(), col)) {
            color->Set(col[0], col[1], col[2]);
        }
    }

    void ImGuiRenderer::DrawColorEdit4(const std::string& label, Vector4* color) {
        float col[4] = { color->x, color->y, color->z, color->w };
        if (ImGui::ColorEdit4(label.c_str(), col)) {
            color->Set(col[0], col[1], col[2], col[3]);
        }
    }

    void ImGuiRenderer::DrawColorPicker3(const std::string& label, Vector3* color) {
        float col[3] = { color->x, color->y, color->z };
        if (ImGui::ColorPicker3(label.c_str(), col)) {
            color->Set(col[0], col[1], col[2]);
        }
    }

    void ImGuiRenderer::DrawColorPicker4(const std::string& label, Vector4* color) {
        float col[4] = { color->x, color->y, color->z, color->w };
        if (ImGui::ColorPicker4(label.c_str(), col)) {
            color->Set(col[0], col[1], col[2], col[3]);
        }
    }

    void ImGuiRenderer::DrawColorButton3(const std::string& label, const Vector3& color) {
        ImVec4 col(color.x, color.y, color.z, 1.0f);
        ImGui::ColorButton(label.c_str(), col);
    }

    void ImGuiRenderer::DrawColorButton4(const std::string& label, const Vector4& color) {
        ImVec4 col(color.x, color.y, color.z, color.w);
        ImGui::ColorButton(label.c_str(), col);
    }

}