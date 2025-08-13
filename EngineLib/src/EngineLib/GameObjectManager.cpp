#include "CoreLib\Log.h"

#include "EngineLib\EngineTypes.h"
#include "EngineLib\GameObjectManager.h"

namespace EngineCore {

	GameObjectManager::GameObjectManager() {
		GameObject::m_gameObjectManager = this;
	}

	GameObjectManager& GameObjectManager::GetInstance() {
		static GameObjectManager instance;
		return instance;
	}

	void GameObjectManager::AddGameObject(std::unique_ptr<GameObject> gameObject) {
		#ifndef NDEBUG
		if (!gameObject) {
			Log::Warn("GameObjectManager: could not add gameObject, ptr is nullptr!");
			return;
		}
		#endif
		m_gameObjects.emplace_back(std::move(gameObject));
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
		std::vector<GameObject*> childrenCopy = gameObjectPtr->m_childObjPtrs;
		for (GameObject* child : childrenCopy) {
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
				std::vector<GameObject*> children = (*it)->m_childObjPtrs;
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
				std::vector<GameObject*> children = (*it)->m_childObjPtrs;
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

	static void CleareGameObjects() {
		
	}

	#pragma endregion

	#pragma region Get

	std::string GameObjectManager::GetHierarchy() {
		std::string hierarchyString;
		hierarchyString.append("GameObject Hierarchy:\n");
		// Find all root GOs
		for (const auto& goPtr : m_gameObjects) {
			if (!goPtr->HasParent()) {
				BuildHierarchyString(goPtr.get(), hierarchyString, 0);
			}
		}

		return hierarchyString;
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
		outStr.append("|- ");
		outStr.append(obj->GetName());
		unsigned int id = obj->GetID();
		if (id == ENGINE_INVALID_ID)
			outStr.append("(INVALID_ID)");
		else 
			outStr.append(FormatUtils::formatString(" ({})", id));
		outStr.append("\n");

		// Adds Children recursively
		for (GameObject* child : obj->GetChildren()) {
			for (int i = 0; i < (level + 1); i++) {
				outStr.append("|  ");
			}
			BuildHierarchyString(child, outStr, level + 1);
		}
	}
}