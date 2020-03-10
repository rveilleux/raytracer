#include "GLWindow.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>  // for MS Windows
#include <gl/gl.h>
#include <gl/glu.h>
#pragma warning(push)
#pragma warning(disable:4505)
#include "freeglut-3.2.1/include/GL/freeglut.h"
#include "freeglut-3.2.1/include/GL/glut.h"  // GLUT, include glu.h and gl.h
#pragma warning(pop)
#include "Matrix.h"
#include "Dynarray.h"

class GLInternal {
public:
	GLInternal(int width, int height);
	~GLInternal();

	void SetRenderCallback(const GLWindow::TRenderCallBackFunc& func);

	void display();
	void reshape_window(GLsizei w, GLsizei h);
	void setupTexture();
	void updateTexture();
	void keyboardDown(unsigned char key, int /*x*/, int /*y*/);
	void keyboardUp(unsigned char key, int x, int y);
	void specialKeys(int key, int /*x*/, int /*y*/);
	void mouse(int /*button*/, int /*state*/, int /*x*/, int /*y*/);
	void passiveMotion(int x, int y);
private:
	int _width;
	int _height;
	Dynarray<uint8_t> _screenData;

	GLWindow::TRenderCallBackFunc _renderFunc;
	Point _cameraFrom = Point(0, 1.5, -5);
	Vector _cameraForward = Vector(0, 0, 0);
	Vector _cameraStrafe = Vector(0, 0, 0);
	Vector _cameraUp = Vector(0, 0, 0);
	double _forwardSpeed = 0;
	double _upSpeed = 0;
	double _strafeSpeed = 0;

	//Display size
	int _screenWidth = 0;
	int _screenHeight = 0;

	// Window size
	int _displayWidth = 0;
	int _displayHeight = 0;

	double _speed = 0.1;

	void ComputeCameraPosition();
	void RenderCanvasToScreenData();
	void OnKeyboardMove(unsigned char key, double direction);
};

namespace GLGlobal {
	GLInternal* glInternal = nullptr;

	void display() {
		glInternal->display();
	}

	void reshape_window(GLsizei w, GLsizei h) {
		glInternal->reshape_window(w, h);
	}
	void setupTexture() {
		glInternal->setupTexture();
	}
	void updateTexture() {
		glInternal->updateTexture();
	}
	void keyboardDown(unsigned char key, int x, int y) {
		glInternal->keyboardDown(key, x, y);
	}
	void keyboardUp(unsigned char key, int x, int y) {
		glInternal->keyboardUp(key, x, y);
	}
	void specialKeys(int key, int x, int y) {
		glInternal->specialKeys(key, x, y);
	}
	void mouse(int button, int state, int x, int y) {
		glInternal->mouse(button, state, x, y);
	}
	void passiveMotion(int x, int y) {
		glInternal->passiveMotion(x, y);
	}
};

// Setup Texture
void GLInternal::setupTexture() {
	// Create a texture 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _screenWidth, _screenHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)_screenData.GetData());

	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

void GLInternal::SetRenderCallback(const GLWindow::TRenderCallBackFunc& func) {
	_renderFunc = func;
}

void GLInternal::ComputeCameraPosition() {
	_cameraFrom = _cameraFrom + _cameraForward * _forwardSpeed + _cameraStrafe * _strafeSpeed + _cameraUp * _upSpeed;
}

void GLInternal::RenderCanvasToScreenData() {
	Point cameraTo = _cameraFrom + _cameraForward;
	const Canvas& canvas = _renderFunc(_cameraFrom, cameraTo, _cameraUp);
	// Update pixels
	uint8_t* writePtr = _screenData.GetData();
	for (int y = 0; y < _screenHeight; ++y) {
		for (int x = 0; x < _screenWidth; ++x) {
			const Color& c = canvas.PixelAt(x, y);
			writePtr[2] = Color::DoubleToByte(c.x);
			writePtr[1] = Color::DoubleToByte(c.y);
			writePtr[0] = Color::DoubleToByte(c.z);
			writePtr += 4;
		}
	}
}

void GLInternal::updateTexture() {
	if (!_renderFunc) return;

	ComputeCameraPosition();

	RenderCanvasToScreenData();
	// Update Texture
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _screenWidth, _screenHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)_screenData.GetData());

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);		glVertex2d(0.0, 0.0);
	glTexCoord2d(1.0, 0.0); 	glVertex2d(_displayWidth, 0.0);
	glTexCoord2d(1.0, 1.0); 	glVertex2d(_displayWidth, _displayHeight);
	glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0, _displayHeight);
	glEnd();
}

void GLInternal::display() {
	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw pixels to texture
	updateTexture();

	// Swap buffers!
	glutSwapBuffers();
}

void GLInternal::reshape_window(GLsizei w, GLsizei h) {
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);

	// Resize quad
	_displayWidth = w;
	_displayHeight = h;
}

/* Callback handler for normal-key event: when a key has been pressed */
void GLInternal::keyboardDown(unsigned char key, int /*x*/, int /*y*/) {
	OnKeyboardMove(key, 1);
}

/* Callback handler for normal-key event: when a key has been released */
void GLInternal::keyboardUp(unsigned char key, int /*x*/, int /*y*/) {
	OnKeyboardMove(key, -1);
}

void GLInternal::OnKeyboardMove(unsigned char key, double direction) {
	double resultSpeed = _speed * direction;
	switch (key) {
	case 27:     // ESC key
		//exit(0);
		glutLeaveMainLoop();
		break;
	case 'a':
		_strafeSpeed -= resultSpeed;
		break;
	case 'd':
		_strafeSpeed += resultSpeed;
		break;
	case 'w':
		_forwardSpeed += resultSpeed;
		break;
	case 's':
		_forwardSpeed -= resultSpeed;
		break;
	case 'q':
		_upSpeed -= resultSpeed;
		break;
	case 'e':
		_upSpeed += resultSpeed;
		break;
	}
}

/* Callback handler for special-key event */
void GLInternal::specialKeys(int key, int /*x*/, int /*y*/) {
	switch (key) {
		//case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
		//	fullScreenMode = !fullScreenMode;         // Toggle state
		//	if (fullScreenMode) {                     // Full-screen mode
		//		windowPosX = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
		//		windowPosY = glutGet(GLUT_WINDOW_Y);
		//		windowWidth = glutGet(GLUT_WINDOW_WIDTH);
		//		windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
		//		glutFullScreen();                      // Switch into full screen
		//	}
		//	else {                                         // Windowed mode
		//		glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
		//		glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
		//	}
		//	break;
	case GLUT_KEY_DOWN:    // Right: increase x speed
		break;
	case GLUT_KEY_LEFT:     // Left: decrease x speed
		break;
	}
}

/* Callback handler for mouse event */
void GLInternal::mouse(int /*button*/, int /*state*/, int /*x*/, int /*y*/) {
	//if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Pause/resume
	//}
	//glutWarpPointer(320, 200);
}

/* Callback handler for mouse event */
void GLInternal::passiveMotion(int x, int y) {
	int dx = _displayWidth / 2 - x;
	int dy = _displayHeight / 2 - y;
	auto rotate = RotationY(-dx / 100.0) * RotationX(-dy / 100.0);
	_cameraForward = rotate * Vector(0, 0, 1);
	_cameraStrafe = rotate * Vector(1, 0, 0);
	_cameraUp = rotate * Vector(0, 1, 0);
}

GLInternal::GLInternal(int width, int height)
	: _width(width)
	, _height(height)
	, _screenData(_width * _height * 4)

{
	_ASSERT_EXPR(GLGlobal::glInternal == nullptr, L"GLInternal failure: only one GLWindow is supported");

	GLGlobal::glInternal = this;

	_screenWidth = _width;
	_screenHeight = _height;
	int modifier = 2;
	_displayWidth = _screenWidth * modifier;
	_displayHeight = _screenHeight * modifier;

	int argc = 0;
	char* argv = nullptr;
	glutInit(&argc, &argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(_displayWidth, _displayHeight);
	glutInitWindowPosition(320, 320);
	glutCreateWindow("Rem Raytracer");

	glutDisplayFunc(GLGlobal::display);
	glutIdleFunc(GLGlobal::display);
	glutReshapeFunc(GLGlobal::reshape_window);
	glutSpecialFunc(GLGlobal::specialKeys); // Register callback handler for special-key event
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(GLGlobal::keyboardDown);   // Register callback handler for special-key event
	glutKeyboardUpFunc(GLGlobal::keyboardUp);   // Register callback handler for special-key event
	glutMouseFunc(GLGlobal::mouse);   // Register callback handler for mouse event
	glutPassiveMotionFunc(GLGlobal::passiveMotion);

	setupTexture();
}

GLInternal::~GLInternal() {
	GLGlobal::glInternal = nullptr;
}

GLWindow::GLWindow(int width, int height)
	: _impl(std::make_unique<GLInternal>(width, height))
{
}

GLWindow::~GLWindow() {
}

void GLWindow::StartMainLoop() {
	glutMainLoop();
}

void GLWindow::SetRenderCallback(const TRenderCallBackFunc& func) {
	_impl->SetRenderCallback(func);
}
