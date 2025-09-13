#pragma once
#include <unordered_map>
#include <vector>
#include <string>

#include "IDManager.h"
#include "EngineTypes.h"

namespace EngineCore {

	/*
	* @brief A RenderLayer defines a logical rendering layer in the engine.
	*
	* - Each GameObject must belong to exactly one RenderLayer, otherwise it will not be visible.
	* - A Camera specifies which RenderLayers it can see; only objects in those layers are rendered.
	* - RenderLayers also define a rendering priority:
	*     - The priority is an integer value. Lower values are rendered first (further back),
	*       higher values are rendered later (further front).
	*     - Example: a layer with priority -1 is drawn behind a layer with priority 0,
	*       and both are behind a layer with priority 1.
	*     - This makes it possible to enforce strict ordering independent of depth testing,
	*       for example to ensure UI or overlays are always rendered on top.
	*/
	class RenderLayerManager {
		friend class Engine;
	public:
		/*
		* @brief Adds a new render layer with the specified name.
		*        Layers can be assigned to GameObjects or Cameras to control visibility.
		* @param layerName Name of the layer to add.
		* @param renderPriority Render priority of the layer to add (Default is 0).
		* @return The index of the newly added layer.
		*/
		static RenderLayerID AddLayer(const std::string& layerName, int renderPriority = 0);

		/*
		* @brief Retrieves the name of a render layer given its index.
		* @param layerID The ID of the layer.
		* @return The name of the render layer, or "UNKNOWN" if not found.
		*/
		static std::string GetRenderLayerName(RenderLayerID layerID);

		/*
		* @brief Retrieves the ID of a render layer given its name.
		* @param layerName The name of the layer.
		* @return The ID of the render layer.
		*/
		static RenderLayerID GetRenderLayerID(const std::string& layerName);

		/*
		* @brief Retrieves the Priority of a render layer given its ID.
		* @param layerID The ID of the layer.
		* @return The Priority of the render layer, or 0 if not found.
		*/
		static int GetRenderLayerPriority(RenderLayerID layerID);

		/*
		* @brief Returns a vector containing all layer names.
		* @return A vector of all layer names.
		*/
		static std::vector<std::string> GetAllRenderLayerNames();

		/*
		* @brief Returns a vector containing all layer IDs.
		* @return A vector of all layer IDs.
		*/
		static std::vector<RenderLayerID> GetAllRenderLayerIDs();

		static const std::unordered_map<std::string, RenderLayerID>& GetNameToRenderLayer();

		/*
		* @brief Returns the total number of layers currently defined.
		* @return Number of layers.
		*/
		static size_t GetRenderLayerCount();

	private:
		RenderLayerManager() = delete;

		// Internal mapping of layer names to their unique indices
		static inline std::unordered_map<std::string, RenderLayerID> m_nameToID;
		static inline std::unordered_map<RenderLayerID, int> m_idToPriority;
		static inline IDManager m_idManager;

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
		* @brief Sets up default layers for the engine (e.g., "Default", "Debug").
		*/
		static void SetupDefaultRenderLayers();
	};

}