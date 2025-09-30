#include <CoreLib/Log.h>
#include <CoreLib/ConversionUtils.h>
#include <CoreLib/FormatUtils.h>
#include <CoreLib/Math/Vector4.h>

#include "EngineLib/UI/Elements/Element.h"
#include "EngineLib/UI/AttributeHelper.h"

namespace EngineCore::UI {
	
    std::vector<std::string> AttributeHelper::GetValues(const std::string& input) {
        std::vector<std::string> values;
        size_t start = 0;

        for (size_t i = 0; i <= input.length(); ++i) {
            if (i == input.length() || input[i] == ' ') {
                if (i > start) {
                    values.push_back(input.substr(start, i - start));
                }
                start = i + 1;
            }
        }

        return values;
    }

	bool AttributeHelper::TryGetUnit(const std::string& input, std::string& out) {
		static const std::vector<std::string> units = { "px", "%w", "%h", "vh", "vw" };

		for (const auto& u : units) {
			if (input.size() >= u.size() &&
				input.compare(input.size() - u.size(), u.size(), u) == 0) {
				out = u;
				return true;
			}
		}

		out.clear();
		return false;
	}

	bool AttributeHelper::TryGetNumber(const ElementBase& element, const std::string& input, float& out) {
		std::string unit;
		if (TryGetUnit(input, unit)) {
			// get number
			std::string numberPart = input.substr(0, input.size() - unit.size());
			auto val = FormatUtils::stringToNumber<float>(numberPart);
			if (!val)
				return false;

			out = *val;
			// calculate in unit
			if (unit == "px") {
				// nothing, already in pixels
			}
			else if (unit == "%w") {
				out = element.GetParentWidth() * out / 100.0f;
			}
			else if (unit == "%h") {
				out = element.GetParentHeight() * out / 100.0f;
			}
			else if (unit == "vh") {
				out = element.GetViewportHeight() * out / 100.0f;
			}
			else if (unit == "vw") {
				out = element.GetViewportWidth() * out / 100.0f;
			}
			return true;
		}
		Log::Warn("StyleAttribute: Invalid unit in style '{}', input:'{}'", element.GetStyle()->GetName(), input);
		return false;
	}

	StyleAttribute AttributeHelper::MakeSimpleStringAttribute(
		const char* name,
		const char* description,
		std::vector<std::string> inputs,
		const std::string& defaultValue)
	{
		return StyleAttribute(name, description, inputs,
			[defaultValue](const ElementBase& element, const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				std::string s = FormatUtils::toLowerCase(val);
				if (AttributeHelper::ListContains(styleAtt->GetInputs(), s))
					return StyleValue(s);

				Log::Warn("StyleAttribute: {} could not calculate value in style '{}', input:'{}' invalid!", styleAtt->GetName(), element.GetStyle()->GetName(), val);
				return StyleValue(defaultValue);
			}
		);
	}

	StyleAttribute AttributeHelper::MakeSimpleNumberAttribute(
		const char* name,
		const char* description,
		float defaultValue)
	{
		return StyleAttribute(name, description, { "nummber" },
			[defaultValue](const ElementBase& element, const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				float f = 0;
				if (TryGetNumber(element, val, f))
					return StyleValue(f);

				Log::Warn("StyleAttribute: {} could not calculate value in style '{}', input:'{}' invalid!", styleAtt->GetName(), element.GetStyle()->GetName(), val);
				return StyleValue(defaultValue);
			}
		);
	}

	StyleAttribute AttributeHelper::MakeSimpleColorAttribute(
		const char* name,
		const char* description,
		std::vector<std::string> inputs,
		Vector4 defaultValue) 
	{
		return StyleAttribute(name, description,
			[defaultValue](const ElementBase& element, const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				bool errorType = false;
				std::string clearString = FormatUtils::removeSpaces(val);
				size_t pos = clearString.find('#');
				if (pos == std::string::npos) {
					Log::Warn("StyleAttribute: {} could not calculate value in style '{}', '#' is missing, input:'{}' invalid!", element.GetStyle()->GetName(), styleAtt->GetName(), val);
					return StyleValue(defaultValue);
				}
				pos++;
				size_t colorLen = clearString.size() - pos;
				std::string colorStr = clearString.substr(pos, colorLen);
				if (colorLen == 2) {
					errorType = true;
					int b = ConversionUtils::HexToIntegral(colorStr);
					if (b != -1)
						return StyleValue(Vector4(b / 255.0f, b / 255.0f, b / 255.0f, 1.0f));
				}
				else if (colorLen == 6) {
					errorType = true;
					int r = ConversionUtils::HexToIntegral(colorStr.substr(0, 2));
					int g = ConversionUtils::HexToIntegral(colorStr.substr(2, 2));
					int b = ConversionUtils::HexToIntegral(colorStr.substr(4, 2));
					if (r != -1 && g != -1 && b != -1)
						return StyleValue(Vector4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));
				}
				else if (colorLen == 8) {
					errorType = true;
					int r = ConversionUtils::HexToIntegral(colorStr.substr(0, 2));
					int g = ConversionUtils::HexToIntegral(colorStr.substr(2, 2));
					int b = ConversionUtils::HexToIntegral(colorStr.substr(4, 2));
					int a = ConversionUtils::HexToIntegral(colorStr.substr(6, 2));
					if (r != -1 && g != -1 && b != -1 && a != -1)
						return StyleValue(Vector4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));
				}

				if(errorType)
					Log::Warn("StyleAttribute: {}, invalid hex symbols in style '{}', input:'{}'!", styleAtt->GetName(), element.GetStyle()->GetName(), val);
				else 
					Log::Warn("StyleAttribute: {}, invalid color format in style '{}', expected hex string with 2, 6 or 8 digits after '#', input:'{}'!", styleAtt->GetName(), element.GetStyle()->GetName(), val);
				return StyleValue(defaultValue);
			}
		);
	}

}