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
        m_commands.push_back(cmd);
    }

    void Renderer::DrawAll() {
        // 1. Sortiere die Commands nach Material/Shader
        //std::sort(m_commands.begin(), m_commands.end(), [](const auto& a, const auto& b) {
        //    return a.material->GetShaderID() < b.material->GetShaderID();
        //    });

        Shader* currentShader = nullptr;
        unsigned int currentShaderID = ENGINE_INVALID_ID;
        Material* currentMaterial = nullptr;
        unsigned int currentMaterialID = ENGINE_INVALID_ID;
        Mesh* currentMesh = nullptr;
        unsigned int currentMeshID = ENGINE_INVALID_ID;

        ResourceManager& rm = ResourceManager::GetInstance();

        std::shared_ptr<Component::Camera> camptr = GameObject::GetMainCamera().lock();
        Matrix cameraProjectionMat = camptr->GetProjectionMatrix();
        Matrix cameraViewMat = camptr->GetViewMatrix();

        for (auto& cmd : m_commands) {

            // Update material if change
            if (currentMaterialID != cmd.materialID) {
                if (cmd.materialID == ENGINE_INVALID_ID) {
                    // should Load default mat her
                    continue;
                }

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
            
            if (currentMeshID != cmd.meshID) {
                currentMeshID = cmd.meshID;
                currentMesh = rm.GetMesh(currentMeshID);

                if (!currentMesh) {
                    currentMeshID = ENGINE_INVALID_ID;
                    continue;
                }
            }

            currentShader->SetMatrix4("model", cmd.modelMatrixOpenGL);
            glFrontFace(cmd.invertMesh ? GL_CW : GL_CCW);
            currentShader->Bind();
            currentMesh->Draw();
        }

        m_commands.clear();
    }

}