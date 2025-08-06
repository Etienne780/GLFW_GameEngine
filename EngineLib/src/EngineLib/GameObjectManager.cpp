#include "CoreLib\Log.h"

#include "EngineLib\GameObjectManager.h"

namespace EngineCore {

	std::vector<std::unique_ptr<GameObject>> GameObjectManager::m_gameObjects;

	GameObjectManager::GameObjectManager() {}

	void GameObjectManager::AddGameObject(GameObject* gameObjectPtr) {
#ifdef NDEBUG
		if (gameObjectPtr == nullptr) {
			Log::Warn("GameObjectManager: could not add gameObject, ptr is nullptr!");
			return;
		}
#endif
		m_gameObjects.emplace_back(std::unique_ptr<GameObject>(gameObjectPtr));
	}

	std::vector<GameObject*> GameObjectManager::GetAllGameObjects() {
		std::vector<GameObject*> result;
		result.reserve(m_gameObjects.size());
		for (const auto& ptr : m_gameObjects)
			result.push_back(ptr.get());
		return result;
	}
}