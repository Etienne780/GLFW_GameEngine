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

	auto container = Style::Create("Container");
	container->Set(Attribute::layoutType, "flex");
	container->Set(Attribute::layout, "center");
	container->Set(Attribute::width, "100%w");
	container->Set(Attribute::height, "100%h");
	// container->Set(Attribute::visibility, "hidden");

	auto defaultStyleRow = Style::Create("DefaultStyleRow");
	defaultStyleRow->Set(Attribute::layoutDirection, "row");
	defaultStyleRow->Set(Attribute::layout, "center center");
	defaultStyleRow->Set(Attribute::width, "50%w");
	defaultStyleRow->Set(Attribute::height, "50%h");
	defaultStyleRow->Set(Attribute::backgroundColor, "#000000a0");
	defaultStyleRow->Set(State::Hovered, Attribute::backgroundColor, "#000000e0");

	auto defaultStyleColumn = Style::Create("DefaultStyleColumn", defaultStyleRow);
	defaultStyleColumn->Set(Attribute::layoutDirection, "column");
	defaultStyleColumn->Set(Attribute::backgroundColor, "#101010A0");

	auto style1 = Style::Create("style1");
	style1->Set(Attribute::width, "50%a");
	style1->Set(Attribute::height, "50%a");
	style1->Set(Attribute::backgroundColor, "#8f0000");
	style1->Set(State::Hovered, Attribute::backgroundColor, "#ff0000");

	auto style2 = Style::Create("style2", style1);
	style2->Set(Attribute::backgroundColor, "#008f00");
	style2->Set(State::Hovered, Attribute::backgroundColor, "#00ff00");

	auto style3 = Style::Create("style3", style1);
	style3->Set(Attribute::backgroundColor, "#00008f");
	style3->Set(State::Hovered, Attribute::backgroundColor, "#0000ff");

	auto style4 = Style::Create("style4", style1);
	style4->Set(Attribute::backgroundColor, "#b0b000");
	style4->Set(State::Hovered, Attribute::backgroundColor, "#ffff00");

	UI::SetDebug(true);
	UI::Begin<Panel>(container); {

		// Row-layout
		UI::Begin<Panel>(defaultStyleRow); {
		
			UI::Add<Panel>(style1);
			UI::Add<Panel>(style2);
			UI::Add<Panel>(style3);
			UI::Add<Panel>(style4);
		
		}
		UI::End();
		 
		// Column-layout
		UI::Begin<Panel>(defaultStyleColumn); {
		
			UI::Add<Panel>(style1);
			UI::Add<Panel>(style2);
			UI::Add<Panel>(style3);
			UI::Add<Panel>(style4);
		}
		UI::End();

	}
	UI::End();
	Log::Print(Log::levelDebug);
	Log::Print(Log::levelDebug, UI::GetUIHierarchyString());
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