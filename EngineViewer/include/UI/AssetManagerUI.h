#pragma once
#include "AssetManager.h"

class AssetManagerUI {
public:
	AssetManagerUI(AssetManager* am);
	~AssetManagerUI() = default;

	AssetManager* GetManager();

private:
	AssetManager* m_assetManager = nullptr;
};