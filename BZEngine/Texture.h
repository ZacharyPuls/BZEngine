#pragma once

#include "Headers.h"
#include <atomic>

class Texture
{
public:
	explicit Texture(std::string name = "");
	~Texture();

	void Bind() const;
	void UnBind() const;

	void SetParameter(GLenum parameter, GLint value) const;
	static void SetPixelStore(GLenum parameter, GLint value);

	void Load(const std::string& filename, GLint internalFormat, GLenum format, bool flipVertically = true);
	
	void Create(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *data);

	uint32_t Id() const;

	std::string Name() const;
	void SetName(const std::string& name);

	GLsizei Width() const;
	GLsizei Height() const;
	
	GLuint GetTextureId() const;
private:
	uint32_t id_;
	std::string name_;
	
	std::shared_ptr<GLuint> textureId_;
	GLenum target_ = GL_TEXTURE_2D;

	GLsizei width_;
	GLsizei height_;
	
	static void printError_(const std::string& function, const std::string& reason);

	// static std::atomic<int> nextId_;
};
