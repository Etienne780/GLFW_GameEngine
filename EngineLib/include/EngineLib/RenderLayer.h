#pragma once
#include <unordered_map>
#include <vector>
#include <string>

namespace EngineCore {

	class RenderLayer {
		friend class Engine;
	public:
		/*
		* @brief Adds a new render layer with the specified name.
		*        Layers can be assigned to GameObjects or Cameras to control visibility.
		* @param layerName Name of the layer to add.
		* @return The index of the newly added layer.
		*/
		static unsigned int AddLayer(const std::string& layerName);

		/*
		* @brief Retrieves the name of a render layer given its index.
		* @param layerIndex The index of the layer.
		* @return The name of the layer, or "UNKNOWN" if not found.
		*/
		static std::string GetLayerName(unsigned int layerIndex);

		/*
		* @brief Retrieves the index of a render layer given its name.
		* @param layerName The name of the layer.
		* @return The index of the layer.
		*/
		static unsigned int GetLayerIndex(const std::string& layerName);

		/*
		* @brief Returns a reference to the internal mapping of layer names to their indices.
		* @return The map of all layer names and their corresponding indices.
		*/
		static const std::unordered_map<std::string, unsigned int>& GetLayers();

		/*
		* @brief Returns a vector containing all layer names.
		* @return A vector of all layer names.
		*/
		static std::vector<std::string> GetLayerNames();

		/*
		* @brief Returns a vector containing all layer indices.
		* @return A vector of all layer indices.
		*/
		static std::vector<unsigned int> GetLayerIndices();

		/*
		* @brief Returns the total number of layers currently defined.
		* @return Number of layers.
		*/
		static size_t GetLayerCount();

	private:
		// Internal mapping of layer names to their unique indices
		static inline std::unordered_map<std::string, unsigned int> m_layers;

		// Counter to assign the next available layer index
		static inline unsigned int m_layerCounter = 0;

		// Flag to prevent adding new layers after initialization
		static inline bool m_lock = false;

		/*
		* @brief Prevents any further layers from being added.
		*/
		static void Lock();

		/*
		* @brief Checks whether the RenderLayer system is locked.
		* @return True if locked, false otherwise.
		*/
		static bool IsLocked();

		/*
		* @brief Sets up default layers for the engine (e.g., "Default", "UI", "Background").
		*/
		static void SetupDefaultLayers();
	};

}