#include "CoreLib\Log.h"

#include "EngineLib\Time.h"
#include "EngineLib\Renderer.h"
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

	void GameObjectManager::UpdateGameObjects() {
		for (auto& go : m_gameObjects) {
			go->Update();
		}
	}

	void GameObjectManager::DrawGameObjects() {
		static Renderer& renderer = Renderer::GetInstance();
		renderer.ReserveCommands(m_gameObjects.size());
		for (auto& go : m_gameObjects) {
			go->SubmitDrawCall();
		}
		renderer.DrawAll();
	}

	void GameObjectManager::AddGameObject(std::shared_ptr<GameObject> go) {
		if (!go) return;

		auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), go);
		if (it != m_gameObjects.end()) {
			Log::Warn("GameObjectManager: GameObject '{}' already added", go->GetName());
			return;
		}

		m_gameObjects.emplace_back(go);
	}


	#pragma region Delete

	bool GameObjectManager::DeleteGameObject(std::shared_ptr<GameObject> gameObjectPtr) {
		#ifndef NDEBUG
		if (!gameObjectPtr) {
			Log::Warn("GameObjectManager: could not delete gameObject, ptr is nullptr!");
			return false;
		}
		#endif

		DeleteGameObjectInternal(gameObjectPtr);
		return true;
	}

	bool GameObjectManager::DeleteGameObject(unsigned int id) {
		for (auto& obj : m_gameObjects) {
			if (obj->GetID() == id) {
				DeleteGameObjectInternal(obj);
				return true;
			}
		}
		return false;
	}

	bool GameObjectManager::DeleteGameObject(const std::string& name) {
		for (auto& obj : m_gameObjects) {
			if (obj->GetName() == name) {
				DeleteGameObjectInternal(obj);
				return true;
			}
		}
		return false;
	}

	void GameObjectManager::DeleteGameObjectInternal(std::shared_ptr<GameObject> gameObjectPtr) {
		// delete children recursively
		auto children = gameObjectPtr->m_childObjPtrs;
		for (auto& child : children) {
			DeleteGameObjectInternal(child);
		}

		if (gameObjectPtr->GetParent()) {
			gameObjectPtr->Detach();
		}



		gameObjectPtr->UnregisterCameraFromManager();
		gameObjectPtr->UnaliveComponents();
		gameObjectPtr->m_alive = false;
		m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), gameObjectPtr), m_gameObjects.end());
	}

	void GameObjectManager::CleareAllGameObjects() {
		Log::Debug("Clearing {} game objects", m_gameObjects.size());

		for (auto& obj : m_gameObjects) {
			obj->UnregisterCameraFromManager();
			obj->UnaliveComponents();
			obj->m_alive = false;
		}

		m_gameObjects.clear();
	}

	#pragma endregion

	std::shared_ptr<GameObject> GameObjectManager::GetGameObject(unsigned int id) {
		for (auto& obj : m_gameObjects) {
			if (obj->GetID() == id) {
				return obj;
			}
		}
		Log::Warn("GameObjectManager: no GameObject with ID '{}' found!", id);
		return nullptr;
	}

	std::shared_ptr<GameObject> GameObjectManager::GetGameObject(const std::string& name) {
		for (auto& obj : m_gameObjects) {
			if (obj->GetName() == name) {
				return obj;
			}
		}
		Log::Warn("GameObjectManager: no GameObject with name '{}' found!", name);
		return nullptr;
	}

	void GameObjectManager::AddCamera(std::weak_ptr<Component::Camera> camera) {
		auto cameraptr = camera.lock();
		if (!cameraptr) {
			Log::Error("GameObjectManager: Cant add camera, camera is nullptr");
			return;
		}

		for (auto& cam : m_cameras) {
			if (cam.lock() == cameraptr) {
				return;
			}
		}

		if (!m_mainCamera.lock()) {
			m_mainCamera = camera;
		}

		m_cameras.push_back(camera);
	}

	void GameObjectManager::RemoveCamera(const std::weak_ptr<Component::Camera> camera) {
		auto camLocked = camera.lock();
		if (!camLocked) {
			Log::Error("GameObjectManager: Cant remove camera, camera is nullptr");
			return;
		}

		auto mainLocked = m_mainCamera.lock();
		bool changeMainCamera = (mainLocked && camLocked == mainLocked);

		auto it = std::remove_if(m_cameras.begin(), m_cameras.end(),
			[&](const std::weak_ptr<Component::Camera>& w) {
				return camLocked == w.lock();
			});
		m_cameras.erase(it, m_cameras.end());

		if (changeMainCamera) {
			if (m_cameras.empty()) {
				m_mainCamera.reset();
			}
			else {
				m_mainCamera = m_cameras.front();
			}
		}
	}

	std::weak_ptr<Component::Camera> GameObjectManager::GetMainCamera() const {
		return m_mainCamera;
	}
	
	void GameObjectManager::SetMainCamera(std::weak_ptr<Component::Camera> camera) {
		auto cameraptr = camera.lock();
		// sets the main cam to the first in the list
		if (!cameraptr) {
			if (!m_mainCamera.lock() && !m_cameras.empty()) {
				m_mainCamera = m_cameras.front();
			}
			return;
		}

		bool isCameraInList = false;
		for (auto& cam : m_cameras) {
			if (cam.lock() == cameraptr) {
				isCameraInList = true;
				break;
			}
		}

		if (!isCameraInList) {
			AddCamera(camera);
		}

		m_mainCamera = camera;
	}

	#pragma region Get

	std::string GameObjectManager::GetHierarchy() {
		std::string hierarchyString;
		hierarchyString.append("GameObject Hierarchy:\n");
		// Find all root GOs
		for (const auto& goPtr : m_gameObjects) {
			if (!goPtr->HasParent()) {
				BuildHierarchyString(goPtr, hierarchyString, 0);
			}
		}

		return hierarchyString;
	}

	unsigned int GameObjectManager::GetNewUniqueIdentifier() {
		return m_idCounter++;
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

	void GameObjectManager::BuildHierarchyString(const std::weak_ptr<GameObject> obj, std::string& outStr, int level) {
		auto objptr = obj.lock();
		outStr.append("|- ");
		outStr.append(objptr->GetName());
		unsigned int id = objptr->GetID();
		if (id == ENGINE_INVALID_ID)
			outStr.append("(INVALID_ID)");
		else 
			outStr.append(FormatUtils::formatString(" ({})", id));
		outStr.append("\n");

		// Adds Children recursively
		for (auto& child : objptr->GetChildren()) {
			for (int i = 0; i < (level + 1); i++) {
				outStr.append("|  ");
			}
			BuildHierarchyString(child, outStr, level + 1);
		}
	}
}