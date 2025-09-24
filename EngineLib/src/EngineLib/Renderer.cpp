#include <algorithm>
#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include <CoreLib/Log.h>
#include <CoreLib/Math/Vector3.h>
#include <CoreLib/Math/Vector4.h>

#include "EngineLib/Time.h"
#include "EngineLib/ResourceManager.h"
#include "EngineLib/RenderLayerManager.h"
#include "EngineLib/Components/Camera_C.h"
#include "EngineLib/FontManager.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/Mesh.h"
#include "EngineLib/Material.h"
#include "EngineLib/Shader.h"
#include "EngineLib/ShaderBindObject.h"
#include "EngineLib/Renderer.h"

namespace EngineCore {

    Renderer* Renderer::GetInstance() {
        static Renderer instance;
        return &instance;
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
        ShaderBindObject* currentOverrideShaderBindObj = nullptr;
        Mesh* currentMesh = nullptr;
        MeshID currentMeshID(ENGINE_INVALID_ID);
        bool currentInvertMesh = m_commands[0].invertMesh;
        FontID currentFontID(ENGINE_INVALID_ID);
        int currentFontPixelSize = -1;
        int currentRenderLayerPriority = RenderLayerManager::GetLayerPriority(m_commands[0].renderLayerID);
        int currentZOrder = m_commands[0].zOrder;
        Vector4 currentMeshColor(-1,-1, -1, -1);

        ResourceManager* rm = ResourceManager::GetInstance();
        
        SortDrawCommands(camptr);

        auto flushBatch = [&](Mesh* mesh, Shader* shader, ShaderBindObject* shaderBindObjOverride, bool invert, const std::vector<Matrix4x4>& matrices) {
            if (mesh && shader && !matrices.empty()) {
                // shader->Bind(); probably fine
                if (shaderBindObjOverride)
                    shaderBindObjOverride->Bind(shader);
                glFrontFace(invert ? GL_CW : GL_CCW);
                mesh->DrawInstanced((int)matrices.size(), matrices);
            }
        };

        for (auto& cmd : m_commands) {
            // if element is not in renderlayers of the cam inefficient
            bool isInLayer = false;
            for (auto& layer : renderLayers) {
                if (layer == cmd.renderLayerID)
                    isInLayer = true;
            }
            if (!isInLayer)
                continue;

            int renderLayerPrio = RenderLayerManager::GetLayerPriority(cmd.renderLayerID);
            // clear depth buffer if render prio changed
            if (currentRenderLayerPriority != renderLayerPrio) {
                currentRenderLayerPriority = renderLayerPrio;
                currentZOrder = cmd.zOrder;
                //draw the rest
                flushBatch(currentMesh, currentShader, currentOverrideShaderBindObj, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();

                glClear(GL_DEPTH_BUFFER_BIT);
            }// clear if z order changed
            else if (currentZOrder != cmd.zOrder) {
                currentZOrder = cmd.zOrder;
                //draw the rest
                flushBatch(currentMesh, currentShader, currentOverrideShaderBindObj, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();

                glClear(GL_DEPTH_BUFFER_BIT);
            }

            if (cmd.type == RenderCommandType::Text) {
                flushBatch(currentMesh, currentShader, currentOverrideShaderBindObj, currentInvertMesh, m_instanceMatrices);
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
                flushBatch(currentMesh, currentShader, currentOverrideShaderBindObj, currentInvertMesh, m_instanceMatrices);
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

            // shader bind obj override
            if (currentOverrideShaderBindObj != cmd.shaderBindOverride) {
                flushBatch(currentMesh, currentShader, currentOverrideShaderBindObj, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();

                currentOverrideShaderBindObj = cmd.shaderBindOverride;
            }

            if (currentMaterialID.value == ENGINE_INVALID_ID ||
                currentShaderID.value == ENGINE_INVALID_ID ||
                currentShader == nullptr) {
                continue;
            }

            // if mesh color changes
            if (currentMeshColor.x == -1 || currentMeshColor.SquaredMagnitude() != cmd.meshColor.SquaredMagnitude()) {
                currentMeshColor = cmd.meshColor;

                flushBatch(currentMesh, currentShader, currentOverrideShaderBindObj, currentInvertMesh, m_instanceMatrices);
                m_instanceMatrices.clear();
                currentShader->SetVector4("umeshColor", currentMeshColor);
            }

            if (currentMeshID != cmd.meshID || 
                currentInvertMesh != cmd.invertMesh) {

                flushBatch(currentMesh, currentShader, currentOverrideShaderBindObj, currentInvertMesh, m_instanceMatrices);
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
        
        flushBatch(currentMesh, currentShader, currentOverrideShaderBindObj, currentInvertMesh, m_instanceMatrices);
        
        PrintCommands(true);
        m_commands.clear();
        m_instanceMatrices.clear();
    }

    void Renderer::SortDrawCommands(std::shared_ptr<Component::Camera> cameraPtr) {
        Vector3 camPos = cameraPtr->GetGameObject()->GetTransform()->GetWorldPosition();
        std::sort(m_commands.begin(), m_commands.end(),
            [&](const auto& a, const auto& b) {
                // Layer Priority
                int prioA = RenderLayerManager::GetLayerPriority(a.renderLayerID);
                int prioB = RenderLayerManager::GetLayerPriority(b.renderLayerID);
                if (prioA != prioB)
                    return prioA < prioB;

                // Sort after z order
                if (a.zOrder != b.zOrder)
                    return a.zOrder < b.zOrder;

                // Opaque: sort Material/Mesh
                if (!a.isTransparent && !b.isTransparent) {
                    if (a.materialID != b.materialID) return a.materialID < b.materialID;
                    if (a.meshID != b.meshID) return a.meshID < b.meshID;

                    if (a.shaderBindOverride != b.shaderBindOverride)
                        return a.shaderBindOverride == nullptr;

                    float aSQM = a.meshColor.SquaredMagnitude();
                    float bSQM = b.meshColor.SquaredMagnitude();
                    if (aSQM != bSQM)
                        return aSQM > bSQM;
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

    void Renderer::PrintCommands(bool displayOption) {
        Log::Print("");
        Log::Info("Frame {} Render-Commands", Time::GetFrameCount());

        bool printOpaqueHeader = false;
        bool printTransHeader = false;
        int counter = 0;
        for (auto& cmd : m_commands) {
            // prints header
            if (!cmd.isTransparent) {
                if (!printOpaqueHeader) {
                    Log::Print("Opaque:");
                    printOpaqueHeader = true;
                }
            }
            else {
                if (!printTransHeader) {
                    Log::Print("Transparent:");
                    printTransHeader = true;
                }
            }

            if (displayOption) {
                Log::Print(Log::levelInfo, "      - Cmd {}: Type: {}, RenLayID: {}, ZOr: {}, MatID: {}, MeshID: {}, FontID: {}, OSBindObj: {}, IsTrans: {}, InvMesh: {}, MeshCol: {}, FontSize: {}",
                    counter,
                    RenderCommandTypeToString(cmd.type),
                    cmd.renderLayerID.value,
                    cmd.zOrder,
                    (cmd.materialID.value != ENGINE_INVALID_ID) ? std::to_string(cmd.materialID.value) : "-",
                    (cmd.meshID.value != ENGINE_INVALID_ID) ? std::to_string(cmd.meshID.value) : "-",
                    (cmd.fontID.value != ENGINE_INVALID_ID) ? std::to_string(cmd.fontID.value) : "-",
                    (cmd.shaderBindOverride != nullptr),
                    cmd.isTransparent,
                    cmd.invertMesh,
                    cmd.meshColor.ToString(),
                    cmd.pixelSize);
            }
            else {
                Log::Print(Log::levelInfo, "Command {}:", counter);
                Log::Print(Log::levelInfo, "      - Type: {}", RenderCommandTypeToString(cmd.type));
                Log::Print(Log::levelInfo, "      - Render-Layer ID: {}", cmd.renderLayerID.value);
                Log::Print(Log::levelInfo, "      - Z-Order: {}", cmd.zOrder);
                Log::Print(Log::levelInfo, "      - Material ID: {}", (cmd.materialID.value != ENGINE_INVALID_ID) ? std::to_string(cmd.materialID.value) : "-");
                Log::Print(Log::levelInfo, "      - Mesh ID: {}", (cmd.meshID.value != ENGINE_INVALID_ID) ? std::to_string(cmd.meshID.value) : "-");
                Log::Print(Log::levelInfo, "      - Font ID: {}", (cmd.fontID.value != ENGINE_INVALID_ID) ? std::to_string(cmd.fontID.value) : "-");
                Log::Print(Log::levelInfo, "      - Override ShaderBindObj ID: {}", (cmd.shaderBindOverride != nullptr));
                Log::Print(Log::levelInfo, "      - Is transparent: {}", cmd.isTransparent);
                Log::Print(Log::levelInfo, "      - Invert mesh: {}", cmd.invertMesh);
                Log::Print(Log::levelInfo, "      - Mesh color: {}", cmd.meshColor.ToString());
                Log::Print(Log::levelInfo, "      - Font pixel size: {}", cmd.pixelSize);
            }
            counter++;
        }
    }

}