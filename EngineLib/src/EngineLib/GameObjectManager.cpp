#include "CoreLib\Log.h"

#include "EngineLib\GameObjectManager.h"

namespace EngineCore {

	unsigned int GameObjectManager::m_idCounter = 0;
	std::string GameObjectManager::s_hierarchyString;
	std::vector<std::unique_ptr<GameObject>> GameObjectManager::m_gameObjects;

	GameObjectManager::GameObjectManager() {}

	void GameObjectManager::AddGameObject(GameObject* gameObjectPtr) {
		#ifndef NDEBUG
		if (gameObjectPtr == nullptr) {
			Log::Warn("GameObjectManager: could not add gameObject, ptr is nullptr!");
			return;
		}
		#endif
		m_gameObjects.emplace_back(std::unique_ptr<GameObject>(gameObjectPtr));
	}

	#pragma region Delete

	bool GameObjectManager::DeleteGameObject(GameObject* gameObjectPtr) {
		#ifndef NDEBUG
		if (gameObjectPtr == nullptr) {
			Log::Warn("GameObjectManager: could not delete gameObject, ptr is nullptr!");
			return false;
		}
		#endif

		// delete all the child GO
		for (GameObject* child : gameObjectPtr->m_childObjPtrs) {
			DeleteGameObject(child);
		}

		if (gameObjectPtr->GetParent()) {
			gameObjectPtr->Detach();
		}

		for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it) {
			if (it->get() == gameObjectPtr) {
				m_gameObjects.erase(it);
				return true;
			}
		}
		return false;
	}

	bool GameObjectManager::DeleteGameObject(unsigned int id) {
		for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it) {
			if ((*it)->GetID() == id) {
				// delete all the child GO
				auto children = (*it)->m_childObjPtrs;
				for (GameObject* child : children) {
					DeleteGameObject(child);
				}

				if ((*it)->GetParent()) {
					(*it)->Detach();
				}

				m_gameObjects.erase(it);
				return true;
			}
		}
		return false;
	}

	bool GameObjectManager::DeleteGameObject(const std::string& name) {
		for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it) {
			if ((*it)->GetName() == name) {
				// delete all the child GO
				auto children = (*it)->m_childObjPtrs;
				for (GameObject* child : children) {
					DeleteGameObject(child);
				}

				if ((*it)->GetParent()) {
					(*it)->Detach();
				}

				m_gameObjects.erase(it);
				return true;
			}
		}
		return false;
	}

	#pragma endregion

	#pragma region Get

	std::string& GameObjectManager::GetHierarchy() {
		s_hierarchyString.clear();

		// Alle Root-GameObjects finden (keinen Parent)
		for (const auto& goPtr : m_gameObjects) {
			if (!goPtr->HasParent()) {
				BuildHierarchyString(goPtr.get(), s_hierarchyString, 0);
			}
		}

		return s_hierarchyString;
	}

	unsigned int GameObjectManager::GetNewUniqueIdentifier() {
		return m_idCounter++;
	}

	GameObject* GameObjectManager::GetGameObject(unsigned int id) {
		for (const auto& go : m_gameObjects) {
			if (go->GetID() == id)
				return go.get();
		}
		return nullptr;
	}

	GameObject* GameObjectManager::GetGameObject(const std::string& name) {
		for (const auto& go : m_gameObjects) {
			if (go->GetName() == name)
				return go.get();
		}
		return nullptr;
	}

	std::vector<GameObject*> GameObjectManager::GetAllGameObjects() {
		std::vector<GameObject*> result;
		result.reserve(m_gameObjects.size());
		for (const auto& ptr : m_gameObjects)
			result.push_back(ptr.get());
		return result;
	}

	#pragma endregion

	bool GameObjectManager::IsNameUnique(const std::string& name) {
		for (const auto& go : m_gameObjects) {
			if (go->GetName() == name)
				return false;
		}
		return true;
	}

	void GameObjectManager::BuildHierarchyString(const GameObject* obj, std::string& outStr, int level) {
		outStr.append(std::string(level * 2, ' '));  // Einrückung, z.B. 2 Leerzeichen pro Ebene
		outStr.append(obj->GetName());
		outStr.append("\n");

		// Alle Kinder rekursiv hinzufügen
		for (GameObject* child : obj->GetChildren()) {
			BuildHierarchyString(child, outStr, level + 1);
		}
	}
}