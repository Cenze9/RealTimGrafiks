// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Copyright (c) 2013 Mikko Romppainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef ESUTIL_H_
#define ESUTIL_H_

#if defined(_WIN32)
#include <graphics/Win32/EGL/egl.h>
#include <graphics/Win32/GLES2/gl2.h>
#else
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#endif


#include <vector>
#include <string>

#if defined(ANDROID)
struct android_app;
class ASensorManager;
class ASensor;
class ASensorEventQueue;
extern "C" void app_dummy();
#define ENGINE_DUMMY app_dummy();
#endif

#if defined(_WIN32)
#define ENGINE_DUMMY
#endif

namespace core
{


	enum TouchEventType
	{
		TOUCH_BEGIN = 0,
		TOUCH_MOVE,
		TOUCH_CANCEL,
		TOUCH_END
	};

}

namespace graphics
{
enum Version
{
	ES_VERSION_1 = 0,
	ES_VERSION_2 = 1
};

/**
 * Flags for creating window isong esCreateWindow function. Flags can be combined
 * either using or operator '|' or '+' sign.
 */
enum WindowFlag
{
	/// RGB color buffer. Color buffer should have R,G,B channels
	ES_WINDOW_RGB =			0,
	/// ALPHA color buffer. Color buffer should have alpha.
	ES_WINDOW_ALPHA =		1, 
	/// depth buffer should be created.
	ES_WINDOW_DEPTH =		2,
	/// stencil buffer should be created
	ES_WINDOW_STENCIL =		4,
	/// multi-sample buffer should be created
	ES_WINDOW_MULTISAMPLE =	8,
	/// resizable window should be created
	ES_WINDOW_RESIZEABLE =	16,
	/// default flags
	ES_WINDOW_DEFAULT = ES_WINDOW_RGB|ES_WINDOW_ALPHA|ES_WINDOW_DEPTH
};


/**
 * Application context.
 */
struct ESContext
{
	/// Window width
	GLint       width;

	/// Window height
	GLint       height;
	
	/// EGL display
	EGLDisplay  eglDisplay;
      
	/// EGL context
	EGLContext  eglContext;

	/// EGL surface
	EGLSurface  eglSurface;

	Version version;

	/// Callbacks
	bool (*initFunc) ( ESContext* );
	void (*drawFunc) ( ESContext* );
	void (*updateFunc) ( ESContext*, float deltaTime );
	void (*deinitFunc) ( ESContext* );
	void (*touchEventFunc) ( ESContext*, core::TouchEventType type, int touchId, int x, int y );

#if defined(_WIN32)
	/// Window handle
	EGLNativeWindowType  hWnd;
#elif defined(ANDROID)
	struct android_app* app;
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
	GLint windowCreateFlags;
    ASensorEventQueue* sensorEventQueue;
#endif
};


/**
 * Initialize ES graphics context.  This must be called before calling any other functions.
 * 
 * @param esContext Application context
 */

#if defined(ANDROID)
void esInitContext_android( ESContext *esContext );
#endif
static inline void esInitContext ( ESContext *esContext, Version version = ES_VERSION_2 )
{
	ENGINE_DUMMY
//	assert ( esContext != NULL );
	memset( esContext, 0, sizeof( ESContext) );

	esContext->version = version;
#if defined(ANDROID)
	esInitContext_android( esContext );
#endif
}


/**
 * Create a window with the specified parameters.
 * 
 * @param esContext Application context
 * @param title Name for title bar of window
 * @param width Width in pixels of window to create
 * @param height Height in pixels of window to create
 * @param flags Bitfield for the window creation flags see WindowFlag. 
 *
 * @return GL_TRUE if window creation is succesful, GL_FALSE otherwise
 */
GLboolean esCreateWindow ( ESContext *esContext, GLint width, GLint height, GLint flags, Version ver = ES_VERSION_2  );


/**
 * Start the main loop for the OpenGL ES application.
 *
 * @param esContext Application context
 */
void esMainLoop ( ESContext *esContext );

/**
 * Register a init callback function to be used to init the game.
 *
 * @param esContext Application context
 * @param drawFunc Draw callback function that will be used to render the scene
 */
void esRegisterInitFunc ( ESContext *esContext, bool (*initFunc) ( ESContext* ) );


/**
 * Register a draw callback function to be used to render each frame.
 *
 * @param esContext Application context
 * @param drawFunc Draw callback function that will be used to render the scene
 */
void esRegisterDrawFunc ( ESContext *esContext, void (*drawFunc) ( ESContext* ) );


/**
 * Register an update callback function to be used to update on each time step.
 *
 * @param esContext Application context
 * @param updateFunc Update callback function that will be used to render the scene
 */
void esRegisterUpdateFunc ( ESContext *esContext, void (*updateFunc) ( ESContext*, float ) );


/**
 * Register a deinit callback function to be used to deallocate resources.
 *
 * @param esContext Application context
 * @param deinitFunc Deinit callback function
 */
void esRegisterDeinitFunc ( ESContext *esContext, void (*deinitFunc) ( ESContext* ) );


/**
 * Register an keyboard input processing callback function.
 *
 * @param esContext Application context
 * @param keyFunc Key callback function for application processing of keyboard input
 */
void esRegisterKeyFunc ( ESContext *esContext, void (*drawFunc) ( ESContext*, unsigned char, int, int ) );

void esRegisterTouchEventFunc ( ESContext *esContext, void (*touchEventFunc) ( ESContext*, core::TouchEventType type, int touchId, int x, int y ) );





}


#endif


