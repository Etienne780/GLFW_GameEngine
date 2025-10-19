#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/StyleAttribute.h"

namespace EngineCore::UI {

	StyleAttribute::StyleAttribute(std::string name, std::string desc, std::string faBack, std::vector<std::string> inputs, std::function<StyleValue(const StyleAttribute*, const std::string&)> p)
		: m_name(std::move(name)), m_description(std::move(desc)), m_fallbackStr(faBack), m_inputs(std::move(inputs)), m_parser(std::move(p)) {
	}

	StyleValue StyleAttribute::GetAttributeValue(const std::string& name, const std::string& input) {
		auto& att = GetAttribute(name);
		return att.GetValue(input);
	}

	StyleValue StyleAttribute::GetValue(const std::string& value) const {
		if (m_parser)
			return m_parser(this, value);

		Log::Error("StyleAttribute: Cannot get value of Attribute '{}', no parser defined!", m_name);
		return StyleValue{};
	}

	bool StyleAttribute::TryGetValue(const std::string& value, StyleValue& outValue) const {
		if (m_parser) {
			outValue = m_parser(this, value);
			return true;
		}
		return false;
	}

	std::unordered_map<std::string, std::string> StyleAttribute::ParseCompositeValues(const std::string& input) const {
		if (m_isComposite) {
			std::unordered_map<std::string, std::string> map;
			std::istringstream iss(input);
			std::vector<std::string> tokens;
			std::string token;
			while (iss >> token) tokens.push_back(token);

			// If less tokens than mapped attributes, fill the rest with last token
			if (tokens.size() < m_mappedAttributes.size() && !tokens.empty()) {
				tokens.resize(m_mappedAttributes.size(), tokens.back());
			}

			for (size_t i = 0; i < m_mappedAttributes.size(); ++i) {
				const std::string& attrName = m_mappedAttributes[i];
				std::string val;

				if (i < tokens.size()) {
					val = tokens[i];
				}
				else if (!tokens.empty()) {
					val = tokens.back();
				}
				else {
					val = "";
				}

				map[attrName] = val;
			}

			return map;
		}

		Log::Error("StyleAttribute: Could not parse Composite values, attribute {} is not a Composite!", m_name);
		return {};
	}

	const std::string& StyleAttribute::GetName() const { 
		return m_name; 
	}

	const std::string& StyleAttribute::GetDesc() const { 
		return m_description; 
	}

	const std::vector<std::string>& StyleAttribute::GetInputs() const { 
		return m_inputs;
	}

	std::string StyleAttribute::GetFallbackStr() const {
		return m_fallbackStr; 
	}

	bool StyleAttribute::IsComposite() const {
		return m_isComposite;
	}

	const std::vector<std::string>& StyleAttribute::GetMappedAttributes() const {
#ifndef NDEBUG
		if(!m_isComposite)
			Log::Warn("StyleAttribute: MappedAttributes for attribute '{}' will be empty, attribute is not a Composite!", m_name);
#endif 
		return m_mappedAttributes;
	}

	bool StyleAttribute::TryGetAttributeValue(const std::string& name, const std::string& input, StyleValue& outValue) {
		if (StyleAttribute att; TryGetAttribute(name, att)) {
			outValue = att.GetValue(input);
			return true;
		}
		return false;
	}

	const StyleAttribute& StyleAttribute::GetAttribute(const std::string& name) {
		if (m_registeredStyleAttributes.find(name) != m_registeredStyleAttributes.end()) {
			return m_registeredStyleAttributes.at(name);
		}

		Log::Error("StyleAttribute: Could not get Attribute with name '{}', this Attribute does not exist!", name);
		static StyleAttribute dummy;
		return dummy;
	}

	const bool StyleAttribute::TryGetAttribute(const std::string& name, StyleAttribute& outValue) {
		if (m_registeredStyleAttributes.find(name) != m_registeredStyleAttributes.end()) {
			outValue = m_registeredStyleAttributes.at(name);
			return true;
		}
		return false;
	}

	const std::unordered_map<std::string, StyleAttribute>& StyleAttribute::GetAllAttributes() {
		return m_registeredStyleAttributes;
	}

	void StyleAttribute::RegisterAttribute(const StyleAttribute& attribute) {
#ifndef NDEBUG
		if (m_registeredStyleAttributes.find(attribute.m_name) != m_registeredStyleAttributes.end()) {
			Log::Warn("StyleAttribute: Name of Attribute '{}' is not unique, will lead to problems getting Attribute!", 
				attribute.GetName());
		}
#endif
		m_registeredStyleAttributes[attribute.GetName()] = attribute;
	}

}