#pragma once

#include "Headers.h"

class GLTexture
{
public:
	GLTexture();
	~GLTexture();

	void Bind();
	void UnBind() const;

	void SetParameter(GLenum parameter, GLint value) const;
	void SetPixelStore(GLenum parameter, GLint value) const;

	void Create(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *data) const;

	GLuint GetTextureId() const;
private:
	GLuint textureId_ = 0;
	GLenum target_ = GL_TEXTURE_2D;

	static void printError_(const std::string& function, const std::string& reason);
};
