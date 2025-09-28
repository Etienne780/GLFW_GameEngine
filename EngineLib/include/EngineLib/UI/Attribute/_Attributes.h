#pragma once

#include "DisplayAttributes.h"
#include "LayoutAttributes.h"
#include "StyleAttributes.h"
#include "TextAttributes.h"

namespace EngineCore::UI::Init {

	inline bool InitAttributes() {
		return (regDisplayAtt() &&
			regLayoutAtt() &&
			regStyleAtt() &&
			regTextAtt());
	}

}