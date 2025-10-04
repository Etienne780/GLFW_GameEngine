#include <CoreLib/Log.h>

#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/LayoutCalc/GridLayoutCalculator.h"

namespace EngineCore::UI {

    Vector2 GridLayoutCalculator::CalculatePosition(ElementBase* element) {
        Log::Warn("GridLayoutCalculator: Can not CalculatePosition GridLayoutCalculator is not implemented!");
    }

    Vector2 GridLayoutCalculator::CalculateSize(ElementBase* element) {
        Log::Warn("GridLayoutCalculator: Can not CalculateSize GridLayoutCalculator is not implemented!");
    }

}