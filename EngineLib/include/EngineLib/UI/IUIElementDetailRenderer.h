#pragma once
#include <string>

namespace EngineCore::UI {

    class IUIElementDetailRenderer {
    public:
        virtual ~IUIElementDetailRenderer() = default;

        // --- Text ---
        virtual void DrawLabel(const std::string& text) = 0;
        virtual void DrawLabelDisabled(const std::string& text) = 0;

        // --- Separators ---
        virtual void DrawSeparator() = 0;
        virtual void DrawSeparatorText(const std::string& label) = 0;

        // --- Collapsing headers ---
        virtual bool DrawCollapsingHeader(const std::string& text, bool defaultOpen = true) = 0;

        // --- Indentation ---
        virtual void Indent(float width = 0.0f) = 0;
        virtual void Unindent(float width = 0.0f) = 0;

        // --- Child regions ---
        virtual void BeginChild(const std::string& id, float width, float height, bool border = false) = 0;
        virtual void EndChild() = 0;

        // --- Spacing ---
        virtual void AddSpacing(float height = 0.0f) = 0;
    };

}
