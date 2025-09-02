#include "ProjectManager.h"

void ProjectManager::Open() {
	
}

void ProjectManager::Close() {

}

bool ProjectManager::HasProjectOpen() const { 
	return m_state == ProjectState::Opened; 
}

ProjectState ProjectManager::GetState() const { 
	return m_state; 
}

const std::string& ProjectManager::GetProjectName() const { 
	return m_name; 
}

void ProjectManager::LoadSettings() {

}

void ProjectManager::SaveSettings() {

}
