#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <variant>
#include <CoreLib/Log.h>
#include <CoreLib/Math/Vector3.h>
#include <CoreLib/Math/Vector4.h>

#include "StylingAttribute.h"
#include "../EngineTypes.h"

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

	class Style {
	public:
		Style() = default;

        static std::shared_ptr<Style> Create();
        static std::shared_ptr<Style> Create(std::shared_ptr<Style> style);

		void Extend(std::shared_ptr<Style> style);
		void Set(const char* name, const StyleValue& value);
        void Set(State state, const char* name, const StyleValue& value);

        StyleValue Get(const char* name) const;
        StyleValue Get(State state, const char* name) const;
		
		const std::unordered_map<State, std::unordered_map<std::string, StyleValue>>& GetAll() const;
        const std::unordered_map<std::string, StyleValue>& GetAllState(State state) const;
	
	private:
        std::unordered_map<State, std::unordered_map<std::string, StyleValue>> m_attributes;
		std::vector<std::shared_ptr<Style>> m_styles;
		mutable bool m_newStyleAdded = true;
		mutable std::unique_ptr<Style> m_cachedStyle;

		void GenerateCachedStyle() const;
	};

}

/*
styling attribute:
- layout von childs =  horizontal start, center, end, stretch vertical start, center, end, stretch
- Position = x,y
- rotation = z
- overflow = hidden, unset
- color = hex
- radius = 2
- custome styling attribute
*/
 