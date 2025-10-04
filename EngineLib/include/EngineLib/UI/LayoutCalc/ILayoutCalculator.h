#pragma once

namespace EngineCore::UI {
    class ElementBase;

    class ILayoutCalculator {
    public:
        ILayoutCalculator() = default;
        virtual ~ILayoutCalculator() = default;

        virtual void CalculatePosition(ElementBase* element) = 0;
        virtual void CalculateSize(ElementBase* element) = 0;
    };

}