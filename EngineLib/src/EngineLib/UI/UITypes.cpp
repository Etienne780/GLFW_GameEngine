#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/UIManager.h"
#include "EngineLib/UI/UITypes.h"

namespace EngineCore::UI {
	
    std::string StateToString(UI::State s) {
        switch (s) {
        case UI::State::Normal:   return "Normal";
        case UI::State::Hovered:  return "Hovered";
        case UI::State::Pressed:  return "Pressed";
        case UI::State::Focused:  return "Focused";
        case UI::State::Disabled: return "Disabled";
        default:                  return "Unknown";
        }
    }

    LayoutType ToLayoutType(const std::string& typeStr) {
        if (FormatUtils::toLowerCase(typeStr) == "none") return LayoutType::None;
        if (FormatUtils::toLowerCase(typeStr) == "flex") return LayoutType::Flex;
        if (FormatUtils::toLowerCase(typeStr) == "grid") return LayoutType::Grid;
        Log::Error("UI::ToLayoutType: Unknown Layout type '{}', can not convert to LayoutType!", typeStr);
        return LayoutType::Unknown;
    }

    namespace Flex {
    
        LayoutDirection ToLayoutDirection(const std::string& dirStr) {
            if (FormatUtils::toLowerCase(dirStr) == "row") return LayoutDirection::Row;
            if (FormatUtils::toLowerCase(dirStr) == "column") return LayoutDirection::Column;
            return LayoutDirection::Row;
        }

        LayoutWrap ToLayoutWrap(const std::string& wrapStr) {
            if (FormatUtils::toLowerCase(wrapStr) == "wrap") return LayoutWrap::Wrap;
            return LayoutWrap::None;
        }

        LayoutAlign ToLayoutAlign(const std::string& alignStr) {
            if (FormatUtils::toLowerCase(alignStr) == "center") return LayoutAlign::Center;
            if (FormatUtils::toLowerCase(alignStr) == "end") return LayoutAlign::End;
            if (FormatUtils::toLowerCase(alignStr) == "stretch") return LayoutAlign::Stretch;
            if (FormatUtils::toLowerCase(alignStr) == "space-evenly") return LayoutAlign::SpaceEvenly;
            if (FormatUtils::toLowerCase(alignStr) == "space-around") return LayoutAlign::SpaceAround;
            return LayoutAlign::Start;
        }

    }

    namespace StyleUnit {
    
        float EvaluateSizeUnit(float value, Unit unit, const ElementBase& element) {
            switch (unit)
            {
            case EngineCore::UI::StyleUnit::Unit::PX:
                return value;
            case EngineCore::UI::StyleUnit::Unit::Percent_W:
                return element.GetParentWidth() * value / 100.0f;
            case EngineCore::UI::StyleUnit::Unit::Percent_H:
                return element.GetParentHeight() * value / 100.0f;
            case EngineCore::UI::StyleUnit::Unit::Percent_VW:
                return element.GetViewportWidth() * value / 100.0f;
            case EngineCore::UI::StyleUnit::Unit::Percent_VH:
                return element.GetViewportHeight() * value / 100.0f;
            case EngineCore::UI::StyleUnit::Unit::Percent_A:
                Log::Error("UI::StyleUnit::EvaluateTimeUnit: Could not evaluate unit, unit '{}' needs to be calculatet by the Layout Calc!", unit);
                break;
                // Wrong units
            case EngineCore::UI::StyleUnit::Unit::S:
            case EngineCore::UI::StyleUnit::Unit::MS: {
                Log::Error("UI::StyleUnit::EvaluateTimeUnit: Could not evaluate unit, unit '{}' is not a size unit!", unit);
                break;
            }
            case EngineCore::UI::StyleUnit::Unit::Unknown: {
                Log::Error("UI::StyleUnit::EvaluateTimeUnit: Could not evaluate unit, unit is Unknown!", unit);
                break;
            }
            }

            return value;
        }

        float EvaluateTimeUnit(float value, Unit unit) {
            switch (unit)
            {
            case EngineCore::UI::StyleUnit::Unit::S:
                return value;
            case EngineCore::UI::StyleUnit::Unit::MS:
                return value / 1000;
                // Wrong units
            case EngineCore::UI::StyleUnit::Unit::PX:
            case EngineCore::UI::StyleUnit::Unit::Percent_W:
            case EngineCore::UI::StyleUnit::Unit::Percent_H:
            case EngineCore::UI::StyleUnit::Unit::Percent_A:
            case EngineCore::UI::StyleUnit::Unit::Percent_VW:
            case EngineCore::UI::StyleUnit::Unit::Percent_VH: {
                Log::Error("UI::StyleUnit::EvaluateTimeUnit: Could not evaluate unit, unit '{}' is not a time unit!", unit);
                break;
            }
            case EngineCore::UI::StyleUnit::Unit::Unknown: {
                Log::Error("UI::StyleUnit::EvaluateTimeUnit: Could not evaluate unit, unit is Unknown!", unit);
                break;
            }
            }

            return value;
        }

        const std::vector<std::string>& GetSizeUnitStrings() {
            static std::vector<std::string> sizeUnits = { "px", "%w", "%h", "%a", "vw", "vh" };
            return sizeUnits;
        }

        const std::vector<std::string>& GetTimeUnitStrings() {
            static std::vector<std::string> timeUnits = { "s", "ms" };
            return timeUnits;
        }

        const std::vector<std::string>& GetUnitStrings() {
            static std::vector<std::string> units = [] {
                std::vector<std::string> u;
                auto& sizeUntis = GetSizeUnitStrings();
                auto& timeUntis = GetTimeUnitStrings();

                u.reserve(sizeUntis.size() + timeUntis.size());
                u.insert(u.end(), sizeUntis.begin(), sizeUntis.end());
                u.insert(u.end(), timeUntis.begin(), timeUntis.end());
                return u;
                }();
            return units;
        }

        size_t GetSizeUnitCount() {
            return GetSizeUnitStrings().size();
        }

        size_t GetTimeUnitCount() {
            return GetTimeUnitStrings().size();
        }

        size_t GetUnitCount() {
            return GetUnitStrings().size();
        }

        Unit ToUnit(const std::string& unit) {
            const auto& units = GetUnitStrings();
            auto it = std::find(units.begin(), units.end(), unit);

            if (it != units.end()) {
                // calculate enum index
                size_t index = std::distance(units.begin(), it);
                return static_cast<Unit>(index + 1);
                // + 1 because enum 0 = Unknown
            }
            return Unit::Unknown;
        }

        std::string ToString(Unit unit) {
            if (unit == Unit::Unknown) {
                return "Unknown";
            }
            // - 1 because enum 0 = Unknown and in untis vector 0 = px
            size_t index = static_cast<size_t>(unit) - 1;
            const auto& units = GetUnitStrings();
            if (index < units.size()) {
                return units[index];
            }
            return "Unknown";
        }
    
    }

    #pragma region class StyleValue

    StyleValue::StyleValue() 
        : valueType(Type::Float), value(0.0f) {
    }

    StyleValue::StyleValue(int v, StyleUnit::Unit unit) 
        : valueType(Type::Int), value(v), unitTypes({ unit }) {
    }

    StyleValue::StyleValue(float v, StyleUnit::Unit unit) 
        : valueType(Type::Float), value(v), unitTypes({ unit }) {
    }

    StyleValue::StyleValue(const Vector2& v, std::vector<StyleUnit::Unit> unit) 
        : valueType(Type::Vector2), value(v), unitTypes(unit) {
#ifndef NDEBUG
            if (unit.size() != 2)
                Log::Warn("StyleValue: expected exactly 2 units for Vector2, but got {}. This may lead to inconsistent behavior!", 
                    unit.size());
#endif 
    }

    StyleValue::StyleValue(const Vector3& v, std::vector<StyleUnit::Unit> unit) 
        : valueType(Type::Vector3), value(v), unitTypes(unit) {
#ifndef NDEBUG
            if (unit.size() != 3)
                Log::Warn("StyleValue: expected exactly 3 units for Vector3, but got {}. This may lead to inconsistent behavior!", 
                    unit.size());
#endif 
    }

    StyleValue::StyleValue(const Vector4& v, std::vector<StyleUnit::Unit> unit) 
        : valueType(Type::Vector4), value(v), unitTypes(unit) {
#ifndef NDEBUG
            if (unit.size() != 4)
                Log::Warn("StyleValue: expected exactly 4 units for Vector4, but got {}. This may lead to inconsistent behavior!", unit.size());
#endif 
    }

    StyleValue::StyleValue(const std::string& s) 
        : valueType(Type::String), value(s) {
    }

    StyleValue::StyleValue(const char* s) 
        : valueType(Type::String), value(std::string(s)) {
    }

    StyleValue::StyleValue(std::vector<StyleValue> v) 
        : valueType(Type::Multi), value(std::move(v)) {
    }

    StyleUnit::Unit StyleValue::GetUnit() const {
        return GetUnit(0);
    }
    
    StyleUnit::Unit StyleValue::GetUnit(size_t index) const {
        if (index < unitTypes.size())
            return unitTypes[index];
        Log::Info("StyleValue: index '{}' out of bounds for size '{}' (Type='{}', Units={})",
            index, unitTypes.size(), valueType, unitTypes);
        return StyleUnit::Unit::Unknown;
    }
    
    const char* StyleValue::GetReadableTypeName(const std::type_info& type) {
        if (type == typeid(int)) return "int";
        if (type == typeid(float)) return "float";
        if (type == typeid(Vector2)) return "Vector2";
        if (type == typeid(Vector3)) return "Vector3";
        if (type == typeid(Vector4)) return "Vector4";
        if (type == typeid(std::string)) return "string";
        if (type == typeid(std::vector<StyleValue>)) return "StyleValue[]";
        return "unknown";
    }

    #pragma endregion

}