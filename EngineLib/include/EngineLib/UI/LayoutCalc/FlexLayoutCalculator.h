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
            Axis m_axis;
            Flex::LayoutAlign m_align; // Start, Center, End, Stretch, SpaceEvenly, SpaceAround
            bool m_wrap;
            float m_parentSize;
            float m_totalChildrenSize;
            float m_marginStart;
            float m_marginEnd;
            // can be desired position or size
            float m_desired;
            // avaible size is in the amount of percent that the element wants to fill
            float m_availableSize;

            AxisLayout(Axis axis, Flex::LayoutAlign align, bool wrap, float parentSize,
                float totalChildrenSize, float marginStart, float marginEnd, float desiredSize, float availableSize)
                : m_axis(axis), m_align(align), m_wrap(wrap), m_parentSize(parentSize), m_totalChildrenSize(totalChildrenSize),
                m_marginStart(marginStart), m_marginEnd(marginEnd), m_desired(desiredSize), m_availableSize(availableSize) {
            }

            float CalculatePosition(float previousEndPos, float previousEndSize, float elementSize, size_t siblingCount) const;
            float CalculateSize(size_t stretchCount, size_t avaibleStretchCount) const;
        };
    };

}