#pragma once
#include <string>
#include <vector>
#include <memory>
#include <queue>

#include "GameObject.h"
#include "Components\Camera_C.h"

namespace EngineCore {

	namespace Component {
		class Camera;
	}

	class GameObjectManager {
	friend class DebuggerWindows;
	friend class GameObject;
	friend class Engine;
	private:
		GameObjectManager();

		GameObjectManager(const GameObjectManager&) = delete;
		GameObjectManager& operator=(const GameObjectManager&) = delete;

		static GameObjectManager& GetInstance();

		unsigned int m_idCounter = 0;
		bool m_idFallback = false;// gets set to true when the id limit(Integer.Max) is reached.
		std::queue<unsigned int> m_freeIDs;
		std::vector<std::shared_ptr<GameObject>> m_gameObjects;
		std::vector<std::weak_ptr<Component::Camera>> m_cameras;
		std::weak_ptr<Component::Camera> m_mainCamera;

		/**
		* @brief calles the start and update func of the GameObjects
		*/
		void UpdateGameObjects();
		/**
		* @brief Draws a Gameobject if it has a component that can be drawn
		*/
		void DrawGameObjects();

		void AddGameObject(std::shared_ptr<GameObject> go);
		bool DeleteGameObject(std::shared_ptr<GameObject> gameObjectPtr);
		bool DeleteGameObject(unsigned int id);
		bool DeleteGameObject(const std::string& name);
		void DeleteGameObjectInternal(std::shared_ptr<GameObject> gameObjectPtr);
		// Deletes all GameObjects that currently in the GameObjectManager and clears the idCounter
		void CleareAllGameObjects();
		std::shared_ptr<GameObject> GetGameObject(unsigned int id);
		std::shared_ptr<GameObject> GetGameObject(const std::string& name);

		// adds a camera to the camera list
		void AddCamera(std::weak_ptr<Component::Camera> camera);
		void RemoveCamera(const std::weak_ptr<Component::Camera> camera);
		std::weak_ptr<Component::Camera> GetMainCamera() const;
		void SetMainCamera(std::weak_ptr<Component::Camera> camera);

		unsigned int GetNewUniqueIdentifier();
		// searches for the firs free id. should be called if the id limit is reachd
		unsigned int GetNewUniqueIdentifierFallback();
		std::vector<GameObject*> GetAllGameObjects();

		bool IsNameUnique(const std::string& name);

		std::string GetHierarchy();
		void BuildHierarchyString(const std::weak_ptr<GameObject> obj, std::string& outStr, int level);
	};

}