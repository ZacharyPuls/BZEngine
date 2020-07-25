#include "Shader.h"
#include "Log.h"

Shader::Shader(): programId_(0), vertexShaderId_(0), fragmentShaderId_(0)
{
}

Shader::~Shader()
{
	std::clog << "[DEBUG] GLShader::~GLShader()" << std::endl;
}

void Shader::AddVertexShader(const std::string& filename, const std::string& version, const std::string& preambleFilename)
{
	auto vertexShader = version + '\n' + readFileText_(preambleFilename);
	vertexShader += readFileText_(filename);
	auto vertexShaderSource = vertexShader.c_str();
	int success;
	char infoLog[512];

	vertexShaderId_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId_, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShaderId_);
	glGetShaderiv(vertexShaderId_, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId_, 512, nullptr, infoLog);
		std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
	}
}

void Shader::AddFragmentShader(const std::string& filename, const std::string& version, const std::string& preambleFilename)
{
	auto fragmentShader = version + '\n' + readFileText_(preambleFilename);
	fragmentShader += readFileText_(filename);
	auto fragmentShaderSource = fragmentShader.c_str();
	int success;
	char infoLog[512];

	fragmentShaderId_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId_, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShaderId_);
	glGetShaderiv(fragmentShaderId_, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId_, 512, nullptr, infoLog);
		std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
	}
}

GLuint Shader::GetUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(programId_, name.c_str());
}

void Shader::CompileAndLink()
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

void Shader::Bind() const
{
	glUseProgram(programId_);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

std::string Shader::readFileText_(const std::string& filename) const
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
