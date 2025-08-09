#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "Windows\Window.h"

Window::Window(const std::string& name) {
	m_name = name;
}

void Window::Draw() {}