#include <algorithm>
#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include <CoreLib/Math/Vector3.h>

#include "EngineLib/ResourceManager.h"
#include "EngineLib/RenderLayerManager.h"
#include "EngineLib/Components/Camera_C.h"
#include "EngineLib/FontManager.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/Mesh.h"
#include "EngineLib/Material.h"
#include "EngineLib/Shader.h"
#include "EngineLib/Renderer.h"

namespace EngineCore {

    Renderer& Renderer::GetInstance() {
        static Renderer instance;
        return instance;
    }

    void Renderer::Submit(const RenderCommand& cmd) {
        if (cmd.renderLayerID.value == ENGINE_INVALID_ID)
            return;
        m_commands.push_back(cmd);
    }

    void Renderer::ReserveCommands(size_t count) {
        m_commands.reserve(count);
        m_instanceMatrices.reserve(count);
    }

    void Renderer::DrawAll() {
        /*
        * Render Order:
        * Draw all opaque objects first.
        * Sort all the transparent objects.
        * Draw all the transparent objects in sorted order.
        * 
        * Transparency dosent work correctly because i dont sort the faces.
        */
        if (m_commands.size() <= 0)
            return;

        std::shared_ptr<Component::Camera> camptr = GameObject::GetMainCamera();
        if (camptr->IsDisable()) {
            Log::Warn("Renderer: Cant render, main Camera is disabled!");
            return;
        }
        if (camptr->GetGameObject()->IsDisabled()) {
            Log::Warn("Renderer: Cant render, main Camera GameObject is disabled!");
            return;
        }
        Matrix4x4 cameraProjectionMat = camptr->GetProjectionMatrix();
        Matrix4x4 cameraViewMat = camptr->GetViewMatrix();
        const std::vector<RenderLayerID>& renderLayers = camptr->GetRenderLayers();

        Shader* currentShader = nullptr;
        ShaderID currentShaderID(ENGINE_INVALID_ID);
        Material* currentMaterial = nullptr;
        MaterialID currentMaterialID(ENGINE_INVALID_ID);
        Mesh* currentMesh = nullptr;
        MeshID currentMeshID(ENGINE_INVALID_ID);
        bool currentInvertMesh = m_commands[0].invertMesh;
        FontID currentFontID(ENGINE_INVALID_ID);
        int currentFontPixelSize = -1;
        int currentRenderLayerPriority = RenderLayerManager::GetRenderLayerPriority(m_commands[0].renderLayerID);
        int currentZOrder = m_commands[0].zOrder;

        ResourceManager* rm = ResourceManager::GetInstance();
        
        SortDrawCommands(camptr);

        auto flushBatch = [&](Mesh* mesh, Shader* shader, bool invert, const std::vector<Matrix4x4>& matrices) {
            if (mesh && shader && !matrices.empty()) {
                // shader->Bind(); probably fine
                glFrontFace(invert ? GL_CW : GL_CCW);
                mesh->DrawInstanced((int)matrices.size(), matrices);
            }
        };

        for (auto& cmd : m_commands) {
            // if element is not in renderlayers of the cam
            bool isInLayer = false;
            for (auto& layer : renderLayers) {
                if (layer == cmd.renderLayerID)
                    isInLayer = true;
            }
            if (!isInLayer)
                continue;

            int renderLayerPrio = RenderLayerManager::GetRenderLayerPriority(cmd.renderLayerID);
            // clear depth buffer if render prio changed
            if (currentRenderLayerPriority != renderLayerPrio) {
                currentRenderLayerPriority = renderLayerPrio;
                currentZOrder = cmd.zOrder;
                //draw the rest
                flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();

                glClear(GL_DEPTH_BUFFER_BIT);
            }// clear if z order changed
            else if (currentZOrder != cmd.zOrder) {
                currentZOrder = cmd.zOrder;
                //draw the rest
                flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();

                glClear(GL_DEPTH_BUFFER_BIT);
            }

            if (cmd.type == RenderCommandType::Text) {
                flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();

                // new font or new pixel size
                if (currentFontID != cmd.fontID || currentFontPixelSize != cmd.pixelSize) {
                    currentFontID = cmd.fontID;
                    currentFontPixelSize = cmd.pixelSize;
                    unsigned int texID = rm->GetFontAtlasTextureID(cmd.fontID, cmd.pixelSize);
                    // if valid texture
                    if (texID != ENGINE_INVALID_ID) {
                        glBindTexture(GL_TEXTURE_2D, texID);
                    }
                }

                currentMaterialID = cmd.materialID;
                currentMaterial = rm->GetMaterial(currentMaterialID);
                if (!currentMaterial) {
                    currentMaterialID.value = ENGINE_INVALID_ID;
                    continue;
                }

                ShaderID newShaderID = currentMaterial->GetShaderID();
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

                if (currentMaterialID.value == ENGINE_INVALID_ID ||
                    currentShaderID.value == ENGINE_INVALID_ID ||
                    currentShader == nullptr) {
                    continue;
                }

                currentShader->SetVector4("umeshColor", cmd.meshColor);
                if (cmd.modelMatrix)
                    currentShader->SetMatrix4("model", cmd.modelMatrix->ToOpenGLData());

                // if invert mesh changes 
                if (currentInvertMesh != cmd.invertMesh) {
                    currentInvertMesh = cmd.invertMesh;
                    glFrontFace(cmd.invertMesh ? GL_CW : GL_CCW);
                }

                for (const auto& quad : cmd.textQuads) {
                    FontManager::DrawQuad(quad);
                }
                continue;
            }

            // material change
            if (currentMaterialID != cmd.materialID) {
                flushBatch(currentMesh, currentShader, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();
        
                if (cmd.materialID.value == ENGINE_INVALID_ID) continue;
        
                currentMaterialID = cmd.materialID;
                currentMaterial = rm->GetMaterial(currentMaterialID);
                if (!currentMaterial) {
                    currentMaterialID.value = ENGINE_INVALID_ID;
                    continue;
                }
        
                ShaderID newShaderID = currentMaterial->GetShaderID();
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
                currentMesh = rm->GetMesh(currentMeshID);;
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

    void Renderer::SortDrawCommands(std::shared_ptr<Component::Camera> cameraPtr) {
        Vector3 camPos = cameraPtr->GetGameObject()->GetTransform()->GetWorldPosition();
        std::sort(m_commands.begin(), m_commands.end(),
            [&](const auto& a, const auto& b) {
                // Layer Priority
                int prioA = RenderLayerManager::GetRenderLayerPriority(a.renderLayerID);
                int prioB = RenderLayerManager::GetRenderLayerPriority(b.renderLayerID);
                if (prioA != prioB)
                    return prioA < prioB;

                // Sort after z order
                if (a.zOrder != b.zOrder)
                    return a.zOrder < b.zOrder;

                // Opaque: sort Material/Mesh
                if (!a.isTransparent && !b.isTransparent) {
                    if (a.materialID != b.materialID) return a.materialID < b.materialID;
                    if (a.meshID != b.meshID) return a.meshID < b.meshID;
                    return a.invertMesh < b.invertMesh;
                }

                // Transparent: sort Distance
                if (a.isTransparent && b.isTransparent) {
                    Vector3 posA = a.modelMatrix ? a.modelMatrix->GetTranslation() : Vector3::zero;
                    Vector3 posB = b.modelMatrix ? b.modelMatrix->GetTranslation() : Vector3::zero;
                    float distA = Vector3::SquaredDistance(camPos, posA);
                    float distB = Vector3::SquaredDistance(camPos, posB);
                    return distA > distB;
                }

                return a.isTransparent < b.isTransparent; // Opaque first
            }
        );
    }

}