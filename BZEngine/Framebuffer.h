#pragma once

#include "Headers.h"
#include "Renderbuffer.h"
#include "Texture.h"

class Framebuffer
{
public:
	Framebuffer();

	void Bind();
	void UnBind();

	void AttachRenderbuffer(GLenum attachment, const Renderbuffer& renderbuffer);
	void AttachTexture(GLenum attachment, const Texture& texture);
	bool IsComplete() const;
	
	GLuint Id() const;
private:
	std::shared_ptr<GLuint> id_;
};

