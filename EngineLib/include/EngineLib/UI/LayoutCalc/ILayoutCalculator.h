#pragma once
#include <CoreLib/Math/Vector2.h>

namespace EngineCore::UI {
    class ElementBase;

    class ILayoutCalculator {
    public:
        ILayoutCalculator() = default;
        virtual ~ILayoutCalculator() = default;

        virtual Vector2 CalculatePosition(ElementBase* element) = 0;
        virtual Vector2 CalculateSize(ElementBase* element) = 0;
    };

}