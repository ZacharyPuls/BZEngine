#include "Texture.h"

#include <utility>
#include "Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(std::string name) : id_(0), name_(std::move(name)),
                                          textureId_(new GLuint(), [](auto id) { glDeleteTextures(1, id); })
{
	glGenTextures(1, textureId_.get());
}

Texture::~Texture()
{
	Log::Debug("GLTexture::~GLTexture()");
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, *textureId_);
}

void Texture::UnBind() const
{
	glBindTexture(target_, 0);
}

void Texture::SetParameter(const GLenum parameter, const GLint value) const
{
	glTexParameteri(target_, parameter, value);
}

void Texture::SetPixelStore(const GLenum parameter, const GLint value)
{
	glPixelStorei(parameter, value);
}

void Texture::Load(const std::string& filename, const GLint internalFormat, const GLenum format, bool flipVertically)
{
	Bind();
	int numComponents;
	if (flipVertically)
	{
		stbi_set_flip_vertically_on_load(1);
	}
	const auto pixels = stbi_load(filename.c_str(), &width_, &height_, &numComponents, STBI_rgb);
	if (!pixels)
	{
		std::cerr << "stbi_load(" << filename << ") failed with: " << stbi_failure_reason() << std::endl;
	}
	if (flipVertically)
	{
		stbi_set_flip_vertically_on_load(0);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width_, height_, 0, format, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	auto maxAnisotropy = 0;
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(pixels);
}

void Texture::Create(const GLint level, const GLint internalFormat, const GLsizei width, const GLsizei height,
                     const GLenum format, const GLenum type,
                     const GLvoid* data)
{
	glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, type, data);
	width_ = width;
	height_ = height;
}

uint32_t Texture::Id() const
{
	return id_;
}

std::string Texture::Name() const
{
	return name_;
}

void Texture::SetName(const std::string& name)
{
	name_ = name;
}

GLsizei Texture::Width() const
{
	return width_;
}

GLsizei Texture::Height() const
{
	return height_;
}

GLuint Texture::GetTextureId() const
{
	return *textureId_;
}

void Texture::printError_(const std::string& function, const std::string& reason)
{
	std::cerr << "Attempted to call GLTexture::" << function << "() with " << reason << "." << std::endl;
}
