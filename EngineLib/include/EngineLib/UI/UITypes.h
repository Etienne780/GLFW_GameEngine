#pragma once
#include <string>
#include <optional>
#include <variant>
#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>
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

    enum class LayoutType {
        Unknown,
        None,
        // like flex CSS
        Flex,
        // like grid CSS (Not implemented)
        Grid
    };

    inline LayoutType ToLayoutType(const std::string& typeStr) {
        if (FormatUtils::toLowerCase(typeStr) == "none") return LayoutType::None;
        if (FormatUtils::toLowerCase(typeStr) == "flex") return LayoutType::Flex;
        if (FormatUtils::toLowerCase(typeStr) == "grid") return LayoutType::Grid;
        Log::Error("UI::ToLayoutType: Unknown Layout type '{}', can not convert to LayoutType!", typeStr);
        return LayoutType::Unknown;
    }

    namespace Flex {

        enum class LayoutDirection {
            Row,
            Column,
        };

        enum class LayoutWrap {
            None,
            Wrap
        };

        enum class LayoutAlign {
            Start,
            Center,
            End,
            Stretch,
            SpaceEvenly,
            SpaceAround
        };

        inline LayoutDirection ToLayoutDirection(const std::string& dirStr) {
            if (FormatUtils::toLowerCase(dirStr) == "row") return LayoutDirection::Row;
            if (FormatUtils::toLowerCase(dirStr) == "column") return LayoutDirection::Column;
            return LayoutDirection::Row;
        }

        inline LayoutWrap ToLayoutWrap(const std::string& wrapStr) {
            if (FormatUtils::toLowerCase(wrapStr) == "wrap") return LayoutWrap::Wrap;
            return LayoutWrap::None;
        }

        inline LayoutAlign ToLayoutAlign(const std::string& alignStr) {
            if (FormatUtils::toLowerCase(alignStr) == "center") return LayoutAlign::Center;
            if (FormatUtils::toLowerCase(alignStr) == "end") return LayoutAlign::End;
            if (FormatUtils::toLowerCase(alignStr) == "stretch") return LayoutAlign::Stretch;
            if (FormatUtils::toLowerCase(alignStr) == "space-evenly") return LayoutAlign::SpaceEvenly;
            if (FormatUtils::toLowerCase(alignStr) == "space-around") return LayoutAlign::SpaceAround;
            return LayoutAlign::Start;
        }
    }

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
    
        /**
        * @brief Returns a list of all size-related unit strings (e.g., px, %w, %h, etc.).
        * @return Reference to a vector containing all size unit strings.
        */
        inline const std::vector<std::string>& GetSizeUnitStrings() {
            static std::vector<std::string> sizeUnits = { "px", "%w", "%h", "%a", "vw", "vh" };
            return sizeUnits;
        }
    
        /**
        * @brief Returns a list of all time-related unit strings (e.g., s, ms).
        * @return Reference to a vector containing all time unit strings.
        */
        inline const std::vector<std::string>& GetTimeUnitStrings() {
            static std::vector<std::string> timeUnits = { "s", "ms" };
            return timeUnits;
        }
    
        /**
        * @brief Returns a list of all valid unit strings including both size and time units.
        * @return Reference to a vector containing all unit strings.
        */
        inline const std::vector<std::string>& GetUnitStrings() {
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
    
        /**
        * @brief Returns the number of available size units.
        * @return Number of size unit strings.
        */
        inline size_t GetSizeUnitCount() {
            return GetSizeUnitStrings().size();
        }
    
        /**
        * @brief Returns the number of available time units.
        * @return Number of time unit strings.
        */
        inline size_t GetTimeUnitCount() {
            return GetTimeUnitStrings().size();
        }
    
        /**
        * @brief Returns the total number of defined units (size + time).
        * @return Total count of unit strings.
        */
        inline size_t GetUnitCount() {
            return GetUnitStrings().size();
        }
    
        /**
        * @brief Converts a unit string (e.g., "px", "vw") to its corresponding Unit enum value.
        * @param unit String representation of the unit.
        * @return Corresponding Unit enum, or Unit::Unknown if not recognized.
        */
        inline Unit ToUnit(const std::string& unit) {
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
    
        /**
        * @brief Converts a Unit enum value back into its string representation.
        * @param unit The Unit enum to convert.
        * @return The corresponding string representation, or "Unknown" if invalid.
        */
        inline std::string ToString(Unit unit) {
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

    class StyleValue {
    public:
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
                    "This may lead to inconsistent behavior!", unit.size());
#endif 
        }

        // Vector3 constructor
        StyleValue(const Vector3& v, std::vector<StyleUnit::Unit> unit) : valueType(Type::Vector3), value(v), unitTypes(unit) {
#ifndef NDEBUG
            if (unit.size() != 3)
                Log::Warn("StyleValue: expected exactly 3 units for Vector3, but got {}. "
                    "This may lead to inconsistent behavior!", unit.size());
#endif 
        }

        // Vector4 constructor
        StyleValue(const Vector4& v, std::vector<StyleUnit::Unit> unit) : valueType(Type::Vector4), value(v), unitTypes(unit) {
#ifndef NDEBUG
            if (unit.size() != 4)
                Log::Warn("StyleValue: expected exactly 4 units for Vector4, but got {}. "
                    "This may lead to inconsistent behavior!", unit.size());
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

            Log::Warn("StyleValue: Could not get value of Attribute '{}', expected type '{}', got '{}'!",
                attributeName, GetReadableTypeName(typeid(T)), valueType);
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

    private:
        // Helper: Returns a readable string for the given std::type_info
        static const char* GetReadableTypeName(const std::type_info& type) {
            if (type == typeid(int)) return "int";
            if (type == typeid(float)) return "float";
            if (type == typeid(Vector2)) return "Vector2";
            if (type == typeid(Vector3)) return "Vector3";
            if (type == typeid(Vector4)) return "Vector4";
            if (type == typeid(std::string)) return "string";
            if (type == typeid(std::vector<StyleValue>)) return "StyleValue[]";
            return "unknown";
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
    return StyleUnit::ToString(value);
}

template<>
static inline std::string FormatUtils::toString<std::vector<EngineCore::UI::StyleUnit::Unit>>(std::vector<EngineCore::UI::StyleUnit::Unit> values) {
    using namespace EngineCore::UI;

    std::string unitsStr = "[";
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) unitsStr += ", ";
        unitsStr += StyleUnit::ToString(values[i]);
    }
    unitsStr += "]";
    return unitsStr;
}