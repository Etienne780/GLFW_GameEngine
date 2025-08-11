#pragma once
#include <string>
#include <unordered_map>

#include "EngineTypes.h"

namespace EngineCore {

	class Scene {
	public:
		static Scene* Create(const std::string& name);
		static Scene* Get(const std::string& name);
		static Scene* Get(unsigned int id);

		unsigned int GetID() const;
		const std::string& GetName() const;

	private:
		Scene(unsigned int id, const std::string& name);

		unsigned int m_id = ENGINE_INVALID_ID;
		std::string m_name;

		static unsigned int m_idCounter;
		static std::unordered_map<unsigned int, Scene> m_scenes;

		static unsigned int GetNewUniqueIdentifier();
	};

}