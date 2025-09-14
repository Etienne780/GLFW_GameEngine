#pragma once

#include <EngineLib\Script.h>

namespace EngineCore::Component {

	class Rainbow : public EngineCore::Script{
	public:
		Rainbow(GameObjectID gameObjectID);

		COMPONENT_TYPE_DEFINITION(Rainbow);

	private:
		/*
		* @brief Called every frame before Application Update.
		*/
		void Update() override;
		std::shared_ptr<Component::MeshRenderer> m_meshRenderer = nullptr;
	};

}