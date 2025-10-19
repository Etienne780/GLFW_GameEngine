#pragma once
#include "IUIElementDetailRenderer.h"
#include <string>

namespace EngineCore::UI {

    class UIElementDetailRendererImGui : public IUIElementDetailRenderer {
    public:
        UIElementDetailRendererImGui() = default;
        ~UIElementDetailRendererImGui() override = default;

        // --- Labels ---
        void DrawLabel(const std::string& text) override;
        void DrawLabelDisabled(const std::string& text) override;

        // --- Separators ---
        void DrawSeparator() override;

        // --- Collapsing headers ---
        bool DrawCollapsingHeader(const std::string& text, bool defaultOpen = true) override;

        // --- Indentation ---
        void Indent(float width = 0.0f) override;
        void Unindent(float width = 0.0f) override;

        // --- Child regions ---
        void BeginChild(const std::string& id, float width, float height, bool border = false) override;
        void EndChild() override;

        // --- Spacing ---
        void AddSpacing(float height = 0.0f) override;
    };

}
