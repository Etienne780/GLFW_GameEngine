#include <CoreLib/Log.h>

#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/LayoutCalc/FlexLayoutCalculator.h"

namespace EngineCore::UI {

    Vector2 FlexLayoutCalculator::CalculatePosition(ElementBase* element) {
        // position in the child list of the parent
        size_t listPosition = element->GetListPosition();
        ElementBase* parent = element->GetParent();
        if (!parent)
            return { 0, 0 };
        auto parentLayoutDir = parent->GetLayoutDirection();
        bool isParentLayoutDirRow = parentLayoutDir == Flex::LayoutDirection::Row;
        bool parentWrap = (parent->GetLayoutWrap() == Flex::LayoutWrap::Wrap);
        Axis major = (isParentLayoutDirRow) ? Axis::X : Axis::Y;
        Axis minor = (!isParentLayoutDirRow) ? Axis::X : Axis::Y;

        float parentSizeMajor = (major == Axis::X) ? parent->GetContentSize().x : parent->GetContentSize().y;
        float parentSizeMinor = (major != Axis::X) ? parent->GetContentSize().x : parent->GetContentSize().y;

        float totalSizeMajor = (major == Axis::X) ? element->ComputeSiblingsTotalLayoutSize().x : element->ComputeSiblingsTotalLayoutSize().y;
        float majorAvaibleSize = (isParentLayoutDirRow) ? element->GetAviableSize().x : element->GetAviableSize().y;
        float minorAvaibleSize = (!isParentLayoutDirRow) ? element->GetAviableSize().x : element->GetAviableSize().y;

        Vector4 margin = element->GetMargin();
        float marginMajorStart = (isParentLayoutDirRow) ? margin.y : margin.x;
        float marginMajorEnd = (isParentLayoutDirRow) ? margin.w : margin.z;
        float marginMinorStart = (!isParentLayoutDirRow) ? margin.y : margin.x;
        float marginMinorEnd = (!isParentLayoutDirRow) ? margin.w : margin.z;

        float majorDesiredPosition = (isParentLayoutDirRow) ? element->GetDesiredPosition().x : element->GetDesiredPosition().y;
        float minorDesiredPosition = (!isParentLayoutDirRow) ? element->GetDesiredPosition().x : element->GetDesiredPosition().y;

        AxisLayout majorAxis{ MAJOR_AXIS, major, parent->GetLayoutMajor(), parentWrap, parentSizeMajor,
          totalSizeMajor, marginMajorStart, marginMajorEnd, majorDesiredPosition, majorAvaibleSize };
        AxisLayout minorAxis{ MINOR_AXIS, minor, parent->GetLayoutMinor(), parentWrap, parentSizeMinor,
            0, marginMinorStart, marginMinorEnd, minorDesiredPosition, minorAvaibleSize };

        float majorSize = (isParentLayoutDirRow) ? element->GetLocalSize().x : element->GetLocalSize().y;
        float minorSize = (!isParentLayoutDirRow) ? element->GetLocalSize().x : element->GetLocalSize().y;

        size_t siblingCount = parent->GetChildCount();
        float preMajorEndPos = 0.0f;
        float preMajorSize = 0.0f;
        float preMinorEndPos = 0.0f;
        float preMinorSize = 0.0f;
        if (listPosition > 0.0f) {
            std::shared_ptr<ElementBase> preElement = parent->GetChild(listPosition - 1);
            if (preElement) {
                preMajorEndPos = (isParentLayoutDirRow) ?
                    preElement->GetLocalPosition().x :
                    preElement->GetLocalPosition().y;

                preMinorEndPos = (!isParentLayoutDirRow) ?
                    preElement->GetLocalPosition().x :
                    preElement->GetLocalPosition().y;

                preMajorSize = (isParentLayoutDirRow) ?
                    preElement->GetMarginSize().x :
                    preElement->GetMarginSize().y;

                preMinorSize = (!isParentLayoutDirRow) ?
                    preElement->GetMarginSize().x :
                    preElement->GetMarginSize().y;
            }
        }

        Vector2 newPosition;
        if (isParentLayoutDirRow) {
            // major axis X
            newPosition.Set(
                majorAxis.CalculatePosition(preMajorEndPos, preMajorSize, majorSize, siblingCount, listPosition),
                minorAxis.CalculatePosition(preMinorEndPos, preMinorSize, minorSize, siblingCount, listPosition)
            );
        }
        else {
            // major axis Y
            newPosition.Set(
                minorAxis.CalculatePosition(preMinorEndPos, preMinorSize, minorSize, siblingCount, listPosition),
                majorAxis.CalculatePosition(preMajorEndPos, preMajorSize, majorSize, siblingCount, listPosition)
            );
        }

        return newPosition;
    }

    Vector2 FlexLayoutCalculator::CalculateSize(ElementBase* element) {
        ElementBase* parent = element->GetParent();
        if (!parent)
            return { 0, 0 };
        auto parentLayoutDir = parent->GetLayoutDirection();
        bool isParentLayoutDirRow = parentLayoutDir == Flex::LayoutDirection::Row;
        bool parentWrap = (parent->GetLayoutWrap() == Flex::LayoutWrap::Wrap);
        Axis major = (isParentLayoutDirRow) ? Axis::X : Axis::Y;
        Axis minor = (!isParentLayoutDirRow) ? Axis::X : Axis::Y;

        float parentSizeMajor = (major == Axis::X) ? parent->GetContentSize().x : parent->GetContentSize().y;
        float parentSizeMinor = (major != Axis::X) ? parent->GetContentSize().x : parent->GetContentSize().y;
        
        float totalSizeMajor = (major == Axis::X) ? element->ComputeSiblingsTotalDesiredSize().x : element->ComputeSiblingsTotalDesiredSize().y;
        float majorAvaibleSize = (isParentLayoutDirRow) ? element->GetAviableSize().x : element->GetAviableSize().y;
        float minorAvaibleSize = (!isParentLayoutDirRow) ? element->GetAviableSize().x : element->GetAviableSize().y;

        Vector4 margin = element->GetMargin();
        float marginMajorStart = (isParentLayoutDirRow) ? margin.y : margin.x;
        float marginMajorEnd = (isParentLayoutDirRow) ? margin.w : margin.z;
        float marginMinorStart = (!isParentLayoutDirRow) ? margin.y : margin.x;
        float marginMinorEnd = (!isParentLayoutDirRow) ? margin.w : margin.z;

        float majorDesiredSize = (isParentLayoutDirRow) ? element->GetDesiredSize().x : element->GetDesiredSize().y;
        float minorDesiredSize = (!isParentLayoutDirRow) ? element->GetDesiredSize().x : element->GetDesiredSize().y;

        AxisLayout majorAxis{ MAJOR_AXIS, major, parent->GetLayoutMajor(), parentWrap, parentSizeMajor,
            totalSizeMajor, marginMajorStart, marginMajorEnd, majorDesiredSize, majorAvaibleSize };
        AxisLayout minorAxis{ MINOR_AXIS, minor, parent->GetLayoutMinor(), parentWrap, parentSizeMinor,
            0, marginMinorStart, marginMinorEnd, minorDesiredSize, minorAvaibleSize };
    
        // Calculates the amount of children that will be stretched
        size_t cStretchCountMajor = 0, cAvaibleStretchCountMajor = 0;
        for (const auto& c : parent->GetChildren()) {
            if (isParentLayoutDirRow) {
                // major axis X
                if (c->GetAviableSize().x >= 0.0f)
                    cAvaibleStretchCountMajor++;
                else if (c->GetDesiredSize().x <= 0.0f)
                    cStretchCountMajor++;
            }
            else {
                // major axis Y
                if (c->GetAviableSize().y >= 0.0f)
                    cAvaibleStretchCountMajor++;
                else if (c->GetDesiredSize().y <= 0.0f)
                    cStretchCountMajor++;
            }
        }

        Vector2 newSize;
        if (isParentLayoutDirRow) {
            // major axis X
            newSize.Set(
                majorAxis.CalculateSize(cStretchCountMajor, cAvaibleStretchCountMajor),
                minorAxis.CalculateSize(0, 0)
            );
        }
        else {
            // major axis Y
            newSize.Set(
                minorAxis.CalculateSize(0, 0),
                majorAxis.CalculateSize(cStretchCountMajor, cAvaibleStretchCountMajor)
            );
        }

        return newSize;
    }

    float FlexLayoutCalculator::AxisLayout::CalculatePosition(float previousEndPos, float previousEndSize, float elementSize, size_t siblingCount, size_t elementIndex) const {
         float freeSpace = m_parentSize - (m_totalChildrenSize + elementSize);

        switch (m_align)
        {
        case EngineCore::UI::Flex::LayoutAlign::Start:
            if (m_isMajorAxis)
                return previousEndPos + previousEndSize + m_marginStart + m_marginEnd;
            else
                return previousEndPos + m_marginStart + m_marginEnd;

        case EngineCore::UI::Flex::LayoutAlign::Center:
            // Center the element within the parent
            if (elementIndex == 0)
                return freeSpace * 0.5f;
            else {
                if (m_isMajorAxis)
                    return previousEndPos + previousEndSize;
                else
                    return previousEndPos;
            }

        case EngineCore::UI::Flex::LayoutAlign::End:
            // Align element to the end of the parent
            if (elementIndex == 0)
                return m_parentSize - elementSize - m_marginStart - m_marginEnd;
            else {
                if (m_isMajorAxis)
                    return previousEndPos - elementSize - m_marginStart - m_marginEnd;
                else
                    return previousEndPos - m_marginStart - m_marginEnd;
            }

        case EngineCore::UI::Flex::LayoutAlign::Stretch:
            if (m_isMajorAxis)
                return previousEndPos + previousEndSize + m_marginStart + m_marginEnd;
            else
                return previousEndPos + m_marginStart + m_marginEnd;

        case EngineCore::UI::Flex::LayoutAlign::SpaceEvenly:
            // Space elements evenly including start and end padding
            if (siblingCount > 1)
            {
                float space = (m_parentSize - m_totalChildrenSize) / (siblingCount + 1);
                return previousEndPos + previousEndSize + space;
            }
            return previousEndPos + previousEndSize;

        case EngineCore::UI::Flex::LayoutAlign::SpaceAround:
            // Space elements around so each element has half-space before and after
            if (siblingCount > 0)
            {
                float space = (m_parentSize - m_totalChildrenSize) / siblingCount;
                return previousEndPos + previousEndSize + (space * 0.5f);
            }
            return previousEndPos + previousEndSize;

        default:
            // Fallback: normal flow
            return previousEndPos + previousEndSize;
        }
    }

    float FlexLayoutCalculator::AxisLayout::CalculateSize(size_t stretchCount, size_t avaibleStretchCount) const {
        // stop stretch if wrap is enabled
        if (m_wrap)
            return m_desired;

        // m_availableSize is -1 if not used
        if (m_availableSize >= 0.0f) {
            if (avaibleStretchCount > 0 && m_isMajorAxis) {
                float usableSpace = m_parentSize - m_totalChildrenSize;
                float adaptiveSize = ((usableSpace / avaibleStretchCount) * (m_availableSize / 100.0f)) - m_marginStart - m_marginEnd;
                return std::max(adaptiveSize, 0.0f);
            }
            else if(!m_isMajorAxis) {
                return std::max((m_parentSize * (m_availableSize / 100.0f)) - m_marginStart - m_marginEnd, 0.0f);
            }
            return std::max(m_desired, 0.0f);
        }

        switch (m_align)
        {
        // elements that have a desired size will not be streched, stretches elements to the avaible size left (100%a)
        case EngineCore::UI::Flex::LayoutAlign::Stretch: {
            if (m_desired > 0.0f)
                return std::max(m_desired, 0.0f);

            if (stretchCount > 0 && m_isMajorAxis) {
                float usableSpace = m_parentSize - m_totalChildrenSize;
                float stretchedSize = (usableSpace / stretchCount) - m_marginStart - m_marginEnd;
                return std::max(stretchedSize, 0.0f);
            }
            else if (!m_isMajorAxis) {
                return std::max(m_parentSize - m_marginStart - m_marginEnd, 0.0f);
            }
            return std::max(m_desired, 0.0f);
            break;
        }
        case EngineCore::UI::Flex::LayoutAlign::Start:
        case EngineCore::UI::Flex::LayoutAlign::Center:
        case EngineCore::UI::Flex::LayoutAlign::End:
        case EngineCore::UI::Flex::LayoutAlign::SpaceEvenly:
        case EngineCore::UI::Flex::LayoutAlign::SpaceAround:
        default:
            return std::max(m_desired, 0.0f);
        }
        return std::max(m_desired, 0.0f);
    }

}