#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/StyleAttribute.h"

namespace EngineCore::UI {

	StyleAttribute::StyleAttribute(std::string name, std::string desc, std::function<StyleValue(const ElementBase& element, const StyleAttribute*, const std::string&)> p)
		: m_name(std::move(name)), m_description(std::move(desc)), m_parser(std::move(p)) {
	}

	StyleAttribute::StyleAttribute(std::string name, std::string desc, std::vector<std::string> inputs, std::function<StyleValue(const ElementBase& element, const StyleAttribute*, const std::string&)> p)
		: m_name(std::move(name)), m_description(std::move(desc)), m_inputs(std::move(inputs)), m_parser(std::move(p)) {
	}

	StyleValue StyleAttribute::GetAttributeValue(const std::string& name, const ElementBase& element, const std::string& valueStr) {
		auto att = GetAttribute(name);
		return att.GetValue(element, valueStr);
	}

	const StyleAttribute& StyleAttribute::GetAttribute(const std::string& name) {
		if (m_registeredStyleAttributes.find(name) != m_registeredStyleAttributes.end()) {
			return m_registeredStyleAttributes.at(name);
		}

		Log::Error("StyleAttribute: Could not get Attribute with name '{}', this Attribute does not exist!", name);
		static StyleAttribute dummy;
		return dummy;
	}

	const std::unordered_map<std::string, StyleAttribute>& StyleAttribute::GetAllAttributes() {
		return m_registeredStyleAttributes;
	}

	void StyleAttribute::RegisterAttribute(const StyleAttribute& attribute) {
#ifndef NDEBUG
		if (m_registeredStyleAttributes.find(attribute.m_name) != m_registeredStyleAttributes.end()) {
			Log::Warn("StyleAttribute: Name of Attribute '{}' is not unique, will lead to problems getting Attribute!");
		}
#endif
		m_registeredStyleAttributes[attribute.GetName()] = attribute;
	}

}