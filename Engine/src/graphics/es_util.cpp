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
#include <graphics/OpenGLES/es_util.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

int Vsnprintf8(char* pDestination, size_t n, const char* pFormat, va_list arguments)
{
#ifdef _MSC_VER
  return _vsnprintf(pDestination, n, pFormat, arguments);
#else
  return vsnprintf(pDestination, n, pFormat, arguments);
#endif
}




namespace graphics
{

// anonymous namespace for internal functions
namespace
{
#if defined(ANDROID)
template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) {  return  __a < __b ? __b : __a; }
#endif

#if defined(YAM_WRITING_LOGS_TO_FILE)
	const char* const logFileName = 	YAM_WRITING_LOGS_TO_FILE;
	FILE* logFileHandle = (strlen(logFileName) > 0) ? fopen (logFileName,"w") : NULL;
#endif

	int toInteger(double v)
	{
		return int(v+0.5);
	}
}


void esRegisterInitFunc ( ESContext *esContext, bool (*initFunc) ( ESContext* ) )
{
	esContext->initFunc = initFunc;
}

void esRegisterDrawFunc ( ESContext *esContext, void (*drawFunc) (ESContext* ) )
{
	esContext->drawFunc = drawFunc;
}


void esRegisterUpdateFunc ( ESContext *esContext, void (*updateFunc) ( ESContext*, float ) )
{
	esContext->updateFunc = updateFunc;
}


void esRegisterDeinitFunc ( ESContext *esContext, void (*deinitFunc) ( ESContext* ) )
{
	esContext->deinitFunc = deinitFunc;
}

void esRegisterTouchEventFunc(ESContext *esContext, void(*touchEventFunc) (ESContext*, core::TouchEventType type, int touchId, int x, int y))
{
	esContext->touchEventFunc = touchEventFunc;
}

}


void engineAssert(const char* expression, const char* file, int line )
{
	std::string s;
	printf("Assertation failed at %s:%d: %s", file, line, expression );
//	LOG_ERROR(s.c_str());
	throw std::exception(s.c_str());
}


