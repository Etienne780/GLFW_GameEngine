#pragma once

namespace EngineCore {

	class Component {
	public:
		virtual ~Component() = default;

	protected:
		Component() = default;
	};

}