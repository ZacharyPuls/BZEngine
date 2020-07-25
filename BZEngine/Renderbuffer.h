#pragma once

#include "Headers.h"

class Renderbuffer
{
public:
	Renderbuffer();

	void Bind();
	void UnBind();

	void Storage(GLenum internalFormat, GLsizei width, GLsizei height);

	GLuint Id() const;
private:
	std::shared_ptr<GLuint> id_;
};

