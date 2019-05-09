#pragma once

#include "Headers.h"
#include "AppWindow.h"

enum Resolution
{
	_1920x1080,
	_1280x960,
	_640x480
};

static int RESOLUTION_X(Resolution resolution)
{
	switch (resolution)
	{
	case _1920x1080:
		return 1920;
	case _1280x960:
		return 1280;
	case _640x480:
		return 640;
	}
}

enum Quality
{
	LOW,
	MEDIUM,
	HIGH,
	ULTRA
};

struct Settings
{
	int ResolutionX;
	int ResolutionY;
	Quality GraphicsQuality;
};

class OptionsMenu : public AppWindow
{
public:
	using AppWindow::AppWindow;
	~OptionsMenu();

	void Create() override;
private:
	Settings settings_;
};
