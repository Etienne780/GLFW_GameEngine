#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/EngineTypes.h"
#include "EngineLib/Renderlayer.h"

namespace EngineCore {

	RenderLayerID RenderLayer::AddLayer(const std::string& layerName) {
		if (m_lock) {
			Log::Warn("Renderlayer: Cant add Layer '{}', Layers can only be added in the start method", layerName);
			return RenderLayerID(ENGINE_INVALID_ID);
		}
		std::string lowerName = FormatUtils::toLowerCase(layerName);

		// RenderLayerID r(m_layerCounter++);
		RenderLayerID r(m_layerCounter);
		m_layerCounter++;
		m_layers.emplace(lowerName, r);
		return r;
	}

	std::string RenderLayer::GetLayerName(RenderLayerID layerID) {
		for (const auto& [name, ID] : m_layers) {
			if (ID == layerID)
				return name;
		}
		return "UNKNOWN";
	}

	RenderLayerID RenderLayer::GetLayerID(const std::string& layerName) {
		std::string lowerName = FormatUtils::toLowerCase(layerName);

		auto it = m_layers.find(lowerName);
		if (it != m_layers.end()) {
			return it->second;
		}

		return RenderLayerID(ENGINE_INVALID_ID);
	}


	const std::unordered_map<std::string, RenderLayerID>& RenderLayer::GetLayers() {
		return m_layers;
	}

	std::vector<std::string> RenderLayer::GetLayerNames() {
		std::vector<std::string> names;
		names.reserve(m_layers.size());

		for (const auto& [name, id] : m_layers) {
			names.push_back(name);
		}
		return names;
	}

	std::vector<RenderLayerID> RenderLayer::GetLayerIDs() {
		std::vector<RenderLayerID> indices;
		indices.reserve(m_layers.size());

		for (const auto& [name, id] : m_layers) {
			indices.push_back(id);
		}
		return indices;
	}

	size_t RenderLayer::GetLayerCount() {
		return m_layers.size();
	}

	void RenderLayer::Lock() {
		m_lock = true;
	}

	bool RenderLayer::IsLocked() {
		return m_lock;
	}

	void RenderLayer::SetupDefaultLayers() {
		AddLayer("Default");
		AddLayer("Debug");
	}

}