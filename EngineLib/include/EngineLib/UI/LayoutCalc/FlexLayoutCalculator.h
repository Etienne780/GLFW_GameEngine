#pragma once
#include "EngineLib/UI/UITypes.h"
#include "ILayoutCalculator.h"

namespace EngineCore::UI {

    class FlexLayoutCalculator : public ILayoutCalculator {
    public:
        FlexLayoutCalculator() = default;
        ~FlexLayoutCalculator() = default;

        Vector2 CalculatePosition(ElementBase* element) override;
        Vector2 CalculateSize(ElementBase* element) override;

    private:

        enum class Axis { X, Y };

        struct AxisLayout {
            Axis axis;
            Flex::LayoutAlign align; // Start, Center, End, Stretch, SpaceEvenly, SpaceAround
            bool wrap;
            float parentSize;
            float totalChildrenSize;
            float marginStart;
            float marginEnd;
            float availableSize;

            AxisLayout(Axis axis, Flex::LayoutAlign align, bool wrap, float parentSize,
                float totalChildrenSize, float marginStart, float marginEnd, float availableSize) 
                : axis(axis), align(align), wrap(wrap), parentSize(parentSize), totalChildrenSize(totalChildrenSize),
                marginStart(marginStart), marginEnd(marginEnd), availableSize(availableSize) {
            }

            float CalculatePosition(float previousEndPos = 0.0f) const;
            float CalculateSize(size_t stretchCount) const;
        };
    };

}