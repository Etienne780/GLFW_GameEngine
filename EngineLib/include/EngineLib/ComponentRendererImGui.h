class Vector2;
class Vector3;
class Vector4;

#include "IComponentUIRenderer.h"

namespace EngineCore {

    class ImGuiRenderer : public IComponentUIRenderer {
    public:
        // Labels
        void DrawLabel(const std::string& text) override;
        void DrawLabelDisabled(const std::string& text) override;

        // Input
        void DrawInputText(const std::string& label, std::string* value) override;
        void DrawInputMultilineText(const std::string& label, std::string* value) override;
        void DrawInputFloat(const std::string& label, float* value) override;
        void DrawInputFloat2(const std::string& label, Vector2* value) override;
        void DrawInputFloat3(const std::string& label, Vector3* value) override;
        void DrawInputFloat4(const std::string& label, Vector4* value) override;
        void DrawInputInt(const std::string& label, int* value) override;

        // Drag
        void DrawDragFloat(const std::string& label, float* value, float speed, float min, float max) override;
        void DrawDragFloat2(const std::string& label, Vector2* value, float speed, float min, float max) override;
        void DrawDragFloat3(const std::string& label, Vector3* value, float speed, float min, float max) override;
        void DrawDragFloat4(const std::string& label, Vector4* value, float speed, float min, float max) override;
        void DrawDragInt(const std::string& label, int* value, float speed, int min, int max) override;

        // Slider
        void DrawSliderFloat(const std::string& label, float* value, float min, float max) override;
        void DrawSliderFloat2(const std::string& label, Vector2* value, float min, float max) override;
        void DrawSliderFloat3(const std::string& label, Vector3* value, float min, float max) override;
        void DrawSliderFloat4(const std::string& label, Vector4* value, float min, float max) override;
        void DrawSliderInt(const std::string& label, int* value, int min, int max) override;

        // Misc
        void DrawCheckbox(const std::string& label, bool* value) override;
        void DrawSeparatorText(const std::string& label) override;
        void DrawSeparator() override;
        bool DrawCollapsingHeader(const std::string& label, bool open) override;

        // Color
        void DrawColorEdit3(const std::string& label, Vector3* color) override;
        void DrawColorEdit4(const std::string& label, Vector4* color) override;
        void DrawColorPicker3(const std::string& label, Vector3* color) override;
        void DrawColorPicker4(const std::string& label, Vector4* color) override;
        void DrawColorButton3(const std::string& label, const Vector3& color) override;
        void DrawColorButton4(const std::string& label, const Vector4& color) override;

    };

}