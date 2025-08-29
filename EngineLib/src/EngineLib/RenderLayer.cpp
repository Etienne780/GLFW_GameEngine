#include <CoreLib/Log.h>
#include <CoreLib/FormatUtils.h>

#include "EngineLib/EngineTypes.h"
#include "EngineLib/Renderlayer.h"

namespace EngineCore {

	unsigned int RenderLayer::AddLayer(const std::string& layerName) {
		if (m_lock) {
			Log::Warn("Renderlayer: Cant add Layer '{}', Layers can only be added in the start method", layerName);
			return -1;
		}
		std::string lowerName = FormatUtils::toLowerCase(layerName);

		m_layers.emplace(lowerName, m_layerCounter);
		return m_layerCounter++;
	}

	std::string RenderLayer::GetLayerName(unsigned int layerIndex) {
		for (const auto& [name, index] : m_layers) {
			if (index == layerIndex)
				return name;
		}
		return "UNKNOWN";
	}

	unsigned int RenderLayer::GetLayerIndex(const std::string& layerName) {
		std::string lowerName = FormatUtils::toLowerCase(layerName);

		auto it = m_layers.find(lowerName);
		if (it != m_layers.end()) {
			return it->second;
		}

		return ENGINE_INVALID_ID;
	}


	const std::unordered_map<std::string, unsigned int>& RenderLayer::GetLayers() {
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

	std::vector<unsigned int> RenderLayer::GetLayerIndices() {
		std::vector<unsigned int> indices;
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