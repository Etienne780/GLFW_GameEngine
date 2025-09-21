#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/Input/InputLayerManager.h"

namespace EngineCore {
	
	InputLayerID InputLayerManager::AddLayer(const std::string& layerName) {
		std::string lowerName = FormatUtils::toLowerCase(layerName);

		InputLayerID iID(m_idManager.GetNewUniqueIdentifier());
		m_nameToID.emplace(lowerName, iID);
		return iID;
	}

	std::string InputLayerManager::GetLayerName(InputLayerID layerID) {
		for (const auto& [name, rID] : m_nameToID) {
			if (rID == layerID)
				return name;
		}

		Log::Warn("InputLayerManager: Input layer name with ID {}, not found!", layerID.value);
		return "UNKNOWN";
	}

	InputLayerID InputLayerManager::GetLayerID(const std::string& layerName) {
		std::string lowerName = FormatUtils::toLowerCase(layerName);

		auto it = m_nameToID.find(lowerName);
		if (it != m_nameToID.end()) {
			return it->second;
		}

		Log::Warn("InputLayerManager: Input layer ID with name {}, not found!", layerName);
		return InputLayerID(ENGINE_INVALID_ID);
	}

	std::vector<std::string> InputLayerManager::GetAllInputLayerNames() {
		std::vector<std::string> names;
		names.reserve(m_nameToID.size());

		for (const auto& [name, rID] : m_nameToID) {
			names.push_back(name);
		}
		return names;
	}

	std::vector<InputLayerID> InputLayerManager::GetAllInputLayerIDs() {
		std::vector<InputLayerID> indices;
		indices.reserve(m_nameToID.size());

		for (const auto& [name, rID] : m_nameToID) {
			indices.push_back(rID);
		}
		return indices;
	}

	const std::unordered_map<std::string, InputLayerID>& InputLayerManager::GetNameToInputLayer() {
		return m_nameToID;
	}

	size_t InputLayerManager::GetInputLayerCount() {
		return m_nameToID.size();
	}

	void InputLayerManager::SetupDefaultInputLayers() {
		AddLayer("Debug");
	}

}