#include "UI/AssetManagerUI.h"

AssetManagerUI::AssetManagerUI(AssetManager* am) {
	m_assetManager = am;
}

AssetManager* AssetManagerUI::GetManager() {
	return m_assetManager;
}