#include "MyGameApp.h"

using namespace EngineCore;

MyGameApp::MyGameApp() 
	: Application("MyGameApp", "1.0.0") {

	App_Application_Set_Header(true);
	App_Application_Set_CloseAppOnWindowClose(true);
	App_Application_Set_Window_Resizable(true);
	App_Application_Set_Window_Decoration(true);
	App_Application_Set_Window_Floating(true);

	App_OpenGL_Set_Version(3, 3);
}

unsigned int EBO, VBO, VAO;
Shader DefaultShader;
Texture2D texture1;
Matrix model, view, projection;
Vector3 modelPos;
void MyGameApp::OnStart() {
	App_OpenGL_Set_BackgroundColor(0.2f, 0.3f, 0.3f);
	App_OpenGL_Set_DepthTesting(true);

	DefaultShader = Shader("shader/Default.vert", "shader/Default.frag");
	texture1.Create("assets/stone.jpg das funktioniert save");
	
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
		
		modelPos.Set(0, 0, -5);
		model = RotationXYZ(ConversionUtils::ToRadians(-55.0f), 0, 0);
		Translation(model, modelPos);
		view = Translation(0, 0, -3.0f);
		//projection = Orthographic(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 10.0f);
		projection = Perspective(ConversionUtils::ToRadians(45.0f), static_cast<float>(App_Application_Get_Window_Width() / App_Application_Get_Window_Height()), 0.1f, 100.0f);
	}

	App_Shader_Bind(&DefaultShader);
	DefaultShader.SetInt("texture1", 0);
}

struct Vertex {
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
};
float t = 0;

void MyGameApp::OnUpdate() {
	if (Input::KeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(App_Application_Get_Window(), true);

	if (Input::KeyJustPressed(GLFW_KEY_J)) {
		App_Application_Set_Window_Resizable(!App_Application_Get_Window_Resizable());
	}

	if (Input::KeyJustPressed(GLFW_KEY_K)) {
		App_Application_Set_Window_Visibility(!App_Application_Get_Window_Visibility());
	}

	if (Input::KeyJustPressed(GLFW_KEY_L)) {
		App_Application_Set_Window_Floating(!App_Application_Get_Window_Floating());
	}

	App_OpenGL_BackgroundColor();
	App_Shader_Bind(&DefaultShader);

	{
		using namespace GLTransform;

		t += 0.7f * Time::GetDeltaTimeSec();
		modelPos.x = sin(t);
		Vector3 rotation(
			ConversionUtils::ToRadians(t * 80),
			ConversionUtils::ToRadians(t * 80),
			0
		);

		Matrix mat = Identity();
		RotationXYZ(mat, rotation);
		Translation(mat, modelPos);

		model = mat;
	}

	texture1.Bind(0);
	DefaultShader.SetMatrix4("model", model.ToOpenGLData());
	DefaultShader.SetMatrix4("view", view.ToOpenGLData());
	DefaultShader.SetMatrix4("projection", projection.ToOpenGLData());
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	texture1.Unbind(0);
	glBindVertexArray(0);
}

void MyGameApp::OnShutdown() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	App_Shader_Delete(&DefaultShader);
}

void MyGameApp::OnWindowResize(int newWidth, int newHeight) {
}