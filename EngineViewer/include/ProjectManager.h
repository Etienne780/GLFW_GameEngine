#pragma once
#include <string>

enum class ProjectState {
	None = 0,// no project open
	Opened // project open
};

class ProjectManager {
public:
	ProjectManager() = default;
	~ProjectManager() = default;

	void Open();
	void Close();

	bool HasProjectOpen() const;
	ProjectState GetState() const;
	const std::string& GetProjectName() const;

private:
	ProjectState m_state = ProjectState::None;

	std::string m_name;
	std::string m_projectPath;
	std::string m_exportPath;

	void LoadSettings();
	void SaveSettings();
};