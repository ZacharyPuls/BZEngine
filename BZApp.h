#pragma once

#include "Headers.h"
#include "GLShader.h"
#include "VBO.h"
#include "GLWindow.h"
#include "MainMenu.h"

class BZApp
{
public:
	BZApp();
	explicit BZApp(std::string title);
	~BZApp();

	void Start();
private:
	bool started_ = false;
	const std::string title_;
	GLWindow *window_;
	MainMenu *mainMenu_;
	const int width_ = 1280;
	const int height_ = 960;
	// double lastFrameTime_ = 0.0;
	// GLuint shaderPositionAttributeLocation_ = 0;
	// GLuint shaderUvAttributeLocation_ = 1;
	// GLuint shaderColorAttributeLocation_ = 2;
	// GLuint shaderProjectionMatrixUniformLocation_{};
	// GLuint shaderTextureSamplerUniformLocation_{};
	// std::shared_ptr<GLShader> shader_;
	// VBO vertexBuffer_;
	// VBO elementBuffer_;
	// GLTexture fontTexture_;

	static void errorCallback_(const int error, const char *description)
	{
		std::cerr << "GLFW encountered an error [" << error << "]: " << description << std::endl;
	}

	void renderFrame_();
};
