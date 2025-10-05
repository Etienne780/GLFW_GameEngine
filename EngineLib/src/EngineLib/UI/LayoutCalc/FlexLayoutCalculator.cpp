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

        float totalSizeMajor = (major == Axis::X) ? element->ComputeSiblingsTotalMarginSize().x : element->ComputeSiblingsTotalMarginSize().y;
        float majorAvaibleSize = (isParentLayoutDirRow) ? element->GetAviableSize().x : element->GetAviableSize().y;
        float minorAvaibleSize = (!isParentLayoutDirRow) ? element->GetAviableSize().x : element->GetAviableSize().y;

        Vector4 margin = element->GetMargin();
        float marginMajorStart = (isParentLayoutDirRow) ? margin.y : margin.x;
        float marginMajorEnd = (isParentLayoutDirRow) ? margin.w : margin.z;
        float marginMinorStart = (!isParentLayoutDirRow) ? margin.y : margin.x;
        float marginMinorEnd = (!isParentLayoutDirRow) ? margin.w : margin.z;

        float majorDesiredPosition = (isParentLayoutDirRow) ? element->GetDesiredPosition().x : element->GetDesiredPosition().y;
        float minorDesiredPosition = (!isParentLayoutDirRow) ? element->GetDesiredPosition().x : element->GetDesiredPosition().y;

        AxisLayout majorAxis{ major, parent->GetLayoutMajor(), parentWrap, parentSizeMajor,
          totalSizeMajor, marginMajorStart, marginMajorEnd, majorDesiredPosition, majorAvaibleSize };
        AxisLayout minorAxis{ minor, parent->GetLayoutMinor(), parentWrap, parentSizeMinor,
            0, marginMinorStart, marginMinorEnd, minorDesiredPosition, minorAvaibleSize };

        float majorSize = (isParentLayoutDirRow) ? element->GetLocalSize().x : element->GetLocalSize().y;
        float minorSize = (!isParentLayoutDirRow) ? element->GetLocalSize().x : element->GetLocalSize().y;

        size_t siblingCount = parent->GetChildCount();
        float preMajorEndPos = 0.0f;
        float preMajorSize = 0.0f;
        if (listPosition > 0.0f) {
            ElementBase* preElement = parent->GetChild(listPosition - 1);
            if (preElement) {
                preMajorEndPos = (isParentLayoutDirRow) ?
                    preElement->GetLocalPosition().x :
                    preElement->GetLocalPosition().y;

                preMajorSize = (isParentLayoutDirRow) ?
                    preElement->GetMarginSize().x :
                    preElement->GetMarginSize().y;
            }
        }

        Vector2 newPosition;
        if (isParentLayoutDirRow) {
            // major axis X
            newPosition.Set(
                majorAxis.CalculatePosition(preMajorEndPos, preMajorSize, majorSize, siblingCount),
                minorAxis.CalculatePosition(0.0f, 0.0f, majorSize, siblingCount)
            );
        }
        else {
            // major axis Y
            newPosition.Set(
                minorAxis.CalculatePosition(0.0f, 0.0f,  majorSize, siblingCount),
                majorAxis.CalculatePosition(preMajorEndPos, preMajorSize, majorSize, siblingCount)
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
        
        float totalSizeMajor = (major == Axis::X) ? element->ComputeSiblingsTotalMarginSize().x : element->ComputeSiblingsTotalMarginSize().y;
        float majorAvaibleSize = (isParentLayoutDirRow) ? element->GetAviableSize().x : element->GetAviableSize().y;
        float minorAvaibleSize = (!isParentLayoutDirRow) ? element->GetAviableSize().x : element->GetAviableSize().y;

        Vector4 margin = element->GetMargin();
        float marginMajorStart = (isParentLayoutDirRow) ? margin.y : margin.x;
        float marginMajorEnd = (isParentLayoutDirRow) ? margin.w : margin.z;
        float marginMinorStart = (!isParentLayoutDirRow) ? margin.y : margin.x;
        float marginMinorEnd = (!isParentLayoutDirRow) ? margin.w : margin.z;

        float majorDesiredSize = (isParentLayoutDirRow) ? element->GetDesiredSize().x : element->GetDesiredSize().y;
        float minorDesiredSize = (!isParentLayoutDirRow) ? element->GetDesiredSize().x : element->GetDesiredSize().y;

        AxisLayout majorAxis{ major, parent->GetLayoutMajor(), parentWrap, parentSizeMajor, 
            totalSizeMajor, marginMajorStart, marginMajorEnd, majorDesiredSize, majorAvaibleSize };
        AxisLayout minorAxis{ minor, parent->GetLayoutMinor(), parentWrap, parentSizeMinor, 
            0, marginMinorStart, marginMinorEnd, minorDesiredSize, minorAvaibleSize };
    
        // Calculates the amount of children that will be stretched
        size_t cStretchCountMajor = 0, cAvaibleStretchCountMajor = 0;
        for (const auto& c : parent->GetChildren()) {
            if (isParentLayoutDirRow) {
                // major axis X
                if (c->GetDesiredSize().x <= 0.0f)
                    cStretchCountMajor++;
                else if (c->GetAviableSize().x >= 0.0f)
                    cAvaibleStretchCountMajor++;
            }
            else {
                // major axis Y
                if (c->GetDesiredSize().y <= 0.0f)
                    cStretchCountMajor++;
                else if (c->GetAviableSize().y >= 0.0f)
                    cAvaibleStretchCountMajor++;
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

    float FlexLayoutCalculator::AxisLayout::CalculatePosition(float previousEndPos, float previousEndSize, float elementSize, size_t siblingCount) const {
        switch (m_align)
        {
        case EngineCore::UI::Flex::LayoutAlign::Start: {
            return (previousEndPos + previousEndSize);
        }
        case EngineCore::UI::Flex::LayoutAlign::Center: {
            return m_parentSize / 2 - m_totalChildrenSize/2 + (previousEndPos + previousEndSize);
        }
        case EngineCore::UI::Flex::LayoutAlign::End: {
            return m_parentSize - (m_parentSize - previousEndPos) - elementSize;
        }
        case EngineCore::UI::Flex::LayoutAlign::Stretch: {
            break;
        }
        case EngineCore::UI::Flex::LayoutAlign::SpaceEvenly: {
            // to lazy right now
            break;
        }
        case EngineCore::UI::Flex::LayoutAlign::SpaceAround: {
            // to lazy right now
            break;
        }
        default:
            return 0.0f;
        }
        return 0.0f;
    }

    float FlexLayoutCalculator::AxisLayout::CalculateSize(size_t stretchCount, size_t avaibleStretchCount) const {
        // stop stretch if wrap is enabled
        if (m_wrap && m_align == Flex::LayoutAlign::Stretch)
            return m_desired;

        // m_availableSize is -1 if not used
        if (m_availableSize >= 0.0f) {
            if (avaibleStretchCount == 0)
                return m_desired;

            float usableSpace = m_parentSize - m_totalChildrenSize;
            float adaptiveSize = ((usableSpace / avaibleStretchCount) * (m_availableSize / 100.0f)) - m_marginStart - m_marginEnd;
            return std::max(adaptiveSize, 0.0f);
        }

        switch (m_align)
        {
        // elements that have a desired size will not be streched, stretches elements to the avaible size left 
        case EngineCore::UI::Flex::LayoutAlign::Stretch: {
            // only do stretch if wrap is false
            if (stretchCount > 0 && !m_wrap) {
                float usableSpace = m_parentSize - m_totalChildrenSize;
                float stretchedSize = (usableSpace / stretchCount) - m_marginStart - m_marginEnd;
                return std::max(stretchedSize, 0.0f);
            }
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

/*
void ElementBase::UpdateLayoutPosition() {
    if (m_parentElementPtr) {
        bool wrap = (m_parentElementPtr->GetLayoutWrap() == FlexLayoutWrap::Wrap);
        FlexLayoutAlign alignMajor = m_parentElementPtr->GetLayoutMajor();
        FlexLayoutAlign alignMinor = m_parentElementPtr->GetLayoutMinor();
        auto& siblings = m_parentElementPtr->GetChildren();

        float xPosition = 0.0f;
        float yPosition = 0.0f;

        switch (m_parentElementPtr->GetLayoutDirection()) {
        case FlexLayoutDirection::Row: {
            // ---------- horizontal position ----------
            // if layouting start and not first element
            if (alignMajor == FlexLayoutAlign::Start &&
                m_listPosition > 0) {
                auto preElement = m_parentElementPtr->GetChild(m_listPosition - 1);
                xPosition = preElement->GetLocalPosition().x + preElement->GetMarginSize().x;
            }

            // -------- vertical position ----------
            if (alignMinor == FlexLayoutAlign::Center && !wrap) {
                float parentHeight = m_parentElementPtr->GetContentSize().y;
                yPosition = parentHeight / 2 - GetMarginSize().y / 2;
            }

            if (alignMinor == FlexLayoutAlign::End && !wrap) {
                float parentHeight = m_parentElementPtr->GetContentSize().y;
                yPosition = parentHeight - GetMarginSize().y;
            }

            break;
        }
        case FlexLayoutDirection::Column: {
            break;
        }
        }

        m_innerPosition.Set(xPosition, yPosition);
    }

    m_positionDirty = false;
}

void ElementBase::UpdateLayoutSize() {
    // stop stretch calculations if wrap is turned on

    if (m_parentElementPtr) {
        bool wrap = (m_parentElementPtr->GetLayoutWrap() == FlexLayoutWrap::Wrap);

        switch (m_parentElementPtr->GetLayoutDirection()) {
        case FlexLayoutDirection::Row:
            // ---------- horizontal position ----------
            // stretch
            if (m_aviableSize.x < 0.0f && m_innerSize.x <= 0.0f &&
                m_parentElementPtr->GetLayoutMajor() == FlexLayoutAlign::Stretch) {
                float totalChildWidth = ComputeSiblingsTotalMarginSize().x;
                float parentWidth = m_parentElementPtr->GetContentSize().x;
                // Calculates the amount of children that will be stretched
                size_t cStretchCount = 0;
                for (const auto& c : m_parentElementPtr->GetChildren()) {
                    if (c->GetSize().x <= 0.0f)
                        cStretchCount++;
                }
                m_innerSize.x = ((parentWidth - totalChildWidth) / cStretchCount) - m_margin.y - m_margin.w;
            }
            else if (m_aviableSize.x >= 0.0f && !wrap) {
                float totalChildWidth = ComputeSiblingsTotalMarginSize().x;
                float parentWidth = m_parentElementPtr->GetContentSize().x;

                size_t cStretchCount = 0;
                for (const auto& c : m_parentElementPtr->GetChildren()) {
                    if (c->GetAviableSize().x >= 0.0f)
                        cStretchCount++;
                }
                m_innerSize.x = ((parentWidth - totalChildWidth) / cStretchCount) - m_margin.y - m_margin.w;
            }

            // -------- vertical position ----------
            // stretch
            if (m_aviableSize.y < 0.0f && m_innerSize.y <= 0.0f &&
                m_parentElementPtr->GetLayoutMinor() == FlexLayoutAlign::Stretch) {
                float pSize = m_parentElementPtr->GetContentSize().y;
                // totalSize - topMargin - bottomMargin
                m_innerSize.y = pSize - m_margin.x - m_margin.z;
            }
            else if (m_aviableSize.y >= 0.0f && !wrap) {
                float pSize = m_parentElementPtr->GetContentSize().y;
                m_innerSize.y = pSize - m_margin.x - m_margin.z;
            }
            break;
        case FlexLayoutDirection::Column:
            // ---------- horizontal position ----------
            if (m_aviableSize.y < 0.0f && m_innerSize.y <= 0.0f &&
                m_parentElementPtr->GetLayoutMajor() == FlexLayoutAlign::Stretch) {
                float totalChildHeight = ComputeSiblingsTotalMarginSize().y;
                float parentHeight = m_parentElementPtr->GetContentSize().y;
                // Calculates the amount of children that will be stretched
                size_t cStretchCount = 0;
                for (const auto& c : m_parentElementPtr->GetChildren()) {
                    if (c->GetSize().y <= 0.0f)
                        cStretchCount++;
                }
                m_innerSize.y = ((parentHeight - totalChildHeight) / cStretchCount) - m_margin.x - m_margin.z;
            }
            else if (m_aviableSize.y >= 0.0f && !wrap) {
                float totalChildHeight = ComputeSiblingsTotalMarginSize().y;
                float parentHeight = m_parentElementPtr->GetContentSize().y;

                size_t cStretchCount = 0;
                for (const auto& c : m_parentElementPtr->GetChildren()) {
                    if (c->GetAviableSize().y >= 0.0f)
                        cStretchCount++;
                }
                m_innerSize.y = ((parentHeight - totalChildHeight) / cStretchCount) - m_margin.x - m_margin.z;
            }

            // -------- vertical position ----------
            if (m_aviableSize.x < 0.0f && m_innerSize.x <= 0.0f &&
                m_parentElementPtr->GetLayoutMinor() == FlexLayoutAlign::Stretch) {
                float pSize = m_parentElementPtr->GetContentSize().x;
                // (size - parentPadding) - leftMargin - rightMargin
                m_innerSize.x = pSize - m_margin.y - m_margin.w;
            }
            else if (m_aviableSize.x >= 0.0f && !wrap) {
                float pSize = m_parentElementPtr->GetContentSize().x;
                m_innerSize.x = pSize - m_margin.y - m_margin.w;
            }
            break;
        }
    }

    m_sizeDirty = false;
}*/