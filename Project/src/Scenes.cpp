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
	defaultStyle->Set(Attribute::borderRadius, "20px 30px 40px 10px");
	defaultStyle->Set(Attribute::borderColor, "#00ff00");
	defaultStyle->Set(Attribute::width, "50%w");
	defaultStyle->Set(Attribute::height, "50%h");
	defaultStyle->Set(Attribute::backgroundColor, "#00ffff");
	defaultStyle->Set(State::Hovered, Attribute::backgroundColor, "#880088");
	defaultStyle->Set(State::Hovered, Attribute::borderRadius, "50px");
	defaultStyle->Set(State::Pressed, Attribute::backgroundColor, "#0000ff");

	auto style2 = Style::Create("style2");
	style2->Set(Attribute::width, "30%w");
	style2->Set(Attribute::height, "30%h");
	style2->Set(Attribute::backgroundColor, "#0000ff");

	auto otherStyle = Style::Create("OtherStyle");
	otherStyle->Set(Attribute::backgroundColor,"#ff0000");
	otherStyle->Set(Attribute::width, "90%w");
	otherStyle->Set(Attribute::height, "90%h");

	auto otherStyle2 = Style::Create("OtherStyle2");
	otherStyle2->Set(Attribute::backgroundColor, "#ffff00");
	otherStyle2->Set(Attribute::width, "100%a");
	otherStyle2->Set(Attribute::height, "100%a");

	// UI::SetDebug(false);
	UI::Begin<Panel>(defaultStyle); {

		UI::Begin<Panel>(style2); {
			UI::Add<Panel>(otherStyle)->SetOnClick([] { Log::Info("Pressed"); });
		}
		UI::End();

		UI::Add<Panel>(otherStyle2);

		// UI::Begin<Panel>(style2); {
		// 	UI::Add<Panel>(otherStyle)->SetOnClick([] { Log::Info("Pressed"); });
		// }
		// UI::End();
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