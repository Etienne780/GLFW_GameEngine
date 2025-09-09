#include <algorithm>
#include <glad\glad.h>

#include "EngineLib\ResourceManager.h"
#include "EngineLib\Components\Camera_C.h"
#include "EngineLib\FontManager.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\Mesh.h"
#include "EngineLib\Material.h"
#include "EngineLib\Shader.h"
#include "EngineLib\Renderer.h"

namespace EngineCore {

    Renderer& Renderer::GetInstance() {
        static Renderer instance;
        return instance;
    }

    void Renderer::Submit(const RenderCommand& cmd) {
        if (cmd.renderLayer.value == ENGINE_INVALID_ID)
            return;
        m_commands.push_back(cmd);
    }

    void Renderer::ReserveCommands(size_t count) {
        m_commands.reserve(count);
        m_instanceMatrices.reserve(count);
    }

    unsigned int textVAO = 0;
    unsigned int textVBO = 0;
    unsigned int textEBO = 0;
    void InitTextRenderer() {
        glGenVertexArrays(1, &textVAO);
        glGenBuffers(1, &textVBO);
        glGenBuffers(1, &textEBO);

        glBindVertexArray(textVAO);

        // VBO für 4 Vertices
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TextVertex) * 4, nullptr, GL_DYNAMIC_DRAW);

        // EBO für Quad-Indizes
        unsigned int indices[6] = { 0,1,2,2,3,0 };
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Vertex-Attribs: position (vec3) + uv (vec2)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, uv));

        glBindVertexArray(0);
    }

    void DrawQuad(const TextQuad& quad) {
        glBindVertexArray(textVAO);

        // Quad-Daten hochladen (dynamisch)
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad.vertices), quad.vertices);

        // Zeichnen
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    void Renderer::DrawAll() {

        static bool once = true;
        if (once) {
            InitTextRenderer();
            once = false;
        }

        std::sort(m_commands.begin(), m_commands.end(), [](const auto& a, const auto& b) {
            if (a.type != b.type) return a.type < b.type;
            if (a.materialID != b.materialID) return a.materialID < b.materialID;
            if (a.meshID != b.meshID) return a.meshID < b.meshID;
            return a.invertMesh < b.invertMesh;
        });
        
        Shader* currentShader = nullptr;
        Asset_ShaderID currentShaderID(ENGINE_INVALID_ID);
        Material* currentMaterial = nullptr;
        Asset_MaterialID currentMaterialID(ENGINE_INVALID_ID);
        Mesh* currentMesh = nullptr;
        Asset_MeshID currentMeshID(ENGINE_INVALID_ID);
        bool currentInvertMesh = false;
        
        ResourceManager& rm = ResourceManager::GetInstance();
        
        std::shared_ptr<Component::Camera> camptr = GameObject::GetMainCamera();
        if (camptr->IsDisable()) {
            Log::Warn("Renderer: Cant render, main Camera is disabled");
            return;
        }
        if (camptr->GetGameObject()->IsDisabled()) {
            Log::Warn("Renderer: Cant render, main Camera GameObject is disabled");
            return;
        }
        Matrix4x4 cameraProjectionMat = camptr->GetProjectionMatrix();
        Matrix4x4 cameraViewMat = camptr->GetViewMatrix();
        const std::vector<RenderLayerID>& renderLayers = camptr->GetRenderLayers();
        
        auto flushBatch = [&](Mesh* mesh, Shader* shader, bool invert, const std::vector<Matrix4x4>& matrices) {
            if (mesh && shader && !matrices.empty()) {
                shader->Bind();
                glFrontFace(invert ? GL_CW : GL_CCW);
                mesh->DrawInstanced((int)matrices.size(), matrices);
            }
        };
        
        for (auto& cmd : m_commands) {
            // if element is not in renderlayers of the cam
            bool isInLayer = false;
            for (auto& layer : renderLayers) {
                if (layer == cmd.renderLayer)
                    isInLayer = true;
            }
            if (!isInLayer)
                continue;

            if (cmd.type == RenderCommandType::Text) {
                // Flush evtl. aktiven Mesh-Batch
                flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();
                // Text-Rendering
                unsigned int texID = FontManager::GetAtlasTextureID(cmd.fontID, cmd.pixelSize);
                glBindTexture(GL_TEXTURE_2D, texID);

                auto mat = rm.GetMaterial(cmd.materialID);
                // Shader aktivieren
                Shader* textShader = rm.GetShader(mat->GetShaderID());
                textShader->Bind();
                textShader->SetMatrix4("projection", cameraProjectionMat.ToOpenGLData());
                textShader->SetMatrix4("view", cameraViewMat.ToOpenGLData());
                textShader->SetVector3("textColor", cmd.textColor);

                if (cmd.modelMatrix)
                    textShader->SetMatrix4("model", cmd.modelMatrix->ToOpenGLData());

                // Alle Quads zeichnen
                for (const auto& quad : cmd.textQuads) {
                    // Position & UV-Daten uploaden
                    // -> z.B. in ein dynamisches VBO oder via glBufferSubData
                    // -> oder besser: vorbereitete Quad-Mesh-Struktur und pro Quad nur die Uniforms setzen
                    DrawQuad(quad); // musst du implementieren
                }
                continue;
            }

            // material change
            if (currentMaterialID != cmd.materialID) {
                flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();
        
                if (cmd.materialID.value == ENGINE_INVALID_ID) continue;
        
                currentMaterialID = cmd.materialID;
                currentMaterial = rm.GetMaterial(currentMaterialID);
                if (!currentMaterial) {
                    currentMaterialID.value = ENGINE_INVALID_ID;
                    continue;
                }
        
                Asset_ShaderID newShaderID = currentMaterial->GetShaderID();
                if (currentShaderID != newShaderID) {
                    currentShaderID = newShaderID;
                    currentShader = currentMaterial->BindToShader();
                    if (!currentShader) {
                        currentShaderID.value = ENGINE_INVALID_ID;
                        continue;
                    }
                    currentShader->SetMatrix4("projection", cameraProjectionMat.ToOpenGLData());
                    currentShader->SetMatrix4("view", cameraViewMat.ToOpenGLData());
                }
                else {
                    currentMaterial->ApplyParamsOnly(currentShader);
                }
            }
        
            if (currentMaterialID.value == ENGINE_INVALID_ID ||
                currentShaderID.value == ENGINE_INVALID_ID) {
                continue;
            }
        
            if (currentMeshID != cmd.meshID || 
                currentInvertMesh != cmd.invertMesh) {

                flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();
        
                currentMeshID = cmd.meshID;
                currentMesh = rm.GetMesh(currentMeshID);;
                currentInvertMesh = cmd.invertMesh;
        
                if (!currentMesh) {
                    currentMeshID.value = ENGINE_INVALID_ID;
                    continue;
                }
            }
        
            if (cmd.modelMatrix) {
                m_instanceMatrices.push_back(*cmd.modelMatrix);
            }
        }
        
        flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
        
        m_commands.clear();
        m_instanceMatrices.clear();
    }

}