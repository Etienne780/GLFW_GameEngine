#pragma once
#include <string>

class ProjectManager {
public:
	ProjectManager() = default;
	~ProjectManager() = default;

	void Open();
	void Close();

	void Update();

private:
	std::string m_projectPath;
	std::string m_exportPath;

	void LoadSettings();
	void SaveSettings();
};