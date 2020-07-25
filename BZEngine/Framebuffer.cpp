#include "Framebuffer.h"

Framebuffer::Framebuffer() : id_(new GLuint(), [](auto id) { glDeleteFramebuffers(1, id); })
{
	glGenFramebuffers(1, id_.get());
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, *id_);
}

void Framebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::AttachRenderbuffer(GLenum attachment, const Renderbuffer& renderbuffer)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer.Id());
}

void Framebuffer::AttachTexture(GLenum attachment, const Texture& texture)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.GetTextureId(), 0);
}

bool Framebuffer::IsComplete() const
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

GLuint Framebuffer::Id() const
{
	return *id_;
}
