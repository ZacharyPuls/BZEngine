#pragma once

#include "Headers.h"

class Shader
{
public:
	Shader();
	~Shader();

	void AddVertexShader(const std::string &filename, const std::string& version, const std::string& preambleFilename);
	void AddFragmentShader(const std::string &filename, const std::string& version, const std::string& preambleFilename);
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
