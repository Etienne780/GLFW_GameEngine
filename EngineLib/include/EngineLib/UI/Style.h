#pragma once
#include <any>
#include <unordered_map>
#include <string>

#include "StylingAttribute.h"
#include "../EngineTypes.h"

namespace EngineCore::UI {

	class Style {
	public:
		Style() = default;

		void Extend(const Style& style);
		void Set(const char* name, std::any value);

		template<typename T>
		T Get(const char* name) const {
			auto it = m_attributes.find(name);
			if (it != m_attributes.end()) {
				try {
					return std::any_cast<T>(it->second);
				}
				catch (const std::bad_any_cast&) {
					return ENGINE_STYLING_BAD_TYPE;
				}
			}

			return ENGINE_STYLING_NOT_FOUND;
		}

		const std::unordered_map<std::string, std::any>& GetAll() const;
	
	private:
		std::unordered_map<std::string, std::any> m_attributes;
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
