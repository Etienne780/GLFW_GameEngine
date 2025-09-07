#pragma once

namespace EngineCore {
	
	class UIManager {
	public:
		static void Init();
		static void Shutdown();

		UIManager(const UIManager&) = delete;
		UIManager& operator=(const UIManager&) = delete;

		static UIManager* GetInstance();

		void Render();
		void ClearAll();
		void DeleteAll();

	private:
		UIManager() = default;
	};

}