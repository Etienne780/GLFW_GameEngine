#include "Application.h"

Application::Application(String name, String version)
    : name(name), version(version) {
}

void Application::OnStart() {}
void Application::OnUpdate() {}
void Application::OnShutdown() {}