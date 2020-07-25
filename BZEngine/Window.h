#pragma once

#include "Headers.h"
#include "Point2D.h"
#include "Point2Di.h"

class Window
{
public:
	Window();
	Window(std::string title, const int width, const int height);
	~Window();

	void Create();
	void SwapBuffers() const;
	int GetMouseButton(int button) const;
	bool IsFocused() const;
	void SetCursorPos(double x, double y) const;
	Point2D GetCursorPos() const;
	void SetInputMode(int mode, int value) const;
	int GetInputMode(int mode) const;
	void SetCursor(GLFWcursor *cursor) const;
	Point2Di GetWindowSize() const;
	Point2Di GetFramebufferSize() const;
	bool ShouldClose() const;
	void MakeContextCurrent() const;
	static void PollEvents();
	GLFWwindow *GetPointer() const;
	void SetUserPointer(void *ptr) const;
	void SetCursorPosCallback(GLFWcursorposfun callback) const;
	void SetMouseButtonCallback(GLFWmousebuttonfun callback) const;
	void SetKeyCallback(GLFWkeyfun callback) const;
	void SetCharCallback(GLFWcharfun callback) const;
	void SetDropCallback(GLFWdropfun callback) const;
	void SetScrollCallback(GLFWscrollfun callback) const;
	void SetFramebufferSizeCallback(GLFWframebuffersizefun callback) const;
	void InstallCallbacks() const;
	void Close() const;
private:
	GLFWwindow *window_;
	std::string title_;
	int width_;
	int height_;

	static void cursorPosCallback_(GLFWwindow *window, const double x, const double y);
};
