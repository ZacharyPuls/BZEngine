#include "GLShader.h"

GLShader::GLShader(): programId_(0), vertexShaderId_(0), fragmentShaderId_(0)
{
}

GLShader::~GLShader()
{
	std::clog << "[DEBUG] GLShader::~GLShader()" << std::endl;
}

void GLShader::AddVertexShader(const std::string& filename)
{
	auto vertexShaderFileText = readFileText_(filename);
	auto fileText = vertexShaderFileText.c_str();
	int success;
	char infoLog[512];

	vertexShaderId_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId_, 1, &fileText, nullptr);
	glCompileShader(vertexShaderId_);
	glGetShaderiv(vertexShaderId_, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId_, 512, nullptr, infoLog);
		std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
	}
}

void GLShader::AddFragmentShader(const std::string& filename)
{
	auto fragmentShaderFileText = readFileText_(filename);
	auto fileText = fragmentShaderFileText.c_str();
	int success;
	char infoLog[512];

	fragmentShaderId_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId_, 1, &fileText, nullptr);
	glCompileShader(fragmentShaderId_);
	glGetShaderiv(fragmentShaderId_, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId_, 512, nullptr, infoLog);
		std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
	}
}

GLuint GLShader::GetUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(programId_, name.c_str());
}

void GLShader::CompileAndLink()
{
	int success;
	char infoLog[512];

	programId_ = glCreateProgram();
	glAttachShader(programId_, vertexShaderId_);
	glAttachShader(programId_, fragmentShaderId_);
	glLinkProgram(programId_);
	glGetProgramiv(programId_, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId_, 512, nullptr, infoLog);
		std::cerr << "Shader program link failed:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderId_);
	glDeleteShader(fragmentShaderId_);
}

void GLShader::Bind() const
{
	glUseProgram(programId_);
}

void GLShader::UnBind()
{
	glUseProgram(0);
}

std::string GLShader::readFileText_(const std::string& filename) const
{
	std::ifstream file(filename);

	std::string line;
	std::string fileText;

	if (file.is_open())
	{
		while (std::getline(file , line))
		{
			fileText += line + "\n";
		}

		file.close();
	}
	else
	{
		std::cerr << "Could not open file [" << filename << "]." << std::endl;
	}

	// std::cout << "[" << filename << "]: " << fileText << std::endl << std::endl;
	return fileText;
}
