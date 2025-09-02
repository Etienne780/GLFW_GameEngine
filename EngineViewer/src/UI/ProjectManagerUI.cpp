#include "UI/ProjectManagerUI.h"

ProjectManagerUI::ProjectManagerUI(ProjectManager* pm) {
	m_projectManager = pm;
}

void ProjectManagerUI::DrawSelectProject() {

}

ProjectManager* ProjectManagerUI::GetManager() {
	return m_projectManager;
}