#pragma once
#include <limits>
#include <string>
#include <CoreLib/Math.h>
#include <CoreLib/FormatUtils.h>

#include "Vertex.h"

namespace EngineCore {

	class ShaderBindObject;
	class Texture2D;
	class Mesh;
	class Shader;
	class Material;

	inline constexpr unsigned int ENGINE_INVALID_ID = std::numeric_limits<unsigned int>::max();
	inline constexpr int ENGINE_SUCCESS = 0;
	inline constexpr int ENGINE_FAILURE = 1;

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
	struct InputLayerTag {};
	struct FontTag {};
	struct UIElementTag {};

	using Texture2DID = EngineID<Texture2DTag>;
	using MeshID = EngineID<MeshTag>;
	using ShaderID = EngineID<ShaderTag>;
	using MaterialID = EngineID<MaterialTag>;
	using FontID = EngineID<FontTag>;

	using GameObjectID = EngineID<GameObjectTag>;
	using RenderLayerID = EngineID<RenderLayerTag>;
	using InputLayerID = EngineID<InputLayerTag>;
	using UIElementID = EngineID<UIElementTag>;

	enum class RenderCommandType {
		Mesh = 0,
		Text
	};

	struct TextQuad {
		TextVertex vertices[4];
	};

	struct RenderCommand {
		RenderCommandType type = RenderCommandType::Mesh;
		bool isUI = false;
		bool invertMesh = false;

		MaterialID materialID = MaterialID(ENGINE_INVALID_ID);
		MeshID meshID = MeshID(ENGINE_INVALID_ID);
		// Texture2DID textureOverrideID = Texture2DID(ENGINE_INVALID_ID);
		ShaderBindObject* shaderBindOverride = nullptr;
		RenderLayerID renderLayerID = RenderLayerID(ENGINE_INVALID_ID);
		int zOrder = 0;
		const Matrix4x4* modelMatrix = nullptr;
		Vector4 meshColor = { 1, 1, 1, 1 };
		bool isTransparent = false;

		// For text
		FontID fontID = FontID(ENGINE_INVALID_ID);
		int pixelSize = 0;
		std::vector<TextQuad> textQuads;  // precomputed from string
	};

	inline std::string RenderCommandTypeToString(RenderCommandType type) {
		switch (type) {
		case RenderCommandType::Mesh: return "Mesh";
		case RenderCommandType::Text: return "Text";
		default: return "UNKNOWN";
		}
	}

}

namespace std {
	template<typename Tag>
	struct hash<EngineCore::EngineID<Tag>> {
		size_t operator()(const EngineCore::EngineID<Tag>& id) const noexcept {
			return std::hash<unsigned int>()(id.value);
		}
	};
}