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

        float majorDesiredPosition = (isParentLayoutDirRow) ? element->GetDesiredPosition().x : element->GetDesiredPosition().y;
        float minorDesiredPosition = (!isParentLayoutDirRow) ? element->GetDesiredPosition().x : element->GetDesiredPosition().y;

        AxisLayout majorAxis{ MAJOR_AXIS, major, parent->GetLayoutMajor(), parentWrap, parentSizeMajor,
          totalSizeMajor, majorDesiredPosition };
        AxisLayout minorAxis{ MINOR_AXIS, minor, parent->GetLayoutMinor(), parentWrap, parentSizeMinor,
            0, minorDesiredPosition };

        Vector4 margin = element->GetMargin();
        float marginMajorStart = (isParentLayoutDirRow) ? margin.w : margin.x;
        float marginMajorEnd = (isParentLayoutDirRow) ? margin.y : margin.z;
        float marginMinorStart = (!isParentLayoutDirRow) ? margin.w : margin.x;
        float marginMinorEnd = (!isParentLayoutDirRow) ? margin.y : margin.z;
        majorAxis.SetMargin(marginMajorStart, marginMajorEnd);
        minorAxis.SetMargin(marginMinorStart, marginMinorEnd);

        Vector4 border = element->GetBorderSize();
        float borderMajorStart = (isParentLayoutDirRow) ? border.y : border.x;
        float borderMajorEnd = (isParentLayoutDirRow) ? border.w : border.z;
        float borderMinorStart = (!isParentLayoutDirRow) ? border.y : border.x;
        float borderMinorEnd = (!isParentLayoutDirRow) ? border.w : border.z;
        majorAxis.SetBorder(borderMajorStart, borderMajorEnd);
        minorAxis.SetBorder(borderMinorStart, borderMinorEnd);

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

        Vector2 parentSize = parent->GetContentSize();
        float parentSizeMajor = (major == Axis::X) ? parentSize.x : parentSize.y;
        float parentSizeMinor = (major != Axis::X) ? parentSize.x : parentSize.y;
        
        float totalPixelSizeMajor = (major == Axis::X) ? element->ComputeSiblingsTotalDesiredPixelSize().x : element->ComputeSiblingsTotalDesiredPixelSize().y;
        auto unitArray = element->GetSizeUnits();
        StyleUnit::Unit majorUnit = (isParentLayoutDirRow) ? unitArray[0] : unitArray[1];
        StyleUnit::Unit minorUnit = (!isParentLayoutDirRow) ? unitArray[0] : unitArray[1];

        Vector2 majorSize = (majorUnit == StyleUnit::Unit::Percent_A) ? element->GetDesiredSize() : element->GetDesiredPixelSize();
        Vector2 minorSize = (minorUnit == StyleUnit::Unit::Percent_A) ? element->GetDesiredSize() : element->GetDesiredPixelSize();

        float majorDesiredPixelSize = (isParentLayoutDirRow) ? majorSize.x : majorSize.y;
        float minorDesiredPixelSize = (!isParentLayoutDirRow) ? minorSize.x : minorSize.y;

        AxisLayout majorAxis{ MAJOR_AXIS, major, parent->GetLayoutMajor(), parentWrap, parentSizeMajor,
            totalPixelSizeMajor, majorDesiredPixelSize, majorUnit };
        AxisLayout minorAxis{ MINOR_AXIS, minor, parent->GetLayoutMinor(), parentWrap, parentSizeMinor,
            0, minorDesiredPixelSize, minorUnit };
    
        Vector4 margin = element->GetMargin();
        float marginMajorStart = (isParentLayoutDirRow) ? margin.w : margin.x;
        float marginMajorEnd = (isParentLayoutDirRow) ? margin.y : margin.z;
        float marginMinorStart = (!isParentLayoutDirRow) ? margin.w : margin.x;
        float marginMinorEnd = (!isParentLayoutDirRow) ? margin.y : margin.z;
        majorAxis.SetMargin(marginMajorStart, marginMajorEnd);
        minorAxis.SetMargin(marginMinorStart, marginMinorEnd);

        Vector4 border = element->GetBorderSize();
        float borderMajorStart = (isParentLayoutDirRow) ? border.y : border.x;
        float borderMajorEnd = (isParentLayoutDirRow) ? border.w : border.z;
        float borderMinorStart = (!isParentLayoutDirRow) ? border.y : border.x;
        float borderMinorEnd = (!isParentLayoutDirRow) ? border.w : border.z;
        majorAxis.SetBorder(borderMajorStart, borderMajorEnd);
        minorAxis.SetBorder(borderMinorStart, borderMinorEnd);
        
        // Calculates the amount of children that will be stretched
        size_t cStretchCountMajor = 0, cAvaibleStretchCountMajor = 0;
        for (const auto& c : parent->GetChildren()) {
            auto units = c->GetSizeUnits();
            if (isParentLayoutDirRow) {
                // major axis X
                if (units[0] == StyleUnit::Unit::Percent_A)
                    cAvaibleStretchCountMajor++;
                else if (c->GetDesiredSize().x <= 0.0f)
                    cStretchCountMajor++;
            }
            else {
                // major axis Y
                if (units[1] == StyleUnit::Unit::Percent_A)
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
    
    float FlexLayoutCalculator::AxisLayout::CalculatePosition(
        float previousEndPos,
        float previousEndSize,
        float elementSize,
        size_t siblingCount,
        size_t elementIndex) const
    {
        float freeSpace = m_parentSize - (m_totalChildrenSize + elementSize);

        switch (m_align)
        {
        case EngineCore::UI::Flex::LayoutAlign::Start:
            return previousEndPos + (m_isMajorAxis ? previousEndSize : 0.0f) + m_marginStart;

        case EngineCore::UI::Flex::LayoutAlign::Center:
            if (m_isMajorAxis) {
                if (elementIndex == 0)
                    return freeSpace * 0.5f;
                return previousEndPos + previousEndSize;
            }
            else {
                // Minor axis: always center relative to parent
                return (m_parentSize - elementSize) * 0.5f;
            }

        case EngineCore::UI::Flex::LayoutAlign::End:
            if (m_isMajorAxis) {
                if (elementIndex == 0)
                    return m_parentSize - elementSize - m_marginEnd;
                return previousEndPos - previousEndSize;
            }
            else {
                // Minor axis: align relative to parent
                return m_parentSize - elementSize - m_marginEnd;
            }

        case EngineCore::UI::Flex::LayoutAlign::Stretch:
            return previousEndPos + (m_isMajorAxis ? previousEndSize : 0.0f) + m_marginStart;

        case EngineCore::UI::Flex::LayoutAlign::SpaceEvenly:
            if (m_isMajorAxis && siblingCount > 1) {
                float space = (m_parentSize - m_totalChildrenSize) / (siblingCount + 1);
                return previousEndPos + previousEndSize + space;
            }
            return previousEndPos + (m_isMajorAxis ? previousEndSize : 0.0f);

        case EngineCore::UI::Flex::LayoutAlign::SpaceAround:
            if (m_isMajorAxis && siblingCount > 0) {
                float space = (m_parentSize - m_totalChildrenSize) / siblingCount;
                return previousEndPos + previousEndSize + (space * 0.5f);
            }
            return previousEndPos + (m_isMajorAxis ? previousEndSize : 0.0f);

        default:
            return previousEndPos + (m_isMajorAxis ? previousEndSize : 0.0f);
        }
    }

    float FlexLayoutCalculator::AxisLayout::CalculateSize(size_t stretchCount, size_t avaibleStretchCount) const {
        if (m_wrap)
            return m_desired + m_borderStart + m_borderEnd;

        float result = m_desired;

        if (m_unit == StyleUnit::Unit::Percent_A) {
            if (avaibleStretchCount > 0 && m_isMajorAxis) {
                float usableSpace = m_parentSize - m_totalChildrenSize;
                float adaptiveSize = ((usableSpace / avaibleStretchCount) * (m_desired / 100.0f));
                result = std::max(adaptiveSize, 0.0f);
            }
            else if (!m_isMajorAxis) {
                result = std::max(m_parentSize * (m_desired / 100.0f), 0.0f);
            }
        }
        else if (m_align == EngineCore::UI::Flex::LayoutAlign::Stretch) {
            if (m_desired > 0.0f)
                result = m_desired;
            else if (stretchCount > 0 && m_isMajorAxis) {
                float usableSpace = m_parentSize - m_totalChildrenSize;
                result = std::max(usableSpace / stretchCount, 0.0f);
            }
            else if (!m_isMajorAxis) {
                result = std::max(m_parentSize, 0.0f);
            }
        }

        // Add border, remove margin (margin affects only position)
        return std::max(result + m_borderStart + m_borderEnd, 0.0f);
    }

}