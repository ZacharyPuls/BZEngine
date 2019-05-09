#pragma once

#include "Headers.h"

class VAO
{
public:
	VAO();
	~VAO();
	void Create();
	void Bind() const;

private:
	GLuint arrayId_;
};
