#pragma once
#include "ILayoutCalculator.h"

namespace EngineCore::UI {

    class FlexLayoutCalculator : public ILayoutCalculator {
    public:
        FlexLayoutCalculator() = default;
        ~FlexLayoutCalculator() = default;

        void CalculatePosition(ElementBase* element) override;
        void CalculateSize(ElementBase* element) override;

    private:
        void CalculateMajorAxis(ElementBase* element, bool wrap);
        void CalculateMinorAxis(ElementBase* element, bool wrap);
    };

}