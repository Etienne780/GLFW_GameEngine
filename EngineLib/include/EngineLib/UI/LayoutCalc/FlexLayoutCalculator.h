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

        static inline bool MAJOR_AXIS = true;
        static inline bool MINOR_AXIS = false;

        struct AxisLayout {
            bool m_isMajorAxis = false;
            Axis m_axis;
            Flex::LayoutAlign m_align; // Start, Center, End, Stretch, SpaceEvenly, SpaceAround
            bool m_wrap;
            float m_parentSize;
            float m_totalChildrenSize;

            float m_marginStart = 0;
            float m_marginEnd = 0;
            float m_borderStart = 0;
            float m_borderEnd = 0;
            // can be desired position or size
            float m_desired;
            // avaible size is in the amount of percent that the element wants to fill
            float m_availableSize;

            AxisLayout(bool isMajorAxis, Axis axis, Flex::LayoutAlign align, bool wrap, float parentSize,
                float totalChildrenSize, float desiredSize, float availableSize)
                : m_isMajorAxis(isMajorAxis), m_axis(axis), m_align(align), m_wrap(wrap), m_parentSize(parentSize), 
                m_totalChildrenSize(totalChildrenSize), m_desired(desiredSize), m_availableSize(availableSize) {
            }

            void SetMargin(float start, float end) {
                m_marginStart = start;
                m_marginEnd = end;
            }

            void SetBorder(float start, float end) {
                m_borderStart = start;
                m_borderEnd = end;
            }

            /**
            * @brief Calculates the position of an element along an axis based on the Flex alignment.
            * @param previousEndPos The end position of the previous sibling along this axis (X or Y)
            * @param previousEndSize The size of the previous sibling along this axis
            * @param elementSize The size of the current element along this axis
            * @param siblingCount The number of siblings (used for SpaceEvenly / SpaceAround)
            * @return The start position of the element along the axis
            */
            float CalculatePosition(float previousEndPos, float previousEndSize, float elementSize, size_t siblingCount, size_t elementIndex) const;
            float CalculateSize(size_t stretchCount, size_t avaibleStretchCount) const;
        };
    };

}