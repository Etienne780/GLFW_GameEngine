#include <ImGUI/imgui.h>
#include "IComponentUIRenderer.h"

class ImGuiRenderer : public IUIRenderer {
public:
    void DrawLabel(const std::string& text) override {
        ImGui::Text("%s", text.c_str());
    }

    void DrawInputFloat(const std::string& label, float* value) override {
        ImGui::InputFloat(label.c_str(), value);
    }

    void DrawInputInt(const std::string& label, int* value) override {
        ImGui::InputInt(label.c_str(), value);
    }

    void DrawCheckbox(const std::string& label, bool* value) override {
        ImGui::Checkbox(label.c_str(), value);
    }

    void DrawSliderFloat(const std::string& label, float* value, float min, float max) override {
        ImGui::SliderFloat(label.c_str(), value, min, max);
    }
};
