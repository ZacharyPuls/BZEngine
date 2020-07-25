#pragma once

#include "Headers.h"

class VBO
{
public:
	VBO();
	explicit VBO(GLenum target);
	~VBO();

	void Create();
	void SetTarget(GLenum target);
	void SetData(GLsizeiptr size, const GLvoid *data, GLenum usage) const;

	void Bind() const;
	void UnBind() const;
private:
	GLuint bufferId_;
	GLenum target_;
};
