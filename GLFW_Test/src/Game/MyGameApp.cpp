#include "MyGameApp.h"

using namespace EngineCore;

MyGameApp::MyGameApp() 
	: Application("MyGameApp", "1.0.0") {
}

unsigned int EBO, VBO, VAO;
Shader DefaultShader;
Texture2D texture1, texture2;
void MyGameApp::OnStart() {
	App_Background_SetColor(0.2f, 0.3f, 0.3f);

	DefaultShader = Shader("shader/Default.vert", "shader/Default.frag");
	
	texture1.Create("assets/stone.jpg");
	texture2.Create("assets/missingTexture.png");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions // colors // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
	};
	unsigned int indices[] = {
		1, 0, 3,
		3, 2, 1
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLsizei vertexSize = 8 * sizeof(float);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// UV attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	App_Shader_Bind(&DefaultShader);
	DefaultShader.SetInt("texture1", 0);
	DefaultShader.SetInt("texture2", 1);
}

void MyGameApp::OnUpdate() {
	if (Input::KeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(GetWindow(), true);

	App_Background_Clear();

	App_Shader_Bind(&DefaultShader);
	texture1.Bind(0);
	texture2.Bind(1);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	texture1.Unbind(0);
	texture2.Unbind(1);

	glBindVertexArray(0);
}

void MyGameApp::OnShutdown() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	App_Shader_Delete(&DefaultShader);
}

void MyGameApp::OnWindowResize(int newWidth, int newHeight) {
	OnUpdate();
}