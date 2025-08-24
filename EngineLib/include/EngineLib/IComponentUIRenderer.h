#pragma once
#include <string>

class IUIRenderer {
public:
    virtual void DrawLabel(const std::string& text) = 0;
    virtual void DrawInputFloat(const std::string& label, float* value) = 0;
    virtual void DrawInputInt(const std::string& label, int* value) = 0;
    virtual void DrawCheckbox(const std::string& label, bool* value) = 0;
    virtual void DrawSliderFloat(const std::string& label, float* value, float min, float max) = 0;
};