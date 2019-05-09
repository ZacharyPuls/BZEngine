#pragma once

#include "Headers.h"
#include "AppWindow.h"
#include "GLWindow.h"

class MainMenu : public AppWindow
{
public:
	using AppWindow::AppWindow;
	~MainMenu();

	void Create() override;
};
