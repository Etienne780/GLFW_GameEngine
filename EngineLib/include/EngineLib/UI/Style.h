#pragma once
#include <any>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

#include "StylingAttribute.h"
#include "../EngineTypes.h"

namespace EngineCore::UI {

	class Style {
	public:
		Style() = default;

		void Extend(std::shared_ptr<Style> style);
		void Set(const char* name, std::any value);

		template<typename T>
		T Get(const char* name) const {
			if (m_newStyleAdded) {
				m_newStyleAdded = false;
				GenerateCachedStyle();
			}
			const auto& att = m_cachedStyle->GetAll();
			auto it = att.find(name);
			if (it != att.end()) {
				try {
					return std::any_cast<T>(it->second);
				}
				catch (const std::bad_any_cast&) {
					return ENGINE_STYLING_BAD_TYPE;
				}
			}

			return ENGINE_STYLING_NOT_FOUND;
		}
		
		const std::unordered_map<const char*, std::any>& GetAll() const;
	
	private:
		std::unordered_map<const char*, std::any> m_attributes;
		std::vector<std::shared_ptr<Style>> m_styles;
		mutable bool m_newStyleAdded = true;
		mutable std::unique_ptr<Style> m_cachedStyle;

		void GenerateCachedStyle();
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
 