#pragma once
#include <string>

class Window {
public:
	virtual void Draw();

protected:
	Window(const std::string& name);

	std::string m_name;
	float m_sizeX = 0, m_sizeY = 0;
};