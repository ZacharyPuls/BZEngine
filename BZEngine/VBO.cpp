#include "VBO.h"

VBO::VBO(): bufferId_(0), target_(GL_ARRAY_BUFFER)
{
}

VBO::VBO(const GLenum target): bufferId_(0), target_(target)
{
}

VBO::~VBO()
{
	if (bufferId_ != 0)
	{
		glDeleteBuffers(1, &bufferId_);
	}
}

void VBO::Create()
{
	glGenBuffers(1, &bufferId_);
}

void VBO::SetTarget(const GLenum target)
{
	target_ = target;
}

void VBO::SetData(const GLsizeiptr size, const GLvoid* data, const GLenum usage) const
{
	glBufferData(target_, size, data, usage);
}

void VBO::Bind() const
{
	glBindBuffer(target_, bufferId_);
}

void VBO::UnBind() const
{
	glBindBuffer(target_, 0);
}
