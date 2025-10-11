#include <vector>
#include <glad/glad.h>
#include <CoreLib/Log.h>

#include "EngineLib/ResourceManager.h"
#include "EngineLib/Vertex.h"
#include "EngineLib/Nurom_Bold_ttf.h"
#include "EngineLib/FontManager.h"
#include "EngineLib/AssetRepository.h"

EngineCore::Texture2DID g_engineTextureMissingID = EngineCore::Texture2DID(EngineCore::ENGINE_INVALID_ID);
EngineCore::Texture2DID g_engineTextureCursedmod3ID = EngineCore::Texture2DID(EngineCore::ENGINE_INVALID_ID);
EngineCore::MeshID g_engineMeshCubeID = EngineCore::MeshID(EngineCore::ENGINE_INVALID_ID);
EngineCore::MeshID g_engineMeshPlainID = EngineCore::MeshID(EngineCore::ENGINE_INVALID_ID);
EngineCore::MeshID g_engineMeshUIPlainID = EngineCore::MeshID(EngineCore::ENGINE_INVALID_ID);
EngineCore::ShaderID g_engineShaderDefaultID = EngineCore::ShaderID(EngineCore::ENGINE_INVALID_ID);
EngineCore::ShaderID g_engineShaderDefaultTextID = EngineCore::ShaderID(EngineCore::ENGINE_INVALID_ID);
EngineCore::ShaderID g_engineShaderDefaultUIID = EngineCore::ShaderID(EngineCore::ENGINE_INVALID_ID);
EngineCore::MaterialID g_engineMaterialDefaultID = EngineCore::MaterialID(EngineCore::ENGINE_INVALID_ID);
EngineCore::MaterialID g_engineMaterialDefaultTextID = EngineCore::MaterialID(EngineCore::ENGINE_INVALID_ID);
EngineCore::MaterialID g_engineMaterialDefaultUIID = EngineCore::MaterialID(EngineCore::ENGINE_INVALID_ID);
EngineCore::FontID g_engineFontDefaultID = EngineCore::FontID(EngineCore::ENGINE_INVALID_ID);

namespace EngineCore::ASSETS::ENGINE::TEXTURE {
    Texture2DID Missing() { return g_engineTextureMissingID; }
    Texture2DID Cursedmod3() { return g_engineTextureCursedmod3ID; }
}

namespace EngineCore::ASSETS::ENGINE::MESH {
	MeshID Cube() { return g_engineMeshCubeID; }
    MeshID Plain() { return g_engineMeshPlainID; }
    MeshID UIPlain() { return g_engineMeshUIPlainID; }
}

namespace EngineCore::ASSETS::ENGINE::SHADER {
    ShaderID Default() { return g_engineShaderDefaultID; }
    ShaderID DefaultText() { return g_engineShaderDefaultTextID; }
    ShaderID DefaultUI() { return g_engineShaderDefaultUIID; }
}

namespace EngineCore::ASSETS::ENGINE::MATERIAL {
    MaterialID Default() { return g_engineMaterialDefaultID; }
    MaterialID DefaultText() { return g_engineMaterialDefaultTextID; }
    MaterialID DefaultUI() { return g_engineMaterialDefaultUIID; }
}

namespace EngineCore::ASSETS::ENGINE::FONT {
    FontID Default() { return g_engineFontDefaultID; }
}

namespace EngineCore {
	
	void LoadBaseAsset() {
		auto* rm = ResourceManager::GetInstance();
    
        #pragma region TEXTURE::Missing
        {
            g_engineTextureMissingID = rm->AddTexture2DFromMemory(nullptr, 0, 0, 0);
        }
        #pragma endregion

        #pragma region TEXTURE::Missing
        {
            g_engineTextureCursedmod3ID = rm->AddTexture2DFromFile("assets\\images.jpg");
        }
        #pragma endregion

        #pragma region MESH::Cube
		{
            Vertex vertices[] = {
                // Back
                { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 0
                {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f }, // 1
                {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f }, // 2
                { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f }, // 3

                // Front
                { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f }, // 4
                {  0.5f, -0.5f,  0.5f, 1.0f, 0.0f }, // 5
                {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f }, // 6
                { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f }, // 7

                // Left
                { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 8
                { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f }, // 9
                { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f }, // 10
                { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f }, // 11

                // Right
                { 0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 12
                { 0.5f, -0.5f,  0.5f, 1.0f, 0.0f }, // 13
                { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f }, // 14
                { 0.5f,  0.5f, -0.5f, 0.0f, 1.0f }, // 15

                // Down
                { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 16
                {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f }, // 17
                {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f }, // 18
                { -0.5f, -0.5f,  0.5f, 0.0f, 1.0f }, // 19

                // Up
                { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f }, // 20
                {  0.5f,  0.5f, -0.5f, 1.0f, 0.0f }, // 21
                {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f }, // 22
                { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f }  // 23
            };

            unsigned int indices[] = {
                // Back (Z-)
                0, 1, 2, 0, 2, 3,

                // Front (Z+)
                4, 6, 5, 4, 7, 6,

                // Left (X-)
                8, 10, 9, 8, 11, 10,

                // Right (X+)
                12, 13, 14, 12, 14, 15,

                // Down (Y-)
                16, 18, 17, 16, 19, 18,

                // Up (Y+)
                20, 21, 22, 20, 22, 23
            };

            size_t verticesSize = sizeof(vertices) / sizeof(vertices[0]);
            size_t indicesSize = sizeof(indices) / sizeof(indices[0]);

            g_engineMeshCubeID = rm->AddMeshFromMemory(vertices, verticesSize, indices, indicesSize);
		}
        #pragma endregion

        #pragma region MESH::Plain
        {
            Vertex vertices[] = {
                { -0.5f, -0.5f,  0, 0.0f, 1.0f }, // bottom-left
                {  0.5f, -0.5f,  0, 1.0f, 1.0f }, // bottom-right
                {  0.5f,  0.5f,  0, 1.0f, 0.0f }, // top-right
                { -0.5f,  0.5f,  0, 0.0f, 0.0f }, // top-left
            };

            unsigned int indices[] = {
                // Back (Z-)
                0, 1, 2, 0, 2, 3,
            };

            size_t verticesSize = sizeof(vertices) / sizeof(vertices[0]);
            size_t indicesSize = sizeof(indices) / sizeof(indices[0]);

            g_engineMeshPlainID = rm->AddMeshFromMemory(vertices, verticesSize, indices, indicesSize);
        }
        #pragma endregion

        #pragma region MESH::UIPlain
        {
            Vertex vertices[] = {
                { 0.0f, 1.0f,  0, 0.0f, 1.0f }, // bottom-left
                { 1.0f, 1.0f,  0, 1.0f, 1.0f }, // bottom-right
                { 1.0f, 0.0f,  0, 1.0f, 0.0f }, // top-right
                { 0.0f, 0.0f,  0, 0.0f, 0.0f }, // top-left
            };

            unsigned int indices[] = {
                // Back (Z-)
                0, 1, 2, 0, 2, 3,
            };

            size_t verticesSize = sizeof(vertices) / sizeof(vertices[0]);
            size_t indicesSize = sizeof(indices) / sizeof(indices[0]);

            g_engineMeshUIPlainID = rm->AddMeshFromMemory(vertices, verticesSize, indices, indicesSize);
        }
        #pragma endregion

        #pragma region SHADER::Default
        {
            std::string vert = R"(
                #version 330 core
                layout(location = 0) in vec3 aPos;
                layout(location = 1) in vec2 aTexCoord;
                layout(location = 2) in vec3 aNormal;
                layout(location = 3) in mat4 instanceModel;
                
                out vec2 TexCoord;
                uniform mat4 view;
                uniform mat4 projection;
                
                void main() {
                    gl_Position = projection * view * instanceModel * vec4(aPos, 1.0);
                    TexCoord = aTexCoord;
                }
            )";
            std::string frag = R"(
                #version 330 core
                out vec4 FragColor;
                in vec2 TexCoord;

                uniform sampler2D utexture;
                uniform vec4 umeshColor;

                void main()
                {
                    vec4 texColor = texture(utexture, TexCoord);
                    if(texColor.a < 0.1)
                        discard;
                	FragColor = texColor * umeshColor;
                }
            )";
            g_engineShaderDefaultID = rm->AddShaderFromMemory(vert, frag);
        }
        #pragma endregion

        #pragma region SHADER::DefaultText
        {
            std::string vert = R"(
                #version 330 core
                layout(location = 0) in vec3 aPos;
                layout(location = 1) in vec2 aTexCoord;
                
                out vec2 TexCoord;
                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;
                
                void main() {
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                    TexCoord = aTexCoord;
                }
            )";
            std::string frag = R"(
                #version 330 core
                out vec4 FragColor;
                in vec2 TexCoord;
                
                uniform sampler2D utexture;
                uniform vec4 umeshColor;
                
                void main()
                {
                    float alpha = texture(utexture, TexCoord).r;
                    if(alpha < 0.1)
                        discard;
                
                    FragColor = vec4(umeshColor.rgb, umeshColor.a * alpha);
                }
            )";
            g_engineShaderDefaultTextID = rm->AddShaderFromMemory(vert, frag);
        }
        #pragma endregion

        #pragma region SHADER::DefaultUI
        {
            std::string vert = R"(
                #version 330 core
                layout(location = 0) in vec3 aPos;
                layout(location = 1) in vec2 aTexCoord;
                layout(location = 3) in mat4 instanceModel;
                
                out vec2 vTexCoord;
                
                uniform mat4 projection;
                
                void main() {
                    gl_Position = projection * instanceModel * vec4(aPos, 1.0);
                    vTexCoord = aTexCoord;
                }
            )";

            std::string frag = R"(
             #version 330 core
                in vec2 vTexCoord;
                out vec4 FragColor;
                
                uniform vec4 uBackgroundColor;
                uniform vec4 uBorderColor;
                uniform vec4 uBorderRadius;   // top-left, top-right, bottom-right, bottom-left (px)
                uniform vec4 uBorderWidth;    // top, right, bottom, left (px)
                uniform vec2 uSize;           // element size (px)
                uniform bool uFlipY;
                
                // Accurate AA smoothing
                float aaSmooth(float sd) {
                    float fw = fwidth(sd);
                    fw = max(fw, 1e-6);
                    float aa = 1.7; // factor >1 => smoother, <1 => sharper
                    return smoothstep(-0.5 * fw * aa, 0.5 * fw * aa, -sd);
                }
                
                // CSS-style rounded rect SDF (continuous variant)
                float sdRoundRectCSS(vec2 p, vec2 size, vec4 radius) {
                    // Clamp radii
                    float limit = min(size.x, size.y) * 0.5;
                    radius = clamp(radius, vec4(0.0), vec4(limit));
                
                    // Compute distances to each corner
                    vec2 q = p;
                    // Map p into coordinate system of each corner
                    if (p.x < radius.w && p.y < radius.w) { // bottom-left
                        q = p - vec2(radius.w, radius.w);
                        return length(q) - radius.w;
                    }
                    if (p.x > size.x - radius.z && p.y < radius.z) { // bottom-right
                        q = p - vec2(size.x - radius.z, radius.z);
                        return length(q) - radius.z;
                    }
                    if (p.x > size.x - radius.y && p.y > size.y - radius.y) { // top-right
                        q = p - vec2(size.x - radius.y, size.y - radius.y);
                        return length(q) - radius.y;
                    }
                    if (p.x < radius.x && p.y > size.y - radius.x) { // top-left
                        q = p - vec2(radius.x, size.y - radius.x);
                        return length(q) - radius.x;
                    }
                
                    // Otherwise inside edge region (no corner)
                    vec2 d = max(vec2(-p.x, -p.y), vec2(p.x - size.x, p.y - size.y));
                    return max(d.x, d.y);
                }
                
                void main() {
                    vec2 coord = vec2(vTexCoord.x * uSize.x,
                                      (uFlipY ? (1.0 - vTexCoord.y) : vTexCoord.y) * uSize.y);
                
                    // Outer and inner radius setup
                    vec4 rOut = clamp(uBorderRadius, vec4(0.0), vec4(min(uSize.x, uSize.y) * 0.5));

                    float top    = uBorderWidth.z;
                    float right  = uBorderWidth.w;
                    float bottom = uBorderWidth.x;
                    float left   = uBorderWidth.y;
                
                    vec2 innerSize = vec2(uSize.x - (left + right), uSize.y - (top + bottom));
                    innerSize = max(innerSize, vec2(0.0));
                
                    vec4 rIn = vec4(
                        max(rOut.x - max(left, top), 0.0),
                        max(rOut.y - max(top, right), 0.0),
                        max(rOut.z - max(right, bottom), 0.0),
                        max(rOut.w - max(bottom, left), 0.0)
                    );
                
                    float sdOuter = sdRoundRectCSS(coord, uSize, rOut);
                    float sdInner = sdRoundRectCSS(coord - vec2(left, bottom), innerSize, rIn);
                
                    float outerMask = aaSmooth(sdOuter);
                    float innerMask = aaSmooth(sdInner);
                
                    float fillMask = outerMask * innerMask;
                    float borderMask = outerMask * (1.0 - innerMask);
                
                    float borderA = borderMask * uBorderColor.a;
                    float fillA   = fillMask * uBackgroundColor.a;
                    float outA = borderA + fillA * (1.0 - borderA);
                    vec3 outRGB = uBorderColor.rgb * borderA + uBackgroundColor.rgb * fillA * (1.0 - borderA);
                
                    FragColor = vec4(outRGB, outA);
                }
            )";
            g_engineShaderDefaultUIID = rm->AddShaderFromMemory(vert, frag);
        }
        #pragma endregion

        #pragma region MATERIAL::Default
        {
            g_engineMaterialDefaultID = rm->AddMaterial(g_engineShaderDefaultID);
            Material* mat = rm->GetMaterial(g_engineMaterialDefaultID);
            mat->SetParam("texture", g_engineTextureMissingID);
        }
        #pragma endregion

        #pragma region MATERIAL::DefaultText
        {
            g_engineMaterialDefaultTextID = rm->AddMaterial(g_engineShaderDefaultTextID);
        }
        #pragma endregion

        #pragma region MATERIAL::DefaultUI
        {
            g_engineMaterialDefaultUIID = rm->AddMaterial(g_engineShaderDefaultUIID);
        }
        #pragma endregion

        #pragma region FONT::Default
        {
           g_engineFontDefaultID = rm->AddFontFromMemory(StaticFont::Nurom_Bold_ttf, StaticFont::Nurom_Bold_ttf_len);
        }
        #pragma endregion

	}

}