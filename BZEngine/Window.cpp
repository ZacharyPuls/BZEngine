#include "Window.h"
#include <utility>

Window::Window(): window_(nullptr), width_(640), height_(480)
{
	std::clog << "[DEBUG] GLWindow::GLWindow()" << std::endl;
}

Window::Window(std::string title, const int width, const int height): window_(nullptr), title_(std::move(title)),
                                                                                 width_(width),
                                                                                 height_(height)
{
	std::clog << "[DEBUG] GLWindow::GLWindow(...)" << std::endl;
}

Window::~Window()
{
	std::clog << "[DEBUG] GLWindow::~GLWindow()" << std::endl;
	if (window_) {
		glfwSetWindowUserPointer(window_, nullptr);
		glfwDestroyWindow(window_);
	}
}

void Window::Create()
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

void Window::SwapBuffers() const
{
	MakeContextCurrent();
	glfwSwapBuffers(window_);
}

int Window::GetMouseButton(const int button) const
{
	return glfwGetMouseButton(window_, button);
}

bool Window::IsFocused() const
{
	return glfwGetWindowAttrib(window_, GLFW_FOCUSED) != 0;
}

void Window::SetCursorPos(const double x, const double y) const
{
	glfwSetCursorPos(window_, x, y);
}

Point2D Window::GetCursorPos() const
{
	double x;
	double y;
	glfwGetCursorPos(window_, &x, &y);
	return { x, y };
}

void Window::SetInputMode(const int mode, const int value) const
{
	glfwSetInputMode(window_, mode, value);
}

int Window::GetInputMode(const int mode) const
{
	return glfwGetInputMode(window_, mode);
}

void Window::SetCursor(GLFWcursor* cursor) const
{
	glfwSetCursor(window_, cursor);
}

Point2Di Window::GetWindowSize() const
{
	int width;
	int height;
	glfwGetWindowSize(window_, &width, &height);
	return { width, height };
}

Point2Di Window::GetFramebufferSize() const
{
	int width;
	int height;
	glfwGetFramebufferSize(window_, &width, &height);
	return { width, height };
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void Window::MakeContextCurrent() const
{
	glfwMakeContextCurrent(window_);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

GLFWwindow* Window::GetPointer() const
{
	return window_;
}

void Window::SetUserPointer(void* ptr) const
{
	glfwSetWindowUserPointer(window_, ptr);
}

void Window::SetCursorPosCallback(GLFWcursorposfun callback) const
{
	glfwSetCursorPosCallback(window_, callback);
}

void Window::SetMouseButtonCallback(GLFWmousebuttonfun callback) const
{
	glfwSetMouseButtonCallback(window_, callback);
}

void Window::SetKeyCallback(GLFWkeyfun callback) const
{
	glfwSetKeyCallback(window_, callback);
}

void Window::SetCharCallback(GLFWcharfun callback) const
{
	glfwSetCharCallback(window_, callback);
}

void Window::SetDropCallback(GLFWdropfun callback) const
{
	glfwSetDropCallback(window_, callback);
}

void Window::SetScrollCallback(GLFWscrollfun callback) const
{
	glfwSetScrollCallback(window_, callback);
}

void Window::SetFramebufferSizeCallback(GLFWframebuffersizefun callback) const
{
	glfwSetFramebufferSizeCallback(window_, callback);
}

void Window::InstallCallbacks() const
{
	static auto cursorPosCallback = [&](GLFWwindow *window, const double x, const double y)
	{
	};
	SetCursorPosCallback([](GLFWwindow *window, const double x, const double y)
	{
		cursorPosCallback(window, x, y);
	});

	static auto mouseButtonCallback = [&](GLFWwindow *window, const int button, const int action, const int modifiers)
	{
	};
	SetMouseButtonCallback([](GLFWwindow *window, const int button, const int action, const int modifiers)
	{
		mouseButtonCallback(window, button, action, modifiers);
	});

	static auto keyCallback = [&](GLFWwindow *window, const int key, const int scancode, const int action, const int modifiers)
	{
	};
	SetKeyCallback([](GLFWwindow *window, const int key, const int scancode, const int action, const int modifiers)
	{
		keyCallback(window, key, scancode, action, modifiers);
	});

	static auto charCallback = [&](GLFWwindow *window, const unsigned int codepoint)
	{
	};
	SetCharCallback([](GLFWwindow *window, const unsigned int codepoint)
	{
		charCallback(window, codepoint);
	});

	static auto dropCallback = [&](GLFWwindow *window, const int count, const char **filenames)
	{
	};
	SetDropCallback([](GLFWwindow *window, const int count, const char **filenames)
	{
		dropCallback(window, count, filenames);
	});

	static auto scrollCallback = [&](GLFWwindow *window, const double x, const double y)
	{
	};
	SetScrollCallback([](GLFWwindow *window, const double x, const double y)
	{
		scrollCallback(window, x, y);
	});

	static auto framebufferSizeCallback = [&](GLFWwindow *window, const int width, const int height)
	{
	};
	SetFramebufferSizeCallback([](GLFWwindow *window, const int width, const int height)
	{
		framebufferSizeCallback(window, width, height);
	});
}

void Window::Close() const
{
	glfwSetWindowShouldClose(window_, GLFW_TRUE);
}
