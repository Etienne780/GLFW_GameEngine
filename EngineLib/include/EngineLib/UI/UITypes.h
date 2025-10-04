#pragma once
#include <string>
#include <optional>
#include <variant>
#include <CoreLib/Log.h>
#include <CoreLib/Math/Vector2.h>
#include <CoreLib/Math/Vector3.h>
#include <CoreLib/Math/Vector4.h>

namespace EngineCore::UI {

    enum class State {
        Normal,
        Hovered,
        Pressed,
        Focused,
        Disabled
    };

    enum class FlexLayoutDirection {
        Row,
        Column,
    };

    enum class FlexLayoutWrap {
        None,
        Wrap
    };

    enum class FlexLayoutAlign {
        Start,
        Center,
        End,
        Stretch,
        SpaceEvenly,
        SpaceAround
    };


    namespace StyleUnit {
   
        /**
        * @brief contains different UI units
        */
        enum class Unit {
            // is an invalid unit
            Unknown,
            // Pixels
            PX,
            // Percent width of the parent element
            Percent_W,
            // Percent height of the parent element
            Percent_H,
            // Available percent of the parent element for the given achses
            Percent_A,
            // viewport percent width
            VW,
            // viewport percent height
            VH,
            // seconds
            S,
            // milliseconds
            MS,
        };
    
        inline const std::vector<std::string>& GetSizeUnitStrings() {
            static std::vector<std::string> sizeUnits = { "px", "%w", "%h", "%a", "vw", "vh" };
            return sizeUnits;
        }
    
        inline const std::vector<std::string>& GetTimeUnitStrings() {
            static std::vector<std::string> timeUnits = { "s", "ms" };
            return timeUnits;
        }
    
        inline const std::vector<std::string>& GetUnitStrings() {
            static std::vector<std::string> units = [] {
                std::vector<std::string> u;
                auto sizeUntis = GetSizeUnitStrings();
                auto timeUntis = GetTimeUnitStrings();
    
                u.reserve(sizeUntis.size() + timeUntis.size());
                u.insert(u.end(), sizeUntis.begin(), sizeUntis.end());
                u.insert(u.end(), timeUntis.begin(), timeUntis.end());
                return u;
                }();
            return units;
        }
    
        inline size_t GetSizeUnitCount() {
            return GetSizeUnitStrings().size();
        }
    
        inline size_t GetTimeUnitCount() {
            return GetTimeUnitStrings().size();
        }
    
        inline size_t GetUnitCount() {
            return GetUnitStrings().size();
        }
    
        inline Unit StringToUnit(const std::string& unit) {
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
    
        inline std::string UnitToString(Unit unit) {
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

    inline FlexLayoutDirection ToLayoutDirection(const std::string& val) {
        if (val == "row") return FlexLayoutDirection::Row;
        if (val == "column") return FlexLayoutDirection::Column;
        return FlexLayoutDirection::Row;
    }

    inline FlexLayoutWrap ToLayoutWrap(const std::string& val) {
        if (val == "wrap") return FlexLayoutWrap::Wrap;
        return FlexLayoutWrap::None;
    }

    inline FlexLayoutAlign ToLayoutAlign(const std::string& val) {
        if (val == "center") return FlexLayoutAlign::Center;
        if (val == "end") return FlexLayoutAlign::End;
        if (val == "stretch") return FlexLayoutAlign::Stretch;
        if (val == "space-evenly") return FlexLayoutAlign::SpaceEvenly;
        if (val == "space-around") return FlexLayoutAlign::SpaceAround;
        return FlexLayoutAlign::Start;
    }

    struct StyleValue {
        using ValueVariant = std::variant<int, float, Vector2, Vector3, Vector4, std::string, std::vector<StyleValue>>;
        enum class Type { Int, Float, Vector2, Vector3, Vector4, String, Multi } valueType = Type::Float;
        std::vector<StyleUnit::Unit > unitTypes = { StyleUnit::Unit::Unknown };
        ValueVariant value;

        // Default constructor: initializes with float 0.0f
        StyleValue() : valueType(Type::Float), value(0.0f) {}

        //Int constructor
        StyleValue(int v, StyleUnit::Unit unit) : valueType(Type::Int), value(v), unitTypes({ unit }) {}

        // Float constructor
        StyleValue(float v, StyleUnit::Unit unit) : valueType(Type::Float), value(v), unitTypes({ unit }) {}

        // Vector2 constructor
        StyleValue(const Vector2& v, std::vector<StyleUnit::Unit> unit) : valueType(Type::Vector2), value(v), unitTypes(unit) {
#ifndef NDEBUG
            if (unit.size() != 2)
                Log::Warn("StyleValue: expected exactly 2 units for Vector2, but got {}. "
                    "This may lead to inconsistent behavior.", unit.size());
#endif 
        }

        // Vector3 constructor
        StyleValue(const Vector3& v, std::vector<StyleUnit::Unit> unit) : valueType(Type::Vector3), value(v), unitTypes(unit) {
#ifndef NDEBUG
            if (unit.size() != 3)
                Log::Warn("StyleValue: expected exactly 3 units for Vector3, but got {}. "
                    "This may lead to inconsistent behavior.", unit.size());
#endif 
        }

        // Vector4 constructor
        StyleValue(const Vector4& v, std::vector<StyleUnit::Unit> unit) : valueType(Type::Vector4), value(v), unitTypes(unit) {
#ifndef NDEBUG
            if (unit.size() != 4)
                Log::Warn("StyleValue: expected exactly 4 units for Vector4, but got {}. "
                    "This may lead to inconsistent behavior.", unit.size());
#endif 
        }

        // String constructor
        StyleValue(const std::string& s) : valueType(Type::String), value(s) {}
        StyleValue(const char* s) : valueType(Type::String), value(std::string(s)) {}
        // Multi constructor
        StyleValue(std::vector<StyleValue> v) : valueType(Type::Multi), value(std::move(v)) {}

        template<typename T>
        bool TryGetValue(T& out, const std::string& attributeName) const {
            if (std::holds_alternative<T>(value)) {
                out = std::get<T>(value);
                return true;
            }

            Log::Warn("StyleValue: Could not get value of Attribute '{}', expected type '{}', got '{}'",
                attributeName, typeid(T).name(), valueType);
            return false;
        }

        StyleUnit::Unit GetUnit() const  {
            return GetUnit(0);
        }

        StyleUnit::Unit GetUnit(size_t index) const {
            if (index < unitTypes.size())
                return unitTypes[index];
            Log::Info("StyleValue: index '{}' out of bounds for size '{}' (Type='{}', Units={})",
                index, unitTypes.size(), valueType, unitTypes);
            return StyleUnit::Unit::Unknown;
        }
    };

}
// UI Style value types
template<>
static inline std::string FormatUtils::toString<EngineCore::UI::StyleValue::Type>(EngineCore::UI::StyleValue::Type value) {
    using namespace EngineCore::UI;
    typedef StyleValue::Type T;
    switch (value) {
    case T::Int: return "int";
    case T::Float: return "float";
    case T::Vector2: return "Vector2";
    case T::Vector3: return "Vector3";
    case T::Vector4: return "Vector4";
    case T::String: return "String";
    case T::Multi: return "Multi";
    default: return "UNKNOWN_TYPE";
    }
}

template<>
static inline std::string FormatUtils::toString<EngineCore::UI::StyleUnit::Unit>(EngineCore::UI::StyleUnit::Unit value) {
    using namespace EngineCore::UI;
    return StyleUnit::UnitToString(value);
}

template<>
static inline std::string FormatUtils::toString<std::vector<EngineCore::UI::StyleUnit::Unit>>(std::vector<EngineCore::UI::StyleUnit::Unit> values) {
    using namespace EngineCore::UI;

    std::string unitsStr = "[";
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) unitsStr += ", ";
        unitsStr += StyleUnit::UnitToString(values[i]);
    }
    unitsStr += "]";
    return unitsStr;
}