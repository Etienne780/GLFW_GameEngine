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

const float sensitivity = 0.1f;
Vector3 cameraPosition(0, 0, 5), cameraRotation(0, 5, 0);
float cameraSpeed = 10.0f;
float cameraSprintMultiplier = 1.8f;
float cameraSlowMultiplier = 0.4f;
float cameraSpeedVerMultiplier = 1.2f;
float cameraFOV = 66.0f;

std::shared_ptr<GameObject> cameraGO = nullptr;
void Project::Start() {
	App_OpenGL_Set_BackgroundColor(0.2f, 0.3f, 0.3f);
	// App_OpenGL_Set_PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	App_OpenGL_Set_PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	App_OpenGL_Set_FaceCulling(true);

	cameraGO = GameObject::Create("MainCamera");
	auto cam = cameraGO->AddComponent<Component::Camera>();

	auto cubeGO = GameObject::Create("TestCube");
	auto mr = cubeGO->AddComponent<Component::MeshRenderer>();
	mr->SetMesh(ID::MESH::ENGINE::Cube()).SetMaterial(ID::MATERIAL::ENGINE::Default());

	cubeGO->GetTransform()->SetPosition(0, 0, 15);
	cubeGO->GetTransform()->SetScale(10, 10, 10);
}

void CameraMove();
float t = 0;
void Project::Update() {
	if (Input::KeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(App_Application_Get_Window(), true);
	
	if (Input::KeyJustPressed(GLFW_KEY_K)) {
		App_Application_Set_Window_Resizable(!App_Application_Get_Window_Resizable());
	}

	if (Input::KeyJustPressed(GLFW_KEY_L)) {
		App_Application_Set_Window_Floating(!App_Application_Get_Window_Floating());
	}

	int dir;
	if (Input::GetScrollDir(dir)) {
		cameraFOV -= static_cast<float>(dir) * 2;
		MathUtil::Clamp(cameraFOV, 1.0f, 120.0f);
		cameraGO->GetComponent<Component::Camera>()->SetFOV(cameraFOV);
	}

	CameraMove();
}

void CameraMove() {
	static Vector3 forward;
	static bool first = true;
	Vector3 moveDir;
	float moveDirY = 0;

	if (Input::KeyPressed(GLFW_KEY_W)) moveDir.z += 1;
	if (Input::KeyPressed(GLFW_KEY_S)) moveDir.z -= 1;
	if (Input::KeyPressed(GLFW_KEY_A)) moveDir.x += 1;
	if (Input::KeyPressed(GLFW_KEY_D)) moveDir.x -= 1;
	if (Input::KeyPressed(GLFW_KEY_SPACE)) moveDirY += 1;
	if (Input::KeyPressed(GLFW_KEY_LEFT_CONTROL)) moveDirY -= 1;

	Vector2 mouseDelta = Input::GetMousePositionDelta();

	if (moveDirY != 0 || moveDir.SquaredMagnitude() > 0 || mouseDelta.SquaredMagnitude() > 0 || first) {
		mouseDelta *= sensitivity;
		cameraRotation.y += mouseDelta.x; // yaw (horizontal)
		cameraRotation.x += mouseDelta.y; // pitch (vertikal)

		MathUtil::Clamp(cameraRotation.x, -89.0f, 89.0f);

		auto trans = cameraGO->GetTransform();
		forward = trans->GetForward();
		moveDir.Normalize();

		Vector3 right = trans->GetRight(forward);
		Vector3 up = trans->GetUp(forward, right);

		Vector3 worldMoveDir =
			right * moveDir.x +
			up * moveDir.y +
			forward * moveDir.z;

		worldMoveDir.y += moveDirY * cameraSpeedVerMultiplier;

		float multiplier = (Input::KeyPressed(GLFW_KEY_LEFT_SHIFT)) ? cameraSprintMultiplier: 1;
		multiplier *= (Input::KeyPressed(GLFW_KEY_LEFT_ALT)) ? cameraSlowMultiplier : 1;
		cameraPosition += worldMoveDir * cameraSpeed * multiplier * Time::GetDeltaTime();
		
		trans->SetPosition(cameraPosition);
		trans->SetRotation(cameraRotation);
	}

	first = false;
}

void Project::Shutdown() {
}

void Project::OnWindowResize(int newWidth, int newHeight) {
}

void Project::OnWindowFocusLost() {
}

void Project::OnWindowFocusGain() {
}