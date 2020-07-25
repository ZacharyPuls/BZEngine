#pragma once

#include "Scene.h"
#include <memory>

class IRenderer
{
public:
	virtual void SetViewport(int width, int height) = 0;
	virtual void RenderFrame() = 0;
	
protected:
	std::shared_ptr<Scene> scene_;
	bool isFirstFrame_;

	int viewportWidth_ = 0;
	int viewportHeight_ = 0;
};

