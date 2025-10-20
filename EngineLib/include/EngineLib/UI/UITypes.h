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
    class ElementBase;

    /**
    * @brief Represents the visual interaction state of a UI element.
    */
    enum class State {
        Normal,
        Hovered,
        Pressed,
        Focused,
        Disabled
    };

    /**
    * @brief Defines layout container behavior (similar to CSS layout types).
    */
    enum class LayoutType {
        Unknown,
        None,
        // like flex CSS
        Flex,
        // like grid CSS (Not implemented)
        Grid
    };

    /**
    * @brief Converts a UI::State enum value into a human-readable string.
    * @param s The UI state to convert.
    * @return std::string A string representing the given state ("Normal", "Hovered", etc.).
    */
    std::string StateToString(UI::State s);

    /**
    * @brief Converts a string representation of a layout type to its corresponding enum value.
    * @param typeStr Input string representing the layout type (e.g., "flex", "grid").
    * @return Corresponding LayoutType enum, or LayoutType::Unknown if not recognized.
    */
    LayoutType ToLayoutType(const std::string& typeStr);

    std::string ToString(LayoutType layoutType);

    namespace Flex {

        /**
        * @brief Defines the primary axis direction for a flex container.
        */
        enum class LayoutDirection {
            Row,
            Column,
        };

        /**
        * @brief Defines whether and how flex items wrap onto multiple lines.
        */
        enum class LayoutWrap {
            None,
            Wrap
        };

        /**
        * @brief Defines alignment along the main or cross axis in a flex container.
        */
        enum class LayoutAlign {
            Start,
            Center,
            End,
            Stretch,
            SpaceEvenly,
            SpaceAround
        };

        /**
        * @brief Converts a string into a corresponding LayoutDirection enum value.
        * @param dirStr Input string (e.g., "row", "column").
        * @return Corresponding LayoutDirection enum, defaults to Row if invalid.
        */
        LayoutDirection ToLayoutDirection(const std::string& dirStr);
        /**
        * @brief Converts a string into a corresponding LayoutWrap enum value.
        * @param wrapStr Input string (e.g., "wrap", "none").
        * @return Corresponding LayoutWrap enum, defaults to None if invalid.
        */
        LayoutWrap ToLayoutWrap(const std::string& wrapStr);

        /**
        * @brief Converts a string into a corresponding LayoutAlign enum value.
        * @param alignStr Input string (e.g., "center", "end", "stretch").
        * @return Corresponding LayoutAlign enum, defaults to Start if invalid.
        */
        LayoutAlign ToLayoutAlign(const std::string& alignStr);

        std::string ToString(LayoutDirection layoutDirection);
        std::string ToString(LayoutWrap layoutWrap);
        std::string ToString(LayoutAlign layoutAlign);

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
            Percent_VW,
            // viewport percent height
            Percent_VH,
            // seconds
            S,
            // milliseconds
            MS,
        };
    
        /**
        * @brief Converts a numerical value expressed in a specific unit into pixels.
        * @param value Numeric value to be converted.
        * @param unit Unit type of the input value (e.g., PX, Percent_W, Percent_H).
        * @param element Reference to the element used for relative size evaluation.
        * @return Converted value in pixels.
        */
        float EvaluateSizeUnit(float value, Unit unit, const ElementBase& element);

        /**
        * @brief Converts a numerical value expressed in a specific unit into seconds.
        * @param value Numeric value to be converted.
        * @param unit Unit type of the input value (e.g., S, MS).
        * @return Converted value in seconds.
        */
        float EvaluateTimeUnit(float value, Unit unit);

        /**
        * @brief Returns a list of all size-related unit strings (e.g., px, %w, %h, etc.).
        * @return Reference to a vector containing all size unit strings.
        */
        const std::vector<std::string>& GetSizeUnitStrings();
    
        /**
        * @brief Returns a list of all time-related unit strings (e.g., s, ms).
        * @return Reference to a vector containing all time unit strings.
        */
        const std::vector<std::string>& GetTimeUnitStrings();
    
        /**
        * @brief Returns a list of all valid unit strings including both size and time units.
        * @return Reference to a vector containing all unit strings.
        */
        const std::vector<std::string>& GetUnitStrings();
    
        /**
        * @brief Returns the number of available size units.
        * @return Number of size unit strings.
        */
        size_t GetSizeUnitCount();
    
        /**
        * @brief Returns the number of available time units.
        * @return Number of time unit strings.
        */
        size_t GetTimeUnitCount();
    
        /**
        * @brief Returns the total number of defined units (size + time).
        * @return Total count of unit strings.
        */
        size_t GetUnitCount();
    
        /**
        * @brief Converts a unit string (e.g., "px", "vw") to its corresponding Unit enum value.
        * @param unit String representation of the unit.
        * @return Corresponding Unit enum, or Unit::Unknown if not recognized.
        */
        Unit ToUnit(const std::string& unit);
    
        /**
        * @brief Converts a Unit enum value back into its string representation.
        * @param unit The Unit enum to convert.
        * @return The corresponding string representation, or "Unknown" if invalid.
        */
        std::string ToString(Unit unit);
    
    }

    class StyleValue {
    public:
        enum class Type { Int, Float, Vector2, Vector3, Vector4, String, Multi };

        /**
        * @brief Default constructor, initializes value as float(0.0f).
        */
        StyleValue();
        /**
        * @brief Constructs a StyleValue from an integer and a unit.
        * @param v Integer value.
        * @param unit Unit type.
        */
        StyleValue(int v, StyleUnit::Unit unit);
        /**
        * @brief Constructs a StyleValue from a float and a unit.
        * @param v Float value.
        * @param unit Unit type.
        */
        StyleValue(float v, StyleUnit::Unit unit);
        /**
        * @brief Constructs a StyleValue from a Vector2 and multiple units.
        * @param v Vector2 value.
        * @param unit Vector of corresponding units.
        */
        StyleValue(const Vector2& v, std::vector<StyleUnit::Unit> unit);
        /**
        * @brief Constructs a StyleValue from a Vector3 and multiple units.
        * @param v Vector3 value.
        * @param unit Vector of corresponding units.
        */
        StyleValue(const Vector3& v, std::vector<StyleUnit::Unit> unit);
        /**
        * @brief Constructs a StyleValue from a Vector4 and multiple units.
        * @param v Vector4 value.
        * @param unit Vector of corresponding units.
        */
        StyleValue(const Vector4& v, std::vector<StyleUnit::Unit> unit);
        /**
        * @brief Constructs a StyleValue from a string.
        * @param s Input string.
        */
        StyleValue(const std::string& s);
        /**
        * @brief Constructs a StyleValue from a C-style string.
        * @param s Input C-string.
        */
        StyleValue(const char* s);
        /**
        * @brief Constructs a StyleValue containing multiple StyleValue entries.
        * @param v Vector of StyleValue elements.
        */
        StyleValue(std::vector<StyleValue> v);
        /**
        * @brief Attempts to retrieve the stored value as a specific type.
        * @tparam T Expected value type.
        * @param out Output reference to receive the value.
        * @param attributeName Name of the attribute for logging.
        * @return True if successful, false if type mismatch.
        */
        template<typename T>
        bool TryGetValue(T& out, const std::string& attributeName) const {
            if (std::holds_alternative<T>(m_value)) {
                out = std::get<T>(m_value);
                return true;
            }

            Log::Warn("StyleValue: Could not get value of Attribute '{}', expected type '{}', got '{}'!",
                attributeName, GetReadableTypeName(typeid(T)), m_valueType);
            return false;
        }

        /**
        * @brief Returns the current type of the style value.
        * @return The StyleValue::Type representing the stored data type.
        */
        Type GetType() const;

        /**
        * @brief Returns the unit associated with this value.
        * @return The primary StyleUnit::Unit value.
        */
        StyleUnit::Unit GetUnit() const;
        /**
        * @brief Returns the unit at a specific index for multi-unit values.
        * @param index Index of the unit to retrieve.
        * @return The StyleUnit::Unit at the given index.
        */
        StyleUnit::Unit GetUnit(size_t index) const;
        /**
        * @brief Returns all units stored in this value (non-const version).
        * @return A copy of the vector containing all StyleUnit::Unit entries.
        */
        std::vector<StyleUnit::Unit> GetAllUnits();
        /**
        * @brief Returns all units stored in this value (const version).
        * @return A const reference to the vector containing all StyleUnit::Unit entries.
        */
        const std::vector<StyleUnit::Unit>& GetAllUnits() const;
        /**
        * @brief Returns the number of units contained in this value.
        * @return The total count of StyleUnit::Unit entries.
        */
        size_t GetNumberOfUnits() const;

    private:
        using ValueVariant = std::variant<int, float, Vector2, Vector3, Vector4, std::string, std::vector<StyleValue>>;
        /**
        * @brief Defines the underlying type of the StyleValue.
        */
        Type m_valueType = Type::Float;
        std::vector<StyleUnit::Unit > m_unitTypes = { StyleUnit::Unit::Unknown };
        ValueVariant m_value;

        /**
        * @brief Helper function that returns a readable type name for type_info.
        * @param type Type information.
        * @return C-string representing the readable type name.
        */
        static const char* GetReadableTypeName(const std::type_info& type);
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
static inline std::string FormatUtils::toString<EngineCore::UI::State>(EngineCore::UI::State value) {
    using namespace EngineCore::UI;
    return StateToString(value);
}

template<>
static inline std::string FormatUtils::toString<EngineCore::UI::LayoutType>(EngineCore::UI::LayoutType value) {
    using namespace EngineCore::UI;
    return ToString(value);
}

template<>
static inline std::string FormatUtils::toString<EngineCore::UI::Flex::LayoutDirection>(EngineCore::UI::Flex::LayoutDirection value) {
    using namespace EngineCore::UI;
    return Flex::ToString(value);
}

template<>
static inline std::string FormatUtils::toString<EngineCore::UI::Flex::LayoutWrap>(EngineCore::UI::Flex::LayoutWrap value) {
    using namespace EngineCore::UI;
    return Flex::ToString(value);
}

template<>
static inline std::string FormatUtils::toString<EngineCore::UI::Flex::LayoutAlign>(EngineCore::UI::Flex::LayoutAlign value) {
    using namespace EngineCore::UI;
    return Flex::ToString(value);
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