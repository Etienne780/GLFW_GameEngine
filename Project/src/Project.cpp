#include "Project.h"

using namespace EngineCore;

Project::Project()
	: Application("Project", "1.0.0") {

	App_Application_Set_Header(true);
	App_Application_Set_CloseAppOnWindowClose(true);
	App_Application_Set_Window_Resizable(true);
	App_Application_Set_Window_Decoration(true);
	App_Application_Set_Window_Floating(false);
	App_Application_Set_Window_Cursor_LockHidden(true);
	App_Application_Set_Header(true);

	App_OpenGL_Set_Version(3, 3);

	// Log::SaveLogs("Logs/");
}

void GenerateCubesSphere();

std::shared_ptr<Component::FreeCameraController> camController = nullptr;
std::shared_ptr<Component::Transform> containerTrans = nullptr;

size_t cubeCountTheta = 20; // horizontale Segmente
size_t cubeCountPhi = 20;   // vertikale Segmente
float sphereRadius = 50.0f;
void Project::Start() {
	App_OpenGL_Set_DepthTesting(true);
	App_OpenGL_Set_BackgroundColor(0.2f, 0.3f, 0.3f);
	// App_OpenGL_Set_PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	App_OpenGL_Set_FaceCulling(true);

	auto cameraGO = GameObject::Create("MainCamera");
	cameraGO->SetPersistent(true);
	auto cam = cameraGO->AddComponent<Component::Camera>();
	camController = cameraGO->AddComponent<Component::FreeCameraController>();

	GenerateCubesSphere();
}

void GenerateCubesSphere() {
	auto container = GameObject::Create("Container");
	containerTrans = container->GetTransform();

	// ResourceManager& rm = ResourceManager::GetInstance();
	// auto mat = rm.GetMaterial(ID::MATERIAL::ENGINE::Default());
	// mat->SetParam("texture", ID::TEXTURE::ENGINE::Cursedmod3());

	for (size_t i = 0; i < cubeCountTheta; ++i) {
		float theta = static_cast<float>((static_cast<float>(i) / cubeCountTheta) * 2.0f * CORE_PI);

		for (size_t j = 0; j < cubeCountPhi; ++j) {
			float phi = static_cast<float>((static_cast<float>(j) / cubeCountPhi) * CORE_PI);

			float x = sphereRadius * sin(phi) * cos(theta);
			float y = sphereRadius * cos(phi);
			float z = sphereRadius * sin(phi) * sin(theta);

			auto cubeGO = GameObject::Create(FormatUtils::formatString("Cube_{}_{}", i, j));
			cubeGO->SetParent(container);
			auto mr = cubeGO->AddComponent<Component::MeshRenderer>();
			mr->SetMesh(ID::MESH::ENGINE::Cube())->SetMaterial(ID::MATERIAL::ENGINE::Default());

			cubeGO->GetTransform()->SetPosition(x, y, z);
			cubeGO->GetTransform()->SetScale(5, 5, 5);
		}
	}
}

void UpdateCubesSphere(float time) {
	containerTrans->SetRotation(time, time * 1.235f, 0);
}

void Project::Update() {
	if (Input::KeyPressed(KeyCode::ESCAPE))
		glfwSetWindowShouldClose(App_Application_Get_Window(), true);

	if (Input::KeyJustPressed(KeyCode::L)) {
		App_Application_Set_Window_Floating(!App_Application_Get_Window_Floating());
	}

	if (Input::KeyJustPressed(KeyCode::H)) {
		App_Debug_Set_Active(!App_Debug_Get_Active());
	}

	//UpdateCubesSphere(Time::GetTime());
}

void Project::Shutdown() {
}

void Project::OnWindowResize(int newWidth, int newHeight) {
}

void Project::OnWindowFocusLost() {
}

void Project::OnWindowFocusGain() {
}