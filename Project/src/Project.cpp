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
}

const float sensitivity = 0.1f;
Vector3 cameraPosition(0, 0, 5), cameraRotation(180, 5, 0);
float cameraSpeed = 10.0f;
float cameraSprintMultiplier = 1.8f;
float cameraSlowMultiplier = 0.4f;
float cameraSpeedVerMultiplier = 1.2f;
Vector2 lastFrameMousePos;
float cameraFOV = 66.0f;

unsigned int EBO, VBO, VAO;
Shader DefaultShader;
Texture2D texture1;
Matrix model, projection;
void Project::Start() {
	App_OpenGL_Set_BackgroundColor(0.2f, 0.3f, 0.3f);

	DefaultShader = Shader("assets\\shaders\\Default.vert", "assets\\shaders\\Default.frag");
	texture1.Create("assets\\textures\\stone.jpg das funktioniert save");
	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// Rückseite
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		// Vorderseite
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		// Linke Seite
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		// Rechte Seite
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 // Unterseite
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		 // Oberseite
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int indices[] = {
		1, 0, 3,
		3, 2, 1
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLsizei vertexSize = 5 * sizeof(float);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
	glEnableVertexAttribArray(0);
	// UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	{ 
		using namespace GLTransform;
		
		model = RotationXYZ(ConversionUtils::ToRadians(-55.0f), 0, 0);
		//projection = Orthographic(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 10.0f);
		projection = Perspective(ConversionUtils::ToRadians(cameraFOV), static_cast<float>(App_Application_Get_Window_Width() / App_Application_Get_Window_Height()), 0.1f, 100.0f);
	}

	App_Shader_Bind(&DefaultShader);
	Matrix view = GLTransform::LookAt(cameraPosition, cameraPosition + Vector3::back, Vector3::up);
	DefaultShader.SetMatrix4("view", view.ToOpenGLData());
	DefaultShader.SetInt("texture1", 0);

	lastFrameMousePos = Input::GetMousePosition();

	GameObject* go = GameObject::Create("New GameObject");
	Camera* c = go->AddComponent<Camera>();
	c->SetTest(10);
	Log::Info("Camera Test: {}, time {}", c->GetTest(), Time::GetTime());
	Camera* gc = go->GetComponent<Camera>();
	Log::Info("Camera Test: {}, time {}", gc->GetTest(), Time::GetTime());
}

Vector3 cubePositions[] = {
	Vector3(0.0f, 0.0f, 0.0f),
	Vector3(2.0f, 5.0f, -15.0f),
	Vector3(-1.5f, -2.2f, -2.5f),
	Vector3(-3.8f, -2.0f, -12.3f),
	Vector3(2.4f, -0.4f, -3.5f),
	Vector3(-1.7f, 3.0f, -7.5f),
	Vector3(1.3f, -2.0f, -2.5f),
	Vector3(1.5f, 2.0f, -2.5f),
	Vector3(1.5f, 0.2f, -1.5f),
	Vector3(-1.3f, 1.0f, -1.5f)
};

void DrawCube(int index);
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
		projection = GLTransform::Perspective(ConversionUtils::ToRadians(cameraFOV), static_cast<float>(App_Application_Get_Window_Width() / App_Application_Get_Window_Height()), 0.1f, 100.0f);
	}

	CameraMove();

	App_OpenGL_BackgroundColor();
	App_Shader_Bind(&DefaultShader);
	texture1.Bind(0);

	//DefaultShader.SetMatrix4("view", view.ToOpenGLData());
	DefaultShader.SetMatrix4("projection", projection.ToOpenGLData());
	glBindVertexArray(VAO);
	for (int i = 0; i < 10; i++) {
		DrawCube(i);
	}

	texture1.Unbind(0);
	glBindVertexArray(0);
}

void CameraMove() {
	static Vector3 lookDir;
	static bool first = true;
	Vector3 moveDir;
	float moveDirY = 0;

	if (Input::KeyPressed(GLFW_KEY_W)) moveDir.z += 1;
	if (Input::KeyPressed(GLFW_KEY_S)) moveDir.z -= 1;
	if (Input::KeyPressed(GLFW_KEY_A)) moveDir.x += 1;
	if (Input::KeyPressed(GLFW_KEY_D)) moveDir.x -= 1;
	if (Input::KeyPressed(GLFW_KEY_SPACE)) moveDirY += 1;
	if (Input::KeyPressed(GLFW_KEY_LEFT_CONTROL)) moveDirY -= 1;

	Vector2 mousePos = Input::GetMousePosition();
	Vector2 mouseDiff(
		mousePos.x - lastFrameMousePos.x,
		lastFrameMousePos.y - mousePos.y
	);
	if (moveDirY != 0 || moveDir.SquaredMagnitude() > 0 || mouseDiff.SquaredMagnitude() > 0 || first) {
		mouseDiff *= sensitivity;
		cameraRotation.y += mouseDiff.x; // yaw (horizontal)
		cameraRotation.x += mouseDiff.y; // pitch (vertikal)
		lastFrameMousePos = mousePos;

		MathUtil::Clamp(cameraRotation.x, -89.0f, 89.0f);

		lookDir.x = sin(ConversionUtils::ToRadians(cameraRotation.y)) * cos(ConversionUtils::ToRadians(cameraRotation.x));
		lookDir.y = sin(ConversionUtils::ToRadians(cameraRotation.x));
		lookDir.z = cos(ConversionUtils::ToRadians(cameraRotation.y)) * cos(ConversionUtils::ToRadians(cameraRotation.x));
		lookDir.Normalize();

		moveDir.Normalize();

		Vector3 right = lookDir.Cross(Vector3::up).Normalize();
		Vector3 up = right.Cross(lookDir).Normalize();

		Vector3 worldMoveDir =
			right * moveDir.x +
			up * moveDir.y +
			lookDir * moveDir.z;

		worldMoveDir.y += moveDirY * cameraSpeedVerMultiplier;

		float multiplier = (Input::KeyPressed(GLFW_KEY_LEFT_SHIFT)) ? cameraSprintMultiplier: 1;
		multiplier *= (Input::KeyPressed(GLFW_KEY_LEFT_ALT)) ? cameraSlowMultiplier : 1;
		cameraPosition += worldMoveDir * cameraSpeed * multiplier * Time::GetDeltaTime();

		Matrix view = GLTransform::LookAt(cameraPosition, cameraPosition + lookDir, Vector3::up);
		DefaultShader.SetMatrix4("view", view.ToOpenGLData());
	}

	first = false;
}

void DrawCube(int index) {
	using namespace GLTransform;
	
	Matrix mat = Translation(cubePositions[index]);
	float angle = ConversionUtils::ToRadians(20.0f * index);
	RotationXYZ(mat, angle, angle * 0.3f, angle * 0.5f);

	DefaultShader.SetMatrix4("model", mat.ToOpenGLData());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Project::Shutdown() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	App_Shader_Delete(&DefaultShader);
}

void Project::OnWindowResize(int newWidth, int newHeight) {
}

void Project::OnWindowFocusLost() {
}

void Project::OnWindowFocusGain() {
}