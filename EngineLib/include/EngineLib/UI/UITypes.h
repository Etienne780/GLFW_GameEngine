#pragma once
#include <string>
#include <optional>
#include <variant>
#include <CoreLib/Log.h>
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

    enum class Unit {
        Px,
        Percent,
        None
    };

    struct StyleValue {
        std::variant<float, std::string, Vector3, Vector4> value;
        Unit unit = Unit::None;

        StyleValue() : value(0.0f), unit(Unit::None) {}
        StyleValue(float v, Unit u = Unit::None) : value(v), unit(u) {}
        StyleValue(const std::string& s) : value(s) {}
        StyleValue(const Vector3& v) : value(v) {}
        StyleValue(const Vector4& v) : value(v) {}

        template<typename T>
        std::optional<T> GetValue() const {
            if (std::holds_alternative<T>(value)) {
                return std::get<T>(value);
            }
            else {
                Log::Error("StyleValue: Could not get value wrong type T!");
                return std::nullopt;
            }
        }
    };

}