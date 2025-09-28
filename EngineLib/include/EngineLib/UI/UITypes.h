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

    struct StyleValue {
        using ValueVariant = std::variant<float, Vector2, Vector3, Vector4, std::string, std::vector<StyleValue>>;
        enum class Type { Float, Vector2, Vector3, Vector4, String, Multi } type = Type::Float;
        ValueVariant value;

        // Default constructor: initializes with float 0.0f
        StyleValue() : type(Type::Float), value(0.0f) {}

        // Float constructor
        StyleValue(float v) : type(Type::Float), value(v) {}

        // Vector2 constructor
        StyleValue(const Vector2& v) : type(Type::Vector2), value(v) {}

        // Vector3 constructor
        StyleValue(const Vector3& v) : type(Type::Vector3), value(v) {}

        // Vector4 constructor
        StyleValue(const Vector4& v) : type(Type::Vector4), value(v) {}

        // String constructor
        StyleValue(const std::string& s) : type(Type::String), value(s) {}
        StyleValue(const char* s) : type(Type::String), value(std::string(s)) {}
        // Multi constructor
        StyleValue(std::vector<StyleValue> v) : value(std::move(v)), type(Type::Multi) {}

        template<typename T>
        bool TryGetValue(T& out, const std::string& attributeName) const {
            if (std::holds_alternative<T>(value)) {
                out = std::get<T>(value);
                return true;
            }

            Log::Warn("StyleValue: Could not get value of Attribute '{}', expected type '{}', got '{}'",
                attributeName, typeid(T).name(), static_cast<int>(type));
            return false;
        }
    };

}

template<>
static inline std::string FormatUtils::toString<EngineCore::UI::StyleValue::Type>(EngineCore::UI::StyleValue::Type value) {
    using namespace EngineCore::UI;
    typedef StyleValue::Type T;
    switch (value) {
    case T::Float: return "float";
    case T::Vector2: return "Vector2";
    case T::Vector3: return "Vector3";
    case T::Vector4: return "Vector4";
    case T::String: return "String";
    default: return "UNKNOWN_TYPE";
    }
}
