#pragma once
#include <string>
#include <vector>
#include <memory>

#include "GameObject.h"

namespace EngineCore {

	class Engine;

	class GameObjectManager {
	friend class GameObject;
	friend class Engine;
	private:
		GameObjectManager();

		GameObjectManager(const GameObjectManager&) = delete;
		GameObjectManager& operator=(const GameObjectManager&) = delete;

		static GameObjectManager& GetInstance();

		unsigned int m_idCounter = 0;
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;

		void AddGameObject(std::unique_ptr<GameObject> gameObject);
		bool DeleteGameObject(GameObject* gameObjectPtr);
		bool DeleteGameObject(unsigned int id);
		bool DeleteGameObject(const std::string& name);
		// Deletes all GameObjects that currently in the GameObjectManager
		void CleareGameObjects();

		unsigned int GetNewUniqueIdentifier();
		GameObject* GetGameObject(unsigned int id);
		GameObject* GetGameObject(const std::string& name);
		std::vector<GameObject*> GetAllGameObjects();

		bool IsNameUnique(const std::string& name);

		std::string GetHierarchy();
		void BuildHierarchyString(const GameObject* obj, std::string& outStr, int level);
	};

}