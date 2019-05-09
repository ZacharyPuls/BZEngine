#pragma once

#include <utility>
#include "Headers.h"
#include "GLWindow.h"

class AppWindow : public nanogui::Screen
{
public:
	using Screen::Screen;
	explicit AppWindow(GLWindow *parent) : parent_(parent)
	{
		std::clog << "[DEBUG] AppWindow::AppWindow()" << std::endl;
	}
	~AppWindow();

	virtual void Create() = 0;
	void Draw();
protected:
	nanogui::FormHelper formHelper_ = nullptr;
	GLWindow *parent_;
};
