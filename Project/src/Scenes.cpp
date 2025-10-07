#include <EngineLib/Engine.h>

#include "CustomComponents.h"
#include "Scenes.h"

using namespace EngineCore;
RenderLayerID uiLayer(ENGINE_INVALID_ID);

void SetupUI() {
	using namespace EngineCore::UI;
	using UI = UIManager;
	uiLayer = RenderLayerManager::AddLayer("UILayer", 10);
	UI::SetUIRenderLayer(uiLayer);

	auto defaultStyle = Style::Create("DefaultStyle");
	defaultStyle->Set(Attribute::layoutDirection, "row");
	defaultStyle->Set(Attribute::layout, "center center");
	defaultStyle->Set(Attribute::width, "50%w");
	defaultStyle->Set(Attribute::height, "50%h");
	defaultStyle->Set(Attribute::backgroundColor, "#000000");

	auto style1 = Style::Create("style1");
	style1->Set(Attribute::width, "50%a");
	style1->Set(Attribute::height, "50%a");
	style1->Set(Attribute::backgroundColor, "#ff0000");

	auto style2 = Style::Create("style2", style1);
	style2->Set(Attribute::borderColor, "#00ff00");
	style2->Set(Attribute::backgroundColor, "#00ff00");

	auto style3 = Style::Create("style3", style2);
	style3->Set(Attribute::backgroundColor, "#0000ff");

	auto style4 = Style::Create("style4", style1);
	style4->Set(Attribute::backgroundColor, "#ffff00");

	UI::SetDebug(false);
	UI::Begin<Panel>(defaultStyle); {

		UI::Add<Panel>(style1);
		UI::Add<Panel>(style2);
		UI::Add<Panel>(style3);
		UI::Add<Panel>(style4);
	}
	UI::End();
	Log::Print();
	Log::Print(UI::GetUIHierarchyString());
}

void SetupGame() {
	SetupUI();
}

namespace Scenes {

	void LoadTest() {

		auto cameraGO = GameObject::Create("MainCamera");
		cameraGO->SetPersistent(true);
		cameraGO->AddComponent<Component::Camera>()->AddCameraLayer(uiLayer);
		cameraGO->AddComponent<Component::FreeCameraController>();

		auto fontGO = GameObject::Create("FontTest");
		fontGO->GetTransform()->SetPosition(0,20,0);
		auto tr = fontGO->AddComponent<Component::TextRenderer>();
		tr->SetText("Das ist ein Test");

		ResourceManager* rm = ResourceManager::GetInstance();
		auto transparentWinTextureID = rm->AddTexture2DFromFile("assets/blending_transparent_window.png");

		auto matID = rm->AddMaterial(ASSETS::ENGINE::SHADER::Default());
		auto mat = rm->GetMaterial(matID);
		mat->SetParam("texture", transparentWinTextureID);

		auto go = GameObject::Create("box");
		go->GetTransform()->SetScale(20, 20, 20);
		auto mr = go->AddComponent<Component::MeshRenderer>();
		mr->SetMesh(ASSETS::ENGINE::MESH::Cube())->SetMaterial(ASSETS::ENGINE::MATERIAL::Default());
		go->AddComponent<Component::Rainbow>();

		go = GameObject::Create("plain");
		go->GetTransform()->SetPosition(30, 0, 0);
		go->GetTransform()->SetScale(20, 20, 20);
		mr = go->AddComponent<Component::MeshRenderer>();
		mr->SetMesh(ASSETS::ENGINE::MESH::Plain())->SetMaterial(matID);

		go = GameObject::Create("sprite test");
		go->GetTransform()->SetPosition(60, 0, 0);
		go->GetTransform()->SetScale(20, 20, 20);
		auto sr = go->AddComponent<Component::SpriteRenderer>();
		sr->SetSprite(ASSETS::ENGINE::TEXTURE::Cursedmod3());

		go = GameObject::Create(FormatUtils::formatString("Opaque box"));
		go->GetTransform()->SetPosition(-30.0f, 0.0f, 90.0f);
		go->GetTransform()->SetScale(20, 20, 20);
		mr = go->AddComponent<Component::MeshRenderer>();
		mr->SetMesh(ASSETS::ENGINE::MESH::Cube())->SetMaterial(ASSETS::ENGINE::MATERIAL::Default());
		
		auto containerG0 = GameObject::Create("container");
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				auto child = GameObject::Create(FormatUtils::formatString("box_{}_{}", i, j));
				child->SetParent(containerG0);
				child->GetTransform()->SetPosition(30.0f + static_cast<float>(j) * 20.0f - 20.0f, static_cast<float>(i) * 20.0f - 20.0f, 90.0f);
				child->GetTransform()->SetScale(20, 20, 20);
				auto childMR = child->AddComponent<Component::MeshRenderer>();
				childMR->SetMesh(ASSETS::ENGINE::MESH::Cube())->SetMaterial(matID);
			}
		}

		Texture2DID id = rm->AddTexture2DFromFile("assets/fish.png");
		go = GameObject::Create("fish spirte");
		go->GetTransform()->SetPosition(30, 0, 200);
		go->GetTransform()->SetScale(50, 50, 50);
		sr = go->AddComponent<Component::SpriteRenderer>();
		sr->SetSprite(id);

		id = rm->AddTexture2DFromFile("assets/graTest.png");
		go = GameObject::Create("graPNG spirte");
		go->GetTransform()->SetPosition(90, 0, 200);
		go->GetTransform()->SetScale(50, 50, 50);
		sr = go->AddComponent<Component::SpriteRenderer>();
		sr->SetSprite(id);

		id = rm->AddTexture2DFromFile("assets/graTest.jpg");
		go = GameObject::Create("graJPG spirte");
		go->GetTransform()->SetPosition(-30, 0, 200);
		go->GetTransform()->SetScale(50, 50, 50);
		sr = go->AddComponent<Component::SpriteRenderer>();
		sr->SetSprite(id);
	}

}