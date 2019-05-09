#include "AppWindow.h"

AppWindow::~AppWindow()
{
	std::clog << "[DEBUG] AppWindow::~AppWindow()" << std::endl;
}

void AppWindow::Draw()
{
	drawContents();
	drawWidgets();
}
