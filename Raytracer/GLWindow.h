#pragma once
#include <functional>
#include "Canvas.h"

class GLInternal;

class GLWindow {
public:
	GLWindow(int width, int height);
	~GLWindow();
	void StartMainLoop() noexcept;

	typedef std::function<void(const Vector& from, const Point& to, const Vector& up)> TRenderCallBackFunc;
	void SetRenderCallback(const TRenderCallBackFunc& func);
	Canvas& GetCanvas() noexcept;
private:
	std::unique_ptr<GLInternal> _impl;
};
