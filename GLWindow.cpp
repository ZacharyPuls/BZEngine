#include "GLWindow.h"
#include <utility>

GLWindow::GLWindow(): window_(nullptr), width_(640), height_(480)
{
	std::clog << "[DEBUG] GLWindow::GLWindow()" << std::endl;
}

GLWindow::GLWindow(std::string title, const int width, const int height): window_(nullptr), title_(std::move(title)),
                                                                                 width_(width),
                                                                                 height_(height)
{
	std::clog << "[DEBUG] GLWindow::GLWindow(...)" << std::endl;
}

GLWindow::~GLWindow()
{
	std::clog << "[DEBUG] GLWindow::~GLWindow()" << std::endl;
	if (window_) {
		glfwSetWindowUserPointer(window_, nullptr);
		glfwDestroyWindow(window_);
	}
}

void GLWindow::Create()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);

	if (!window_)
	{
		std::cerr << "Could not create GLFW window." << std::endl;
	}

	MakeContextCurrent();
	glfwSwapInterval(1);
}

void GLWindow::SwapBuffers() const
{
	MakeContextCurrent();
	glfwSwapBuffers(window_);
}

int GLWindow::GetMouseButton(const int button) const
{
	return glfwGetMouseButton(window_, button);
}

bool GLWindow::IsFocused() const
{
	return glfwGetWindowAttrib(window_, GLFW_FOCUSED) != 0;
}

void GLWindow::SetCursorPos(const double x, const double y) const
{
	glfwSetCursorPos(window_, x, y);
}

Point2D GLWindow::GetCursorPos() const
{
	double x;
	double y;
	glfwGetCursorPos(window_, &x, &y);
	return { x, y };
}

void GLWindow::SetInputMode(const int mode, const int value) const
{
	glfwSetInputMode(window_, mode, value);
}

int GLWindow::GetInputMode(const int mode) const
{
	return glfwGetInputMode(window_, mode);
}

void GLWindow::SetCursor(GLFWcursor* cursor) const
{
	glfwSetCursor(window_, cursor);
}

Point2Di GLWindow::GetWindowSize() const
{
	int width;
	int height;
	glfwGetWindowSize(window_, &width, &height);
	return { width, height };
}

Point2Di GLWindow::GetFramebufferSize() const
{
	int width;
	int height;
	glfwGetFramebufferSize(window_, &width, &height);
	return { width, height };
}

bool GLWindow::ShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void GLWindow::MakeContextCurrent() const
{
	glfwMakeContextCurrent(window_);
}

void GLWindow::PollEvents()
{
	glfwPollEvents();
}

GLFWwindow* GLWindow::GetPointer() const
{
	return window_;
}

void GLWindow::SetUserPointer(void* ptr) const
{
	glfwSetWindowUserPointer(window_, ptr);
}

void GLWindow::SetCursorPosCallback(GLFWcursorposfun callback) const
{
	glfwSetCursorPosCallback(window_, callback);
}

void GLWindow::SetMouseButtonCallback(GLFWmousebuttonfun callback) const
{
	glfwSetMouseButtonCallback(window_, callback);
}

void GLWindow::SetKeyCallback(GLFWkeyfun callback) const
{
	glfwSetKeyCallback(window_, callback);
}

void GLWindow::SetCharCallback(GLFWcharfun callback) const
{
	glfwSetCharCallback(window_, callback);
}

void GLWindow::SetDropCallback(GLFWdropfun callback) const
{
	glfwSetDropCallback(window_, callback);
}

void GLWindow::SetScrollCallback(GLFWscrollfun callback) const
{
	glfwSetScrollCallback(window_, callback);
}

void GLWindow::SetFramebufferSizeCallback(GLFWframebuffersizefun callback) const
{
	glfwSetFramebufferSizeCallback(window_, callback);
}

void GLWindow::InstallCallbacks() const
{
	static auto cursorPosCallback = [&](GLFWwindow *window, const double x, const double y)
	{
		auto screen = reinterpret_cast<nanogui::Screen*>(glfwGetWindowUserPointer(window_));
		screen->cursorPosCallbackEvent(x, y);
	};
	SetCursorPosCallback([](GLFWwindow *window, const double x, const double y)
	{
		cursorPosCallback(window, x, y);
	});

	static auto mouseButtonCallback = [&](GLFWwindow *window, const int button, const int action, const int modifiers)
	{
		auto screen = reinterpret_cast<nanogui::Screen*>(glfwGetWindowUserPointer(window_));
		screen->mouseButtonCallbackEvent(button, action, modifiers);
	};
	SetMouseButtonCallback([](GLFWwindow *window, const int button, const int action, const int modifiers)
	{
		mouseButtonCallback(window, button, action, modifiers);
	});

	static auto keyCallback = [&](GLFWwindow *window, const int key, const int scancode, const int action, const int modifiers)
	{
		auto screen = reinterpret_cast<nanogui::Screen*>(glfwGetWindowUserPointer(window_));
		screen->keyCallbackEvent(key, scancode, action, modifiers);
	};
	SetKeyCallback([](GLFWwindow *window, const int key, const int scancode, const int action, const int modifiers)
	{
		keyCallback(window, key, scancode, action, modifiers);
	});

	static auto charCallback = [&](GLFWwindow *window, const unsigned int codepoint)
	{
		auto screen = reinterpret_cast<nanogui::Screen*>(glfwGetWindowUserPointer(window_));
		screen->charCallbackEvent(codepoint);
	};
	SetCharCallback([](GLFWwindow *window, const unsigned int codepoint)
	{
		charCallback(window, codepoint);
	});

	static auto dropCallback = [&](GLFWwindow *window, const int count, const char **filenames)
	{
		auto screen = reinterpret_cast<nanogui::Screen*>(glfwGetWindowUserPointer(window_));
		screen->dropCallbackEvent(count, filenames);
	};
	SetDropCallback([](GLFWwindow *window, const int count, const char **filenames)
	{
		dropCallback(window, count, filenames);
	});

	static auto scrollCallback = [&](GLFWwindow *window, const double x, const double y)
	{
		auto screen = reinterpret_cast<nanogui::Screen*>(glfwGetWindowUserPointer(window_));
		screen->scrollCallbackEvent(x, y);
	};
	SetScrollCallback([](GLFWwindow *window, const double x, const double y)
	{
		scrollCallback(window, x, y);
	});

	static auto framebufferSizeCallback = [&](GLFWwindow *window, const int width, const int height)
	{
		auto screen = reinterpret_cast<nanogui::Screen*>(glfwGetWindowUserPointer(window_));
		screen->resizeCallbackEvent(width, height);
	};
	SetFramebufferSizeCallback([](GLFWwindow *window, const int width, const int height)
	{
		framebufferSizeCallback(window, width, height);
	});
}

void GLWindow::Close() const
{
	glfwSetWindowShouldClose(window_, GLFW_TRUE);
}
