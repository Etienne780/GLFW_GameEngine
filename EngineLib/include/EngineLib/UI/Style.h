#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <CoreLib/Log.h>

#include "UITypes.h"
#include "StyleAttribute.h"
#include "../EngineTypes.h"

namespace EngineCore::UI {

	class Style {
	public:
		Style() = default;
		Style(const std::string& name);

        static std::shared_ptr<Style> Create(const std::string& name);
        static std::shared_ptr<Style> Create(const std::string& name, std::shared_ptr<Style> style);

		void Extend(std::shared_ptr<Style> style);
		void Set(const char* name, std::string value);
		void Set(State state, const char* name, std::string value);

        std::string Get(const char* name) const;
		std::string Get(State state, const char* name) const;
		
        const std::string& GetName() const;
		const std::unordered_map<State, std::unordered_map<std::string, std::string>>& GetAll() const;
        const std::unordered_map<std::string, std::string>& GetAllState(State state) const;
	    
	private:
        std::string m_name = "UNKNOWN";
        std::unordered_map<State, std::unordered_map<std::string, std::string>> m_attributes;// state, attName, attValue
		std::vector<std::shared_ptr<Style>> m_extendedStyles;
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
 