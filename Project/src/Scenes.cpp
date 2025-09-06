#include <EngineLib/Engine.h>

#include "Scenes.h"

namespace Scenes {
	
	void Test() {
		using namespace EngineCore;

		ResourceManager& rm = ResourceManager::GetInstance();
		auto matID = rm.AddMaterial(ASSETS::ENGINE::SHADER::Default());
		auto mat = rm.GetMaterial(matID);
		mat->SetParam("texture", ASSETS::ENGINE::TEXTURE::Cursedmod3());

		auto go = GameObject::Create("box");
		go->GetTransform()->SetScale(20, 20, 20);
		auto mr = go->AddComponent<Component::MeshRenderer>();
		mr->SetMesh(ASSETS::ENGINE::MESH::Cube())->SetMaterial(matID);
	}

}