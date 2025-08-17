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

	App_OpenGL_Set_Version(3, 3);
	App_OpenGL_Set_DepthTesting(true);

	App_Application_Set_Header(true);

	// Log::SaveLogs("Logs/");
}

std::shared_ptr<GameObject> cameraGO = nullptr;
std::vector<std::shared_ptr<GameObject>> cubes;
size_t cubeCountTheta = 20; // horizontale Segmente
size_t cubeCountPhi = 20;   // vertikale Segmente
void Project::Start() {
	App_OpenGL_Set_BackgroundColor(0.2f, 0.3f, 0.3f);
	// App_OpenGL_Set_PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	App_OpenGL_Set_FaceCulling(true);

	cameraGO = GameObject::Create("MainCamera");
	auto cam = cameraGO->AddComponent<Component::Camera>();
	cameraGO->AddComponent<Component::CameraMovement>();

	auto c = GameObject::Create("TestCube");
	auto mr = c->AddComponent<Component::MeshRenderer>();
	mr->SetMesh(ID::MESH::ENGINE::Cube()).SetMaterial(ID::MATERIAL::ENGINE::Default());
	c->GetTransform()->SetScale(10, 10, 10);

	float radius = 100.0f;

	for (size_t i = 0; i < cubeCountTheta; ++i) {
		float theta = static_cast<float>((static_cast<float>(i) / cubeCountTheta) * 2.0f * CORE_PI);

		for (size_t j = 0; j < cubeCountPhi; ++j) {
			float phi = static_cast<float>((static_cast<float>(j) / cubeCountPhi) * CORE_PI);

			float x = radius * sin(phi) * cos(theta);
			float y = radius * cos(phi);
			float z = radius * sin(phi) * sin(theta);

			auto cubeGO = GameObject::Create(FormatUtils::formatString("Cube_{}_{}", i, j));
			auto mr = cubeGO->AddComponent<Component::MeshRenderer>();
			mr->SetMesh(ID::MESH::ENGINE::Cube()).SetMaterial(ID::MATERIAL::ENGINE::Default());

			cubeGO->GetTransform()->SetPosition(x, y, z);
			cubeGO->GetTransform()->SetScale(5, 5, 5);

			cubes.push_back(std::move(cubeGO));
		}
	}
}

void UpdateCubes(std::vector<std::shared_ptr<GameObject>>& cubes, float time, size_t thetaCount, size_t phiCount) {
	float radius = 100.0f;
	size_t index = 0;

	for (size_t i = 0; i < thetaCount; ++i) {
		float theta = static_cast<float>((static_cast<float>(i) / thetaCount) * 2.0f * CORE_PI + time);

		for (size_t j = 0; j < phiCount; ++j) {
			float phi = static_cast<float>((static_cast<float>(j) / phiCount) * CORE_PI);

			float x = radius * sin(phi) * cos(theta);
			float y = radius * cos(phi);
			float z = radius * sin(phi) * sin(theta);

			cubes[index++]->GetTransform()->SetPosition(x, y, z);
		}
	}
}

void Project::Update() {
	if (Input::KeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(App_Application_Get_Window(), true);
	
	if (Input::KeyJustPressed(GLFW_KEY_K)) {
		App_Application_Set_Window_Resizable(!App_Application_Get_Window_Resizable());
	}

	if (Input::KeyJustPressed(GLFW_KEY_L)) {
		App_Application_Set_Window_Floating(!App_Application_Get_Window_Floating());
	}

	UpdateCubes(cubes, Time::GetTime(), cubeCountTheta, cubeCountPhi);

	Log::Info("FPS: {}", App_Application_Get_FramesPerSecond());
}

void Project::Shutdown() {
}

void Project::OnWindowResize(int newWidth, int newHeight) {
}

void Project::OnWindowFocusLost() {
}

void Project::OnWindowFocusGain() {
}