#pragma once
#include "ProjectManager.h"

class ProjectManagerUI {
public:
	ProjectManagerUI(ProjectManager* pm);
	~ProjectManagerUI() = default;

	void DrawSelectProject();

	ProjectManager* GetManager();

private:
	ProjectManager* m_projectManager = nullptr;
};