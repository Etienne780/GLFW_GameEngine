#pragma once

#include "DisplayAttributes.h"
#include "LayoutAttributes.h"
#include "StyleAttributes.h"

namespace EngineCore::UI::Init {

	inline bool InitAttributes() {
		return (regDisplayAtt() &&
			regLayoutAtt() &&
			regStyleAtt());
	}

}