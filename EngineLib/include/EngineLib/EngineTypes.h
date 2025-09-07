#pragma once
#include <limits>

#include <CoreLib/Math/Matrix4x4.h>
#include <CoreLib/FormatUtils.h>

namespace EngineCore {

	class Texture2D;
	class Mesh;
	class Shader;
	class Material;

	constexpr unsigned int ENGINE_INVALID_ID = std::numeric_limits<unsigned int>::max();
	constexpr int ENGINE_SUCCESS = 0;
	constexpr int ENGINE_FAILURE = 1;

	constexpr int ENGINE_STYLING_BAD_TYPE= 2;

	template<typename Tag>
	struct EngineID {
		unsigned int value{};

		constexpr EngineID() = default;
		explicit constexpr EngineID(unsigned int v) : value(v) {}

		constexpr bool operator==(const EngineID& other) const { return value == other.value; }
		constexpr bool operator!=(const EngineID& other) const { return value != other.value; }
		constexpr bool operator<(const EngineID& other) const { return value < other.value; }
		constexpr bool operator>(const EngineID& other) const { return value > other.value; }
		constexpr bool operator<=(const EngineID& other) const { return value <= other.value; }
		constexpr bool operator>=(const EngineID& other) const { return value >= other.value; }

		explicit constexpr operator bool() const { return value != 0; }
	};

	struct Texture2DTag {};
	struct MeshTag {};
	struct ShaderTag {};
	struct MaterialTag {};
	struct GameObjectTag {};
	struct RenderLayerTag {};

	using Asset_Texture2DID = EngineID<Texture2DTag>;
	using Asset_MeshID = EngineID<MeshTag>;
	using Asset_ShaderID = EngineID<ShaderTag>;
	using Asset_MaterialID = EngineID<MaterialTag>;
	using GameObjectID = EngineID<GameObjectTag>;
	using RenderLayerID = EngineID<RenderLayerTag>;

	struct RenderCommand {
		Asset_MaterialID materialID = Asset_MaterialID(ENGINE_INVALID_ID);
		Asset_MeshID meshID = Asset_MeshID(ENGINE_INVALID_ID);
		RenderLayerID renderLayer = RenderLayerID(ENGINE_INVALID_ID);
		const Matrix4x4* modelMatrix = nullptr;
		bool invertMesh = false;
	};

}

namespace std {
	template<typename Tag>
	struct hash<EngineCore::EngineID<Tag>> {
		size_t operator()(const EngineCore::EngineID<Tag>& id) const noexcept {
			return std::hash<unsigned int>()(id.value);
		}
	};
}