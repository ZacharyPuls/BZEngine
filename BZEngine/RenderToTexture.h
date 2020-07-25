#pragma once

#include "Headers.h"
#include "Texture.h"
#include "Framebuffer.h"

class RenderToTexture
{
public:
	RenderToTexture(int width, int height);

	void Bind();
	void UnBind();

	bool IsFramebufferComplete() const;
	
	int Width() const;
	int Height() const;
private:
	int width_;
	int height_;
	Texture texture_;
	Framebuffer framebuffer_;
	Renderbuffer renderbuffer_;
};

