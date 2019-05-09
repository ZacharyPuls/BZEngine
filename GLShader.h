#pragma once

#include "Headers.h"

class GLShader
{
public:
	GLShader();
	~GLShader();

	void AddVertexShader(const std::string &filename);
	void AddFragmentShader(const std::string &filename);
	GLuint GetUniformLocation(const std::string &name) const;

	void CompileAndLink();
	void Bind() const;
	static void UnBind();
private:
	GLuint programId_;
	GLuint vertexShaderId_;
	GLuint fragmentShaderId_;

	std::string readFileText_(const std::string &filename) const;
};
