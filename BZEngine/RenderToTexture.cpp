#include "RenderToTexture.h"

RenderToTexture::RenderToTexture(int width, int height) : width_(width), height_(height)
{
	framebuffer_.Bind();
	texture_.Bind();
	texture_.Create(0, GL_R32UI, width, height, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
	texture_.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texture_.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	renderbuffer_.Bind();
	renderbuffer_.Storage(GL_DEPTH_COMPONENT, width, height);
	framebuffer_.AttachRenderbuffer(GL_DEPTH_ATTACHMENT, renderbuffer_);
	framebuffer_.AttachTexture(GL_COLOR_ATTACHMENT0, texture_);
	UnBind();
}

void RenderToTexture::Bind()
{
	framebuffer_.Bind();
	renderbuffer_.Bind();
	texture_.Bind();
	glViewport(0, 0, width_, height_);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void RenderToTexture::UnBind()
{
	framebuffer_.UnBind();
	renderbuffer_.UnBind();
	texture_.UnBind();
}

bool RenderToTexture::IsFramebufferComplete() const
{
	return framebuffer_.IsComplete();
}

int RenderToTexture::Width() const
{
	return width_;
}

int RenderToTexture::Height() const
{
	return height_;
}
