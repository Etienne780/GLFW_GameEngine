#pragma once
#include <string>

class Vector2;
class Vector3;
class Vector4;

namespace EngineCore {

    class IComponentUIRenderer {
    public:
        virtual ~IComponentUIRenderer() = default;

        // Labels
        virtual void DrawLabel(const std::string& text) = 0;
        virtual void DrawLabelDisabled(const std::string& text) = 0;

        // Input
        virtual void DrawInputText(const std::string& label, std::string* value) = 0;
        virtual void DrawInputMultilineText(const std::string& label, std::string* value) = 0;
        virtual void DrawInputFloat(const std::string& label, float* value) = 0;
        virtual void DrawInputFloat2(const std::string& label, Vector2* value) = 0;
        virtual void DrawInputFloat3(const std::string& label, Vector3* value) = 0;
        virtual void DrawInputFloat4(const std::string& label, Vector4* value) = 0;
        virtual void DrawInputInt(const std::string& label, int* value) = 0;

        // Drag
        virtual void DrawDragFloat(const std::string& label, float* value, float speed = 0.1f, float min = 0.0f, float max = 0.0f) = 0;
        virtual void DrawDragFloat2(const std::string& label, Vector2* value, float speed = 0.1f, float min = 0.0f, float max = 0.0f) = 0;
        virtual void DrawDragFloat3(const std::string& label, Vector3* value, float speed = 0.1f, float min = 0.0f, float max = 0.0f) = 0;
        virtual void DrawDragFloat4(const std::string& label, Vector4* value, float speed = 0.1f, float min = 0.0f, float max = 0.0f) = 0;
        virtual void DrawDragInt(const std::string& label, int* value, float speed = 0.1f, int min = 0, int max = 0)= 0;

        // Slider
        virtual void DrawSliderFloat(const std::string& label, float* value, float min, float max) = 0;
        virtual void DrawSliderFloat2(const std::string& label, Vector2* value, float min, float max) = 0;
        virtual void DrawSliderFloat3(const std::string& label, Vector3* value, float min, float max) = 0;
        virtual void DrawSliderFloat4(const std::string& label, Vector4* value, float min, float max) = 0;
        virtual void DrawSliderInt(const std::string& label, int* value, int min, int max) = 0;

        // Misc
        virtual void DrawCheckbox(const std::string& label, bool* value) = 0;
        virtual void DrawSeparatorText(const std::string& label) = 0;
        virtual void DrawSeparator() = 0;
        virtual bool DrawCollapsingHeader(const std::string& label, bool open = false) = 0;

        // Color
        virtual void DrawColorEdit3(const std::string& label, Vector3* color) = 0;
        virtual void DrawColorEdit4(const std::string& label, Vector4* color) = 0;
 
        virtual void DrawColorPicker3(const std::string& label, Vector3* color) = 0;
        virtual void DrawColorPicker4(const std::string& label, Vector4* color) = 0;

        virtual void DrawColorButton3(const std::string& label, const Vector3& color) = 0;
        virtual void DrawColorButton4(const std::string& label, const Vector4& color) = 0;
    };

}