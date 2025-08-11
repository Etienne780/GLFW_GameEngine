#include "CoreLib\Log.h"
#include "EngineLib\Scene.h"

namespace EngineCore {
	
	unsigned int Scene::m_idCounter = 0;
	std::unordered_map<unsigned int, Scene> Scene::m_scenes;

	Scene::Scene(unsigned int id, const std::string& name)
		: m_id(id), m_name(name) {
	}

	#pragma region Static

	Scene* Scene::Create(const std::string& name) {
		unsigned int newID = GetNewUniqueIdentifier();
		auto [it, inserted] = m_scenes.emplace(newID, Scene(newID, name));
		return &(it->second);
	}

	Scene* Scene::Get(const std::string& name) {
		for (auto& [sceneID, scene] : m_scenes) {
			if (scene.GetName() == name) {
				return &scene;
			}
		}
		Log::Warn("Scene: Scene '{}' not found", name);
		return nullptr;
	}

	Scene* Scene::Get(unsigned int id) {
		return &m_scenes[id];
	}

	#pragma endregion
	
	unsigned int Scene::GetID() const {
		return m_id;
	}

	const std::string& Scene::GetName() const {
		return m_name;
	}

	unsigned int Scene::GetNewUniqueIdentifier() {
		return m_idCounter++;
	}

}