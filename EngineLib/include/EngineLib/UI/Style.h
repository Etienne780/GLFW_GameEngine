#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <variant>
#include <CoreLib/Math/Vector3.h>
#include <CoreLib/Math/Vector4.h>

#include "StylingAttribute.h"
#include "../EngineTypes.h"

namespace EngineCore::UI {
    
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
    };

	class Style {
	public:
		Style() = default;

        static std::shared_ptr<Style> Create();
        static std::shared_ptr<Style> Create(std::shared_ptr<Style> style);

		void Extend(std::shared_ptr<Style> style);
		void Set(const char* name, const StyleValue& value);

        template<typename T>
        std::optional<T> Get(const std::string& name) const {
            if (m_newStyleAdded) {
                m_newStyleAdded = false;
                GenerateCachedStyle();
            }
            const auto& att = m_cachedStyle->m_attributes;
            auto it = att.find(name);
            if (it != att.end()) {
                if (std::holds_alternative<T>(it->second.value)) {
                    return std::get<T>(it->second.value);
                }
            }
            return std::nullopt;
        }
		
		const std::unordered_map<std::string, StyleValue>& GetAll() const;
	
	private:
        std::unordered_map<std::string, StyleValue> m_attributes;
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
 