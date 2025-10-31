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

	bool AttributeHelper::TryGetUnit(const std::string& input, std::string& out) {
		for (const auto& u : m_units) {
			if (input.size() >= u.size() &&
				input.rfind(u) == input.size() - u.size()) {
				out = u;
				return true;
			}
		}
		out.clear();
		return false;
	}

	bool AttributeHelper::TryGetNumber(const std::string& input, float& outValue) {
		std::string out;
		return TryGetNumber(input, outValue, out);
	}

	bool AttributeHelper::TryGetNumber(const std::string& input, float& outValue, std::string& outUnit) {
		if (TryGetUnit(input, outUnit)) {
			// get number
			std::string numberPart = input.substr(0, input.size() - outUnit.size());
			auto val = FormatUtils::stringToNumber<float>(numberPart);
			if (!val)
				return false;

			outValue = *val;
			return true;
		}
		return false;
	}

	StyleAttribute AttributeHelper::MakeSimpleStringAttribute(
		const char* name,
		const char* description,
		std::vector<std::string> inputs,
		const std::string& defaultValue)
	{
		return StyleAttribute(name, description, defaultValue, inputs,
			[](const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				std::string s = FormatUtils::toLowerCase(val);
				if (AttributeHelper::ListContains(styleAtt->GetInputs(), s))
					return StyleValue(s);

				Log::Warn("StyleAttribute: {} could not calculate value, input:'{}' invalid!", 
					styleAtt->GetName(), val);
				return StyleValue(styleAtt->GetFallbackStr());
			}
		);
	}


	StyleAttribute AttributeHelper::MakeIndexedAttribute(
		const char* name,
		const char* description,
		std::vector<std::string> inputs,
		unsigned int defaultValue)
	{
		return StyleAttribute(name, description, inputs[defaultValue], inputs,
			[](const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				std::string s = FormatUtils::toLowerCase(val);
				if (size_t index; AttributeHelper::ListContains(styleAtt->GetInputs(), s, index))
					return StyleValue(static_cast<int>(index), StyleUnit::Unit::Unknown);

				Log::Warn("StyleAttribute: {} could not calculate value, input:'{}' invalid!",
					styleAtt->GetName(), val);
				return StyleValue(styleAtt->GetFallbackStr());
			}
		);
	}

	StyleAttribute AttributeHelper::MakeSimpleNumberAttribute(
		const char* name,
		const char* description,
		const std::string& defaultValue,
		NumberType type)
	{
		return StyleAttribute(name, description, defaultValue, {"nummber"},
			[type](const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				float f = 0;
				bool errorType = false;
				std::string unit;
				if (TryGetNumber(val, f, unit)) {
					errorType = true;
					if(IsUnitType(type, unit))
						return StyleValue(f, StyleUnit::ToUnit(unit));
				}
				if(errorType)
					Log::Warn("AttributeHelper: {} could not calculate value, '{}' is not a valid unit, input:'{}' invalid!", 
						styleAtt->GetName(), unit, val);
				else
					Log::Warn("AttributeHelper: {} could not calculate value, input:'{}' invalid!", 
						styleAtt->GetName(), val);

				if (!TryGetNumber(styleAtt->GetFallbackStr(), f, unit)) {
					Log::Error("AttributeHelper: {} could not parse default value '{}' to number!",
						styleAtt->GetName(), styleAtt->GetFallbackStr());
				}

				return StyleValue(f, StyleUnit::ToUnit(unit));
			}
		);
	}

	StyleAttribute AttributeHelper::MakeSimpleColorAttribute(
		const char* name,
		const char* description,
		std::vector<std::string> inputs,
		const std::string& defaultValue)
	{
		// Default unit is always unknown because colors are not numeric units
		static std::vector<StyleUnit::Unit> defaultUnits{
			StyleUnit::Unit::Unknown,
			StyleUnit::Unit::Unknown,
			StyleUnit::Unit::Unknown,
			StyleUnit::Unit::Unknown
		};

		// Lambda that parses a hex string into a Vector4 color (RGBA)
		auto parseHexColor = [&](const std::string& input, Vector4& outColor) -> bool {
			std::string clearString = FormatUtils::removeSpaces(input);
			size_t pos = clearString.find('#');
			if (pos == std::string::npos)
				return false;

			std::string colorStr = clearString.substr(pos + 1);
			size_t colorLen = colorStr.size();

			auto hexToF = [](const std::string& s) -> float {
				int v = ConversionUtils::HexToIntegral(s);
				return (v == -1) ? -1.0f : v / 255.0f;
				};

			if (colorLen == 2) {
				float g = hexToF(colorStr);
				if (g >= 0.0f) {
					outColor = Vector4(g, g, g, 1.0f);
					return true;
				}
			}
			else if (colorLen == 6) {
				float r = hexToF(colorStr.substr(0, 2));
				float g = hexToF(colorStr.substr(2, 2));
				float b = hexToF(colorStr.substr(4, 2));
				if (r >= 0.0f && g >= 0.0f && b >= 0.0f) {
					outColor = Vector4(r, g, b, 1.0f);
					return true;
				}
			}
			else if (colorLen == 8) {
				float r = hexToF(colorStr.substr(0, 2));
				float g = hexToF(colorStr.substr(2, 2));
				float b = hexToF(colorStr.substr(4, 2));
				float a = hexToF(colorStr.substr(6, 2));
				if (r >= 0.0f && g >= 0.0f && b >= 0.0f && a >= 0.0f) {
					outColor = Vector4(r, g, b, a);
					return true;
				}
			}

			return false;
			};

		// Pre-parse the fallback value once so we don't repeat that in the lambda
		Vector4 fallbackColor(0.0f);
		if (!parseHexColor(defaultValue, fallbackColor)) {
			Log::Error("AttributeHelper: '{}' has invalid default color '{}'. Using transparent black.", name, defaultValue);
			fallbackColor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		return StyleAttribute(name, description, defaultValue, inputs,
			[fallbackColor, parseHexColor](const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				Vector4 color;
				if (parseHexColor(val, color)) {
					return StyleValue(color, {
						StyleUnit::Unit::Unknown,
						StyleUnit::Unit::Unknown,
						StyleUnit::Unit::Unknown,
						StyleUnit::Unit::Unknown
						});
				}

				Log::Warn("AttributeHelper: {} could not parse color, using fallback '{}'.",
					styleAtt->GetName(), styleAtt->GetFallbackStr());

				return StyleValue(fallbackColor, {
					StyleUnit::Unit::Unknown,
					StyleUnit::Unit::Unknown,
					StyleUnit::Unit::Unknown,
					StyleUnit::Unit::Unknown
					});
			}
		);
	}

	StyleAttribute AttributeHelper::MakeMultiStringAttribute(
		const char* name,
		const char* description,
		std::vector<size_t> numberOfInputs,
		std::vector<std::string> inputs,
		const std::string& defaultValues)
	{
#ifndef NDEBUG
		if (numberOfInputs.size() == 1 && numberOfInputs[0] == 1)
			Log::Warn("AttributeHelper: Attribute '{}' is defined as MultiStringAttribute but only allows 1 input. Consider using SimpleStringAttribute instead.",
				name);

		auto defautlVa = AttributeHelper::GetValues(defaultValues);
		if (!numberOfInputs.empty() && std::find(numberOfInputs.begin(), numberOfInputs.end(), defautlVa.size()) == numberOfInputs.end())
			Log::Warn("AttributeHelper: Attribute '{}' defaultValue '{}' count ({}) does not match allowed numberOfInputs ({})",
				name, defaultValues, defautlVa.size(), numberOfInputs.size());
#endif 
		return StyleAttribute(name, description, defaultValues, inputs,
			[numberOfInputs](const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
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
					Log::Warn("AttributeHelper: '{}' could not calculate value, invalid token in input '{}'",
						styleAtt->GetName(), val);
				else
					Log::Warn("AttributeHelper: '{}' could not calculate value, invalid argument count, input '{}'",
						styleAtt->GetName(), val);

				// creats the default values from the default string
				std::vector<std::string> fallbackValues =
					AttributeHelper::GetValues(styleAtt->GetFallbackStr());
				std::vector<StyleValue> dV;
				dV.reserve(fallbackValues.size());
				for (auto& s : fallbackValues) {
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
		const std::string& defaultValue,
		NumberType type) 
	{
#ifndef NDEBUG
		if (maxInput == 1)
			Log::Warn("AttributeHelper: Attribute '{}' is defined as MultiNumberAttribute but only allows 1 input. Consider using SimpleNumberAttribute instead.",
				name);

		auto defautlVa = AttributeHelper::GetValues(defaultValue);
		if(maxInput != defautlVa.size())
			Log::Warn("AttributeHelper: Attribute '{}' defaultValue '{}' count ({}) does not match allowed numberOfInputs({})",
				name, defaultValue, defaultValue.size(), maxInput);
#endif 

		return StyleAttribute(name, description, defaultValue, { "nummber" },
			[maxInput, type](const StyleAttribute* styleAtt, const std::string& val) -> StyleValue {
				short errorType = 0;
				auto values = GetValues(val);
				
				if (values.size() <= maxInput && !values.empty()) {
					// alg for filling empty slots looks asss
					switch (maxInput)
					{
					case 1: {
						if (values.size() > 1)
							break;

						errorType = 1;
						float v;
						std::string unit;
						if (TryGetNumber(values[0], v, unit)) {
							if (IsUnitType(type, unit)) {
								return StyleValue(v, StyleUnit::ToUnit(unit));
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
							if (TryGetNumber(values[0], v, unit)) {
								if (IsUnitType(type, unit)) {
									StyleUnit::Unit u = StyleUnit::ToUnit(unit);
									return StyleValue(Vector2(v, v), { u, u});
								}
								errorType = 2;
							}
						}
						else {
							float v1, v2;
							std::string unit1, unit2;
							if (TryGetNumber(values[0], v1, unit1) &&
								TryGetNumber(values[1], v2, unit2)) {
								if (IsUnitType(type, unit1, unit2)) {
									std::vector<StyleUnit::Unit> styleUnits{
										StyleUnit::ToUnit(unit1),
										StyleUnit::ToUnit(unit2)
									};
									return StyleValue(Vector2(v1, v2), styleUnits);
								}
								errorType = 2;
							}
						}

						break;
					}
					case 3: {
						if (values.size() > 3)
							break;
						errorType = 1;
						if (values.size() == 1) {
							float v;
							std::string unit;
							if (TryGetNumber(values[0], v, unit)) {
								if (IsUnitType(type, unit)) {
									StyleUnit::Unit u = StyleUnit::ToUnit(unit);
									return StyleValue(Vector3(v, v, v), { u, u, u});
								}
								errorType = 2;
							}
						}
						else if (values.size() == 2) {
							float v1, v2;
							std::string unit1, unit2;
							if (TryGetNumber(values[0], v1, unit1) &&
								TryGetNumber(values[1], v2, unit2)) {
								if (IsUnitType(type, unit1, unit2)) {
									StyleUnit::Unit u2 = StyleUnit::ToUnit(unit2);
									std::vector<StyleUnit::Unit> styleUnits{
										StyleUnit::ToUnit(unit1),
										u2,
										u2
									};
									return StyleValue(Vector3(v1, v2, v2), styleUnits);
								}
								errorType = 2;
							}
						}
						else {
							float v1, v2, v3;
							std::string unit1, unit2, unit3;
							if (TryGetNumber(values[0], v1, unit1) &&
								TryGetNumber(values[1], v2, unit2) &&
								TryGetNumber(values[2], v3, unit3)) {
								if (IsUnitType(type, unit1, unit2, unit3)) {
									std::vector<StyleUnit::Unit> styleUnits{
										StyleUnit::ToUnit(unit1),
										StyleUnit::ToUnit(unit2),
										StyleUnit::ToUnit(unit3)
									};
									return StyleValue(Vector3(v1, v2, v3), styleUnits);
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
							if (TryGetNumber(values[0], v, unit)) {
								if (IsUnitType(type, unit)) {
									StyleUnit::Unit u = StyleUnit::ToUnit(unit);
									return StyleValue(Vector4(v, v, v, v), { u, u, u, u });
								}
								errorType = 2;
							}
						}
						else if (values.size() == 2) {
							float v1, v2;
							std::string unit1, unit2;
							if (TryGetNumber(values[0], v1, unit1) &&
								TryGetNumber(values[1], v2, unit2)) {
								if (IsUnitType(type, unit1, unit2)) {
									StyleUnit::Unit u1 = StyleUnit::ToUnit(unit1);
									StyleUnit::Unit u2 = StyleUnit::ToUnit(unit2);
									return StyleValue(Vector4(v1, v1, v2, v2), { u1, u1, u2, u2 });
								}
								errorType = 2;
							}
						}
						else if (values.size() == 3) {
							float v1, v2, v3;
							std::string unit1, unit2, unit3;
							if (TryGetNumber(values[0], v1, unit1) &&
								TryGetNumber(values[1], v2, unit2) &&
								TryGetNumber(values[2], v3, unit3)) {
								if (IsUnitType(type, unit1, unit2, unit3)) {
									StyleUnit::Unit u3 = StyleUnit::ToUnit(unit3);
									std::vector<StyleUnit::Unit> styleUnits{
										StyleUnit::ToUnit(unit1),
										StyleUnit::ToUnit(unit2),
										u3,
										u3
									};
									return StyleValue(Vector4(v1, v2, v3, v3), styleUnits);
								}
								errorType = 2;
							}
						}
						else {
							float v1, v2, v3, v4;
							std::string unit1, unit2, unit3, unit4;
							if (TryGetNumber(values[0], v1, unit1) &&
								TryGetNumber(values[1], v2, unit2) &&
								TryGetNumber(values[2], v3, unit3) &&
								TryGetNumber(values[3], v4, unit4)) {
								if (IsUnitType(type, unit1, unit2, unit3, unit4)) {
									std::vector<StyleUnit::Unit> styleUnits{
										StyleUnit::ToUnit(unit1),
										StyleUnit::ToUnit(unit2),
										StyleUnit::ToUnit(unit3),
										StyleUnit::ToUnit(unit4)
									};
									return StyleValue(Vector4(v1, v2, v3, v4), styleUnits);
								}
								errorType = 2;
							}
						}
						break;
					}
					}
				}

				if (errorType == 2)
					Log::Warn("AttributeHelper: {} could not calculate value, invalid unit, input:'{}'!", 
						styleAtt->GetName(), val);
				else if (errorType == 1)
					Log::Warn("AttributeHelper: {} could not calculate value, invalid number, input:'{}'!", 
						styleAtt->GetName(), val);
				else
					Log::Warn("AttributeHelper: {} could not calculate value, invalid argument count max arguments '{}', input:'{}'!", 
						styleAtt->GetName(), maxInput, val);

				// translates default value str to float vector
				auto defaultValues = AttributeHelper::GetValues(styleAtt->GetFallbackStr());
				std::vector<float> defaultValueNum;
				std::vector<StyleUnit::Unit> defaultUnits;
				defaultValueNum.reserve(defaultValues.size());
				defaultUnits.reserve(defaultValues.size());
				float tempNum = 0;
				std::string tempUnit;
				for (auto& str : defaultValues) {
					if (!AttributeHelper::TryGetNumber(str, tempNum, tempUnit)) {
						Log::Error("AttributeHelper: {} could not convert part '{}', of default value '{}', to number!",
							styleAtt->GetName(), str, styleAtt->GetFallbackStr());
					}
					defaultValueNum.push_back(tempNum);
					defaultUnits.push_back(StyleUnit::ToUnit(tempUnit));
				}

				StyleValue fallback;
				// calculate default value
				switch (maxInput)
				{
				case 1:
					fallback = StyleValue((defaultValueNum.empty()) ? 0.0f : defaultValueNum[0], defaultUnits[0]);
					break;
				case 2: {
					Vector2 vec;
					std::vector<StyleUnit::Unit> units;
					units.reserve(2);
					if (defaultValueNum.size() == 1) {
						vec.Set(defaultValueNum[0], defaultValueNum[0]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[0]);
					}
					else if (defaultValueNum.size() == 2) {
						vec.Set(defaultValueNum[0], defaultValueNum[1]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[1]);
					}
					fallback = StyleValue(vec, units);
					break;
				}
				case 3: {
					Vector3 vec;
					std::vector<StyleUnit::Unit> units;
					units.reserve(3);
					if (defaultValueNum.size() == 1) {
						vec.Set(defaultValueNum[0], defaultValueNum[0], defaultValueNum[0]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[0]);
					}
					else if (defaultValueNum.size() == 2) {
						vec.Set(defaultValueNum[0], defaultValueNum[1], defaultValueNum[1]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[1]);
						units.push_back(defaultUnits[1]);
					}
					else if (defaultValueNum.size() == 3) {
						vec.Set(defaultValueNum[0], defaultValueNum[1], defaultValueNum[2]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[1]);
						units.push_back(defaultUnits[2]);
					}
					fallback = StyleValue(vec, units);
					break;
				}
				case 4: {
					Vector4 vec;
					std::vector<StyleUnit::Unit> units;
					units.reserve(4);
					if (defaultValueNum.size() == 1) {
						vec.Set(defaultValueNum[0], defaultValueNum[0], defaultValueNum[0], defaultValueNum[0]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[0]);
					}
					else if (defaultValueNum.size() == 2) {
						vec.Set(defaultValueNum[0], defaultValueNum[0], defaultValueNum[1], defaultValueNum[1]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[1]);
						units.push_back(defaultUnits[1]);
					}
					else if (defaultValueNum.size() == 3) {
						vec.Set(defaultValueNum[0], defaultValueNum[1], defaultValueNum[2], defaultValueNum[2]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[1]);
						units.push_back(defaultUnits[2]);
						units.push_back(defaultUnits[2]);
					}
					else if (defaultValueNum.size() == 4) {
						vec.Set(defaultValueNum[0], defaultValueNum[1], defaultValueNum[2], defaultValueNum[3]);
						units.push_back(defaultUnits[0]);
						units.push_back(defaultUnits[1]);
						units.push_back(defaultUnits[2]);
						units.push_back(defaultUnits[3]);
					}
					fallback = StyleValue(vec, units);
					break;
				}
				default:
					Log::Error("AttributeHelper: Attribute '{}' has the invalid maxInput({}), maxInput needs to be 1, 2, 3 or 4!",
						styleAtt->GetName(), maxInput);
					fallback = StyleValue(0.0f, StyleUnit::Unit::PX);
					break;
				}

				return fallback;
			}
		);
	}

}