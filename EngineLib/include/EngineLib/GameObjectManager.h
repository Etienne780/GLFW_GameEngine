#pragma once
#include <string>
#include <vector>
#include <memory>

#include "GameObject.h"

namespace EngineCore {

	class Scene;

	class GameObjectManager {
	friend class GameObject;
	friend class Scene;
	private:
		GameObjectManager();

		static unsigned int m_idCounter;
		static std::vector<std::unique_ptr<GameObject>> m_gameObjects;

		static void AddGameObject(std::unique_ptr<GameObject> gameObject);
		static bool DeleteGameObject(GameObject* gameObjectPtr);
		static bool DeleteGameObject(unsigned int id);
		static bool DeleteGameObject(const std::string& name);
		// Deletes all GameObjects that currently in the GameObjectManager
		static void CleareGameObjects();

		static unsigned int GetNewUniqueIdentifier();
		static GameObject* GetGameObject(unsigned int id);
		static GameObject* GetGameObject(const std::string& name);
		static std::vector<GameObject*> GetAllGameObjects();

		static bool IsNameUnique(const std::string& name);

		static std::string GetHierarchy();
		static void BuildHierarchyString(const GameObject* obj, std::string& outStr, int level);
	};

}