#include <CoreLib/Log.h>

#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/LayoutCalc/FlexLayoutCalculator.h"

namespace EngineCore::UI {

    void FlexLayoutCalculator::CalculatePosition(ElementBase* element) {
        
    }

    void FlexLayoutCalculator::CalculateSize(ElementBase* element) {
        
    }

    void FlexLayoutCalculator::CalculateMajorAxis(ElementBase* element, bool wrap) {
        
    }

    void FlexLayoutCalculator::CalculateMinorAxis(ElementBase* element, bool wrap) {
        
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