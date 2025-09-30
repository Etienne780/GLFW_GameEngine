#include <CoreLib/Log.h>
#include <CoreLib/ConversionUtils.h>
#include <CoreLib/FormatUtils.h>
#include <CoreLib/Math/Vector2.h>
#include <CoreLib/Math/Vector3.h>
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


	bool AttributeHelper::IsUnit(const std::string& unit) {
		return (IsSizeUnit(unit) || IsTimeUnit(unit));
	}

	bool AttributeHelper::IsSizeUnit(const std::string& unit) {
		return (std::find(m_sizeUnits.begin(), m_sizeUnits.end(), unit) != m_sizeUnits.end());
	}

	bool AttributeHelper::IsTimeUnit(const std::string& unit) {
		return (std::find(m_timeUnits.begin(), m_timeUnits.end(), unit) != m_timeUnits.end());
	}

	bool AttributeHelper::IsUnitType(NumberType type, const std::string& unit) {
		return ((type == NumberType::SIZE && IsSizeUnit(unit)) || 
			(type == NumberType::TIME && IsTimeUnit(unit)));
	}

	const std::vector<std::string>& AttributeHelper::GetAllUnits() {
		static std::vector<std::string> units = [] {
			std::vector<std::string> u;
			u.reserve(m_sizeUnits.size() + m_timeUnits.size());
			u.insert(u.end(), m_sizeUnits.begin(), m_sizeUnits.end());
			u.insert(u.end(), m_timeUnits.begin(), m_timeUnits.end());
			return u;
		}();
		return units;
	}

	bool AttributeHelper::TryGetUnit(const std::string& input, std::string& out) {
		const auto& units = GetAllUnits();
		for (const auto& u : units) {
			if (input.size() >= u.size() &&
				input.rfind(u) == input.size() - u.size()) {
				out = u;
				return true;
			}
		}
		out.clear();
		return false;
	}

	bool AttributeHelper::TryGetNumber(const ElementBase& element, const std::string& input, float& outValue, std::string& outUnit) {
		if (TryGetUnit(input, outUnit)) {
			// get number
			std::string numberPart = input.substr(0, input.size() - outUnit.size());
			auto val = FormatUtils::stringToNumber<float>(numberPart);
			if (!val)
				return false;

			outValue = *val;
			// calculate in unit
			if (outUnit == "px") {
				// nothing, already in pixels
			}
			else if (outUnit == "%w") {
				outValue = element.GetParentWidth() * outValue / 100.0f;
			}
			else if (outUnit == "%h") {
				outValue = element.GetParentHeight() * outValue / 100.0f;
			}
			else if (outUnit == "vh") {
				outValue = element.GetViewportHeight() * outValue / 100.0f;
			}
			else if (outUnit == "vw") {
				outValue = element.GetViewportWidth() * outValue / 100.0f;
			}
			return true;
		}
		Log::Warn("StyleAttribute: Invalid unit in style '{}', input:'{}'", 
			element.GetStyle()->GetName(), input);
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

				Log::Warn("StyleAttribute: {} could not calculate value in style '{}', input:'{}' invalid!", 
					styleAtt->GetName(), element.GetStyle()->GetName(), val);
				return StyleValue(defaultValue);
			}
		);
	}

	StyleAttribute AttributeHelper::MakeSimpleNumberAttribute(
		const char* name,
		const char* description,
		float defaultValue,
		NumberType type)
	{
		return StyleAttribute(name, description, { "nummber" },
			[defaultValue, type](const ElementBase& element, const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				float f = 0;
				bool errorType = false;
				std::string unit;
				if (TryGetNumber(element, val, f, unit)) {
					errorType = true;
					if(IsUnitType(type, unit))
						return StyleValue(f);
				}
				if(errorType)
					Log::Warn("StyleAttribute: {} could not calculate value in style '{}', '{}' is not a valid unit, input:'{}' invalid!", 
						styleAtt->GetName(), element.GetStyle()->GetName(), unit, val);
				else
					Log::Warn("StyleAttribute: {} could not calculate value in style '{}', input:'{}' invalid!", 
						styleAtt->GetName(), element.GetStyle()->GetName(), val);
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
					Log::Warn("StyleAttribute: {} could not calculate value in style '{}', '#' is missing, input:'{}' invalid!", 
						element.GetStyle()->GetName(), styleAtt->GetName(), val);
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
					Log::Warn("StyleAttribute: {}, invalid hex symbols in style '{}', input:'{}'!", 
						styleAtt->GetName(), element.GetStyle()->GetName(), val);
				else 
					Log::Warn("StyleAttribute: {}, invalid color format in style '{}', expected hex string with 2, 6 or 8 digits after '#', input:'{}'!", 
						styleAtt->GetName(), element.GetStyle()->GetName(), val);
				return StyleValue(defaultValue);
			}
		);
	}

	StyleAttribute AttributeHelper::MakeMultiStringAttribute(
		const char* name,
		const char* description,
		std::vector<size_t> numberOfInputs,
		std::vector<std::string> inputs,
		std::vector<std::string> defaultValues)
	{
#ifndef NDEBUG
		if (numberOfInputs.size() == 1 && numberOfInputs[0] == 1)
			Log::Warn("AttributeHelper: Attribute '{}' is defined as MultiStringAttribute but only allows 1 input. Consider using SimpleStringAttribute instead.",
				name);

		if (!numberOfInputs.empty() && std::find(numberOfInputs.begin(), numberOfInputs.end(), defaultValues.size()) == numberOfInputs.end())
			Log::Warn("AttributeHelper: Attribute '{}' defaultValues count ({}) does not match allowed numberOfInputs ({})",
				name, defaultValues.size(), numberOfInputs.size());
#endif 

		return StyleAttribute(name, description,
			[numberOfInputs, defaultValues](const ElementBase& element, const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				auto values = AttributeHelper::GetValues(val);
				bool errorType = false;

				auto it = std::find(numberOfInputs.begin(), numberOfInputs.end(), values.size());
				if (!numberOfInputs.empty() && it != numberOfInputs.end()) {
					const auto& list = styleAtt->GetInputs();
					std::vector<StyleValue> parsed;
					parsed.reserve(values.size());

					for (auto& s : values) {
						std::string lowerS = FormatUtils::toLowerCase(s);
						if (AttributeHelper::ListContains(list, lowerS)) {
							parsed.emplace_back(lowerS);
						}
						else {
							errorType = true;
							break;
						}
					}

					if (!errorType)
						return StyleValue(parsed);
				}

				if (errorType)
					Log::Warn("StyleAttribute: '{}' could not calculate value in style '{}', invalid token in input '{}'",
						styleAtt->GetName(), element.GetStyle()->GetName(), val);
				else
					Log::Warn("StyleAttribute: '{}' could not calculate value in style '{}', invalid argument count, input '{}'",
						styleAtt->GetName(), element.GetStyle()->GetName(), val);

				// fallback to defaults
				std::vector<StyleValue> dV;
				dV.reserve(defaultValues.size());
				for (auto& s : defaultValues) {
					std::string lowerS = FormatUtils::toLowerCase(s);
					dV.emplace_back(lowerS);
				}

				return StyleValue(dV);
			}
		);
	}

	StyleAttribute AttributeHelper::MakeMultiNumberAttribute(
		const char* name,
		const char* description,
		size_t maxInput,
		std::vector<float> defaultValue,
		NumberType type) 
	{
#ifndef NDEBUG
		if (maxInput == 1)
			Log::Warn("AttributeHelper: Attribute '{}' is defined as MultiNumberAttribute but only allows 1 input. Consider using SimpleNumberAttribute instead.",
				name);

		if(maxInput != defaultValue.size())
			Log::Warn("AttributeHelper: Attribute '{}' defaultValues({}) count does not match allowed numberOfInputs({})",
				name, defaultValue.size(), maxInput);
#endif 

		return StyleAttribute(name, description, { "nummber" },
			[maxInput, defaultValue, type](const ElementBase& element, const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				short errorType = 0;
				auto values = GetValues(val);
				
				
				if (maxInput < values.size()) {
					switch (maxInput)
					{
					case 1: {
						if (values.size() > 1)
							break;

						errorType = 1;
						float v;
						std::string unit;
						if (TryGetNumber(element, values[0], v, unit)) {
							if (IsUnitType(type, unit)) {
								return StyleValue(v);
							}
							errorType = 2;
						}

						break;
					}
					case 2: {
						if (values.size() > 2)
							break;

						errorType = 1;
						if (values.size() == 1) {
							float v;
							std::string unit;
							if (TryGetNumber(element, values[0], v, unit)) {
								if (IsUnitType(type, unit)) {
									return StyleValue(Vector2(v, v));
								}
								errorType = 2;
							}
						}
						else {
							float v1, v2;
							std::string unit1, unit2;
							if (TryGetNumber(element, values[0], v1, unit1) &&
								TryGetNumber(element, values[1], v2, unit2)) {
								if (IsUnitType(type, unit1, unit2)) {
									return StyleValue(Vector2(v1, v2));
								}
								errorType = 2;
							}
						}

						break;
					}
					case 3: {
						if (values.size() > 3)
							break;
						// alg for filling empty slots looks asss
						errorType = 1;
						if (values.size() == 1) {
							float v;
							std::string unit;
							if (TryGetNumber(element, values[0], v, unit)) {
								if (IsUnitType(type, unit)) {
									return StyleValue(Vector3(v, v, v));
								}
								errorType = 2;
							}
						}
						else if (values.size() == 2) {
							float v1, v2;
							std::string unit1, unit2;
							if (TryGetNumber(element, values[0], v1, unit1) &&
								TryGetNumber(element, values[1], v2, unit2)) {
								if (IsUnitType(type, unit1, unit2)) {
									return StyleValue(Vector3(v1, v2, v2));
								}
								errorType = 2;
							}
						}
						else {
							float v1, v2, v3;
							std::string unit1, unit2, unit3;
							if (TryGetNumber(element, values[0], v1, unit1) &&
								TryGetNumber(element, values[1], v2, unit2) &&
								TryGetNumber(element, values[2], v3, unit3)) {
								if (IsUnitType(type, unit1, unit2, unit3)) {
									return StyleValue(Vector3(v1, v2, v3));
								}
								errorType = 2;
							}
						}

						break;
					}
					case 4: {
						if (values.size() > 4)
							break;

						errorType = 1;
						if (values.size() == 1) {
							float v;
							std::string unit;
							if (TryGetNumber(element, values[0], v, unit)) {
								if (IsUnitType(type, unit)) {
									return StyleValue(Vector4(v, v, v, v));
								}
								errorType = 2;
							}
						}
						else if (values.size() == 2) {
							float v1, v2;
							std::string unit1, unit2;
							if (TryGetNumber(element, values[0], v1, unit1) &&
								TryGetNumber(element, values[1], v2, unit2)) {
								if (IsUnitType(type, unit1, unit2)) {
									return StyleValue(Vector4(v1, v1, v2, v2));
								}
								errorType = 2;
							}
						}
						else if (values.size() == 3) {
							float v1, v2, v3;
							std::string unit1, unit2, unit3;
							if (TryGetNumber(element, values[0], v1, unit1) &&
								TryGetNumber(element, values[1], v2, unit2) &&
								TryGetNumber(element, values[2], v3, unit3)) {
								if (IsUnitType(type, unit1, unit2, unit3)) {
									return StyleValue(Vector4(v1, v2, v3, v3));
								}
								errorType = 2;
							}
						}
						else {
							float v1, v2, v3, v4;
							std::string unit1, unit2, unit3, unit4;
							if (TryGetNumber(element, values[0], v1, unit1) &&
								TryGetNumber(element, values[1], v2, unit2) &&
								TryGetNumber(element, values[2], v3, unit3) &&
								TryGetNumber(element, values[2], v4, unit4)) {
								if (IsUnitType(type, unit1, unit2, unit3, unit4)) {
									return StyleValue(Vector4(v1, v2, v3, v4));
								}
								errorType = 2;
							}
						}
						break;
					}
					}
				}

				if (errorType == 2)
					Log::Warn("StyleAttribute: {} could not calculate value in style '{}', invalid unit, input:'{}'!", 
						styleAtt->GetName(), element.GetStyle()->GetName(), val);
				else if (errorType == 1)
					Log::Warn("StyleAttribute: {} could not calculate value in style '{}', invalid number, input:'{}'!", 
						styleAtt->GetName(), element.GetStyle()->GetName(), val);
				else
					Log::Warn("StyleAttribute: {} could not calculate value in style '{}', invalid argument count max arguments '{}', input:'{}'!", 
						styleAtt->GetName(), element.GetStyle()->GetName(), maxInput, val);

				// cal default value
				switch (maxInput)
				{
				case 1:
					return StyleValue((defaultValue.empty()) ? 0.0f : defaultValue[0]);
				case 2: {
					Vector2 vec;
					if (defaultValue.size() == 1) {
						vec.Set(defaultValue[0], defaultValue[0]);
					}
					else if (defaultValue.size() == 2) {
						vec.Set(defaultValue[0], defaultValue[1]);
					}
					return StyleValue(vec);
				}
				case 3: {
					Vector3 vec;
					if (defaultValue.size() == 1) {
						vec.Set(defaultValue[0], defaultValue[0], defaultValue[0]);
					}
					else if (defaultValue.size() == 2) {
						vec.Set(defaultValue[0], defaultValue[1], defaultValue[1]);
					}
					else if (defaultValue.size() == 3) {
						vec.Set(defaultValue[0], defaultValue[1], defaultValue[2]);
					}
					return StyleValue(vec);
				}
				case 4: {
					Vector4 vec;
					if (defaultValue.size() == 1) {
						vec.Set(defaultValue[0], defaultValue[0], defaultValue[0], defaultValue[0]);
					}
					else if (defaultValue.size() == 2) {
						vec.Set(defaultValue[0], defaultValue[0], defaultValue[1], defaultValue[1]);
					}
					else if (defaultValue.size() == 3) {
						vec.Set(defaultValue[0], defaultValue[1], defaultValue[2], defaultValue[2]);
					}
					else if (defaultValue.size() == 4) {
						vec.Set(defaultValue[0], defaultValue[1], defaultValue[2], defaultValue[3]);
					}
					return StyleValue(vec);
				}
				default:
					Log::Error("AttributeHelper: Attribute '{}' has the invalid maxInput({}), maxInput needs to be 1, 2, 3 or 4!",
						styleAtt->GetName(), maxInput);
					return StyleValue(0.0f);
				}
			}
		);
	}

}