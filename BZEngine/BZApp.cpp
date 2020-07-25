#include "BZApp.h"
#include <utility>

BZApp::BZApp()
{
	std::clog << "[DEBUG] BZApp::BZApp()" << std::endl;
}

BZApp::BZApp(std::string title) : title_(std::move(title))
{
	std::clog << "[DEBUG] BZApp::BZApp(...)" << std::endl;
}

BZApp::~BZApp()
{
	std::clog << "[DEBUG] BZApp::~BZApp()" << std::endl;
	// mainMenu_.reset();
	// window_.reset();
	// glfwTerminate();
}

void BZApp::Start()
{
	if (started_)
	{
		std::cerr << "Attempting to start BZApp that is already started. Aborting..." << std::endl;
		return;
	}

	if (!glfwInit())
	{
		std::cerr << "Could not initialize GLFW." << std::endl;
	}

	glfwSetErrorCallback(errorCallback_);

	window_ = new GLWindow("BZApp", 640, 480);
	window_->Create();

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	mainMenu_ = new MainMenu(window_);

	mainMenu_->initialize(window_->GetPointer(), false);
	mainMenu_->Create();
	window_->SetUserPointer(mainMenu_);
	window_->InstallCallbacks();
	
	while (!window_->ShouldClose())
	{
		GLWindow::PollEvents();
		renderFrame_();	
	}

	std::clog << "[DEBUG] Terminating BZApp and cleaning up GLFW." << std::endl;

	// delete mainMenu_;
	delete window_;
	glfwTerminate();
}

void BZApp::renderFrame_()
{
	const auto framebufferSize = window_->GetFramebufferSize();
	const auto displayWidth = framebufferSize.X;
	const auto displayHeight = framebufferSize.Y;
	
	glViewport(0, 0, displayWidth, displayHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	mainMenu_->Draw();
	window_->SwapBuffers();
}
