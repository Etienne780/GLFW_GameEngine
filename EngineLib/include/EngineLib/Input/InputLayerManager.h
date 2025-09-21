#pragma once
#include <unordered_map>
#include <vector>
#include <string>

#include "EngineLib/IDManager.h"
#include "EngineLib/EngineTypes.h"

namespace EngineCore {
	
	class Engine;

	class InputLayerManager {
	friend class Engine;
	public:
		/*
		* @brief Adds a new input layer with the specified name.
		*        Layers can be used to specify what inputs can used a what time
		* @param layerName Name of the layer to add.
		* @return The index of the newly added layer.
		*/
		static InputLayerID AddLayer(const std::string& layerName);

		/*
		* @brief Retrieves the name of a input layer given its index.
		* @param layerID The ID of the layer.
		* @return The name of the input layer, or "UNKNOWN" if not found.
		*/
		static std::string GetLayerName(InputLayerID layerID);

		/*
		* @brief Retrieves the ID of a input layer given its name.
		* @param layerName The name of the layer.
		* @return The ID of the render layer.
		*/
		static InputLayerID GetLayerID(const std::string& layerName);

		/*
		* @brief Returns a vector containing all layer names.
		* @return A vector of all layer names.
		*/
		static std::vector<std::string> GetAllInputLayerNames();

		/*
		* @brief Returns a vector containing all layer IDs.
		* @return A vector of all layer IDs.
		*/
		static std::vector<InputLayerID> GetAllInputLayerIDs();

		static const std::unordered_map<std::string, InputLayerID>& GetNameToInputLayer();

		/*
		* @brief Returns the total number of layers currently defined.
		* @return Number of layers.
		*/
		static size_t GetInputLayerCount();

	private:
		InputLayerManager() = delete;

		// Internal mapping of layer names to their unique indices
		static inline std::unordered_map<std::string, InputLayerID> m_nameToID;
		static inline IDManager m_idManager;

		/*
		* @brief Sets up default layers for the engine (e.g., "Default (Prio = 0)", "Debug (Prio = 1)").
		*/
		static void SetupDefaultInputLayers();
	};

}