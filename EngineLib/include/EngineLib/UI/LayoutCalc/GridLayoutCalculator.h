#pragma once
#include "ILayoutCalculator.h"

namespace EngineCore::UI {

    class GridLayoutCalculator : public ILayoutCalculator {
    public:
        GridLayoutCalculator() = default;
        ~GridLayoutCalculator() = default;

        void CalculatePosition(ElementBase* element) override;
        void CalculateSize(ElementBase* element) override;
    };

}