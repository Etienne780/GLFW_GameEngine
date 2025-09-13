#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/EngineTypes.h"
#include "EngineLib/RenderLayerManager.h"

namespace EngineCore {

	RenderLayerID RenderLayerManager::AddLayer(const std::string& layerName, int renderPriority) {
		if (m_lock) {
			Log::Warn("RenderLayerManager: Cant add Layer '{}', Layers can only be added in the start method", layerName);
			return RenderLayerID(ENGINE_INVALID_ID);
		}
		std::string lowerName = FormatUtils::toLowerCase(layerName);

		RenderLayerID rID(m_idManager.GetNewUniqueIdentifier());
		m_nameToID.emplace(lowerName, rID);
		m_idToPriority.emplace(rID, renderPriority);
		return rID;
	}

	std::string RenderLayerManager::GetRenderLayerName(RenderLayerID layerID) {
		for (const auto& [name, rID] : m_nameToID) {
			if (rID == layerID)
				return name;
		}

		Log::Warn("RenderLayerManager: Render layer name with ID {}, not found!", layerID.value);
		return "UNKNOWN";
	}

	RenderLayerID RenderLayerManager::GetRenderLayerID(const std::string& layerName) {
		std::string lowerName = FormatUtils::toLowerCase(layerName);

		auto it = m_nameToID.find(lowerName);
		if (it != m_nameToID.end()) {
			return it->second;
		}

		Log::Warn("RenderLayerManager: Render layer ID with name {}, not found!", layerName);
		return RenderLayerID(ENGINE_INVALID_ID);
	}

	int RenderLayerManager::GetRenderLayerPriority(RenderLayerID layerID) {
		auto it = m_idToPriority.find(layerID);
		if (it != m_idToPriority.end()) {
			return it->second;
		}

		Log::Warn("RenderLayerManager: Render layer Priority with name {} and ID {}, not found!", layerID.value, GetRenderLayerName(layerID));
		return 0;
	}

	std::vector<std::string> RenderLayerManager::GetAllRenderLayerNames() {
		std::vector<std::string> names;
		names.reserve(m_nameToID.size());

		for (const auto& [name, rID] : m_nameToID) {
			names.push_back(name);
		}
		return names;
	}

	std::vector<RenderLayerID> RenderLayerManager::GetAllRenderLayerIDs() {
		std::vector<RenderLayerID> indices;
		indices.reserve(m_nameToID.size());

		for (const auto& [name, rID] : m_nameToID) {
			indices.push_back(rID);
		}
		return indices;
	}

	const std::unordered_map<std::string, RenderLayerID>& RenderLayerManager::GetNameToRenderLayer() {
		return m_nameToID;
	}

	size_t RenderLayerManager::GetRenderLayerCount() {
		return m_nameToID.size();
	}

	void RenderLayerManager::Lock() {
		m_lock = true;
	}

	bool RenderLayerManager::IsLocked() {
		return m_lock;
	}

	void RenderLayerManager::SetupDefaultRenderLayers() {
		AddLayer("Default");
		AddLayer("Debug");
	}

}