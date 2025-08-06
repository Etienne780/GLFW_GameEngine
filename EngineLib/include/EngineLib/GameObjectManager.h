#pragma once
#include <vector>
#include <memory>

#include "GameObject.h"

namespace EngineCore {

	class GameObjectManager {
	friend class GameObject;
	private:
		GameObjectManager();

		static std::vector<std::unique_ptr<GameObject>> m_gameObjects;

		static void AddGameObject(GameObject* gameObjectPtr);

		static std::vector<GameObject*> GetAllGameObjects();
	};

}