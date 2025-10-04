#pragma once
#include "ILayoutCalculator.h"

namespace EngineCore::UI {

    class GridLayoutCalculator : public ILayoutCalculator {
    public:
        GridLayoutCalculator() = default;
        ~GridLayoutCalculator() = default;

        Vector2 CalculatePosition(ElementBase* element) override;
        Vector2 CalculateSize(ElementBase* element) override;
    };

}