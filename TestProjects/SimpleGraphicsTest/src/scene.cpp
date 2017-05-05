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
#include <scene.h>
#include <es_assert.h>



Scene::Scene()
{
}


Scene::~Scene()
{
}


void Scene::checkOpenGL()
{
#ifdef _DEBUG
    GLenum test = glGetError();
    if( test != GL_NO_ERROR )
    {
        switch( test )
        {
        case GL_INVALID_ENUM:
			printf("gl error: GL_INVALID_ENUM\n");
            break;

        case GL_INVALID_VALUE:
			printf("gl error: GL_INVALID_VALUE\n");
            break;

        case GL_INVALID_OPERATION:
			printf("gl error: GL_INVALID_OPERATION\n");
            break;

        case GL_OUT_OF_MEMORY:
			printf("gl error: GL_OUT_OF_MEMORY\n");
            break;

        default:
            {
				printf("gl error: %X\n", test);
            }
        }
    }
    assert( test == GL_NO_ERROR );


	EGLint err = eglGetError();

	if( err != EGL_SUCCESS )
    {
        switch( err )
        {
        case EGL_NOT_INITIALIZED:
			printf("egl error: EGL_NOT_INITIALIZED\n");
            break;

        case EGL_BAD_ACCESS:
			printf("egl error: EGL_BAD_ACCESS\n");
            break;

        case EGL_BAD_ALLOC:
			printf("egl error: EGL_BAD_ALLOC\n");
            break;

        case EGL_BAD_ATTRIBUTE:
			printf("egl error: EGL_BAD_ATTRIBUTE\n");
            break;
        
		case EGL_BAD_CONTEXT:
			printf("egl error: EGL_BAD_CONTEXT\n");
            break;
			        
		case EGL_BAD_CONFIG:
			printf("egl error: EGL_BAD_CONFIG\n");
            break;

		case EGL_BAD_CURRENT_SURFACE:
			printf("egl error: EGL_BAD_CURRENT_SURFACE\n");
            break;

		case EGL_BAD_DISPLAY:
			printf("egl error: EGL_BAD_DISPLAY\n");
            break;
		case EGL_BAD_SURFACE:
			printf("egl error: EGL_BAD_SURFACE\n");
            break;
		case EGL_BAD_MATCH:
			printf("egl error: EGL_BAD_MATCH\n");
            break;
		case EGL_BAD_PARAMETER:
			printf("egl error: EGL_BAD_PARAMETER\n");
            break;
		case EGL_BAD_NATIVE_PIXMAP:
			printf("egl error: EGL_BAD_NATIVE_PIXMAP\n");
            break;
		case EGL_BAD_NATIVE_WINDOW:
			printf("egl error: EGL_BAD_NATIVE_WINDOW\n");
            break;
		case EGL_CONTEXT_LOST:
			printf("egl error: EGL_CONTEXT_LOST\n");
            break;
        default:
            {
				printf("egl error: %X\n", err);
            }
        }
    }
	assert( err == EGL_SUCCESS );
#endif
}


