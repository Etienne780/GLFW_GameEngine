#include <algorithm>
#include <glad\glad.h>

#include "EngineLib\ResourceManager.h"
#include "EngineLib\Components\Camera_C.h"
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
        if (cmd.renderLayer == -1)
            return;
        m_commands.push_back(cmd);
    }

    void Renderer::ReserveCommands(size_t count) {
        m_commands.reserve(count);
        m_instanceMatrices.reserve(count);
    }

    void Renderer::DrawAll() {

        std::sort(m_commands.begin(), m_commands.end(), [](const auto& a, const auto& b) {
            if (a.materialID != b.materialID) return a.materialID < b.materialID;
            if (a.meshID != b.meshID) return a.meshID < b.meshID;
            return a.invertMesh < b.invertMesh;
        });
        
        Shader* currentShader = nullptr;
        unsigned int currentShaderID = ENGINE_INVALID_ID;
        Material* currentMaterial = nullptr;
        unsigned int currentMaterialID = ENGINE_INVALID_ID;
        Mesh* currentMesh = nullptr;
        unsigned int currentMeshID = ENGINE_INVALID_ID;
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
        const std::vector<unsigned int>& renderLayers = camptr->GetRenderLayers();
        
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

            // material change
            if (currentMaterialID != cmd.materialID) {
                flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();
        
                if (cmd.materialID == ENGINE_INVALID_ID) continue;
        
                currentMaterialID = cmd.materialID;
                currentMaterial = rm.GetMaterial(currentMaterialID);
                if (!currentMaterial) {
                    currentMaterialID = ENGINE_INVALID_ID;
                    continue;
                }
        
                unsigned int newShaderID = currentMaterial->GetShaderID();
                if (currentShaderID != newShaderID) {
                    currentShaderID = newShaderID;
                    currentShader = currentMaterial->BindToShader();
                    if (!currentShader) {
                        currentShaderID = ENGINE_INVALID_ID;
                        continue;
                    }
                    currentShader->SetMatrix4("projection", cameraProjectionMat.ToOpenGLData());
                    currentShader->SetMatrix4("view", cameraViewMat.ToOpenGLData());
                }
                else {
                    currentMaterial->ApplyParamsOnly(currentShader);
                }
            }
        
            if (currentMaterialID == ENGINE_INVALID_ID ||
                currentShaderID == ENGINE_INVALID_ID) {
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
                    currentMeshID = ENGINE_INVALID_ID;
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