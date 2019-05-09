#include "GLTexture.h"

GLTexture::GLTexture() = default;

GLTexture::~GLTexture()
{
	std::clog << "[DEBUG] GLTexture::~GLTexture()" << std::endl;
}

void GLTexture::Bind()
{
	if (textureId_ == 0)
	{
		glGenTextures(1, &textureId_);
	}

	glBindTexture(target_, textureId_);
}

void GLTexture::UnBind() const
{
	if (textureId_ == 0)
	{
		printError_("UnBind", "NULL GLTexture::textureId_");
		return;
	}

	glBindTexture(target_, 0);
}

void GLTexture::SetParameter(const GLenum parameter, const GLint value) const
{
	if (textureId_ == 0)
	{
		printError_("SetParameter", "NULL GLTexture::textureId_");
		return;
	}

	glTexParameteri(target_, parameter, value);
}

void GLTexture::SetPixelStore(const GLenum parameter, const GLint value) const
{
	if (textureId_ == 0)
	{
		printError_("SetPixelStore", "NULL GLTexture::textureId_");
		return;
	}

	glPixelStorei(parameter, value);
}

void GLTexture::Create(const GLint level, const GLint internalFormat, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type,
	const GLvoid* data) const
{
	glTexImage2D(target_, level, internalFormat, width, height, 0, format, type, data);
}

GLuint GLTexture::GetTextureId() const
{
	return textureId_;
}

void GLTexture::printError_(const std::string& function, const std::string& reason)
{
	std::cerr << "Attempted to call GLTexture::" << function << "() with " << reason << "." << std::endl;
}
