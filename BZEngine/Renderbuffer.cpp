#include "Renderbuffer.h"

Renderbuffer::Renderbuffer() : id_(new GLuint(), [](auto id) { glDeleteRenderbuffers(1, id); })
{
	glGenRenderbuffers(1, id_.get());
}

void Renderbuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, *id_);
}

void Renderbuffer::UnBind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::Storage(GLenum internalFormat, GLsizei width, GLsizei height)
{
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

GLuint Renderbuffer::Id() const
{
	return *id_;
}
