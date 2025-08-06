#pragma once
#include <string>
#include <vector>
#include <memory>

#include "GameObject.h"

namespace EngineCore {

	class GameObjectManager {
	friend class GameObject;
	private:
		GameObjectManager();

		static unsigned int m_idCounter;
		static std::vector<std::unique_ptr<GameObject>> m_gameObjects;

		static void AddGameObject(GameObject* gameObjectPtr);
		static bool DeleteGameObject(GameObject* gameObjectPtr);
		static bool DeleteGameObject(unsigned int id);
		static bool DeleteGameObject(const std::string& name);

		static unsigned int GetNewUniqueIdentifier();
		static GameObject* GetGameObject(unsigned int id);
		static GameObject* GetGameObject(const std::string& name);
		static std::vector<GameObject*> GetAllGameObjects();

		static bool IsNameUnique(const std::string& name);
	};

}