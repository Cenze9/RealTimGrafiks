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
#include "scene.h"
#include <graphics/Shader.h>
//#include <core/log.h>

class TriangleScene : public Scene
{
public:
	TriangleScene()
	{
		
		printf("TriangleScene construct");
		checkOpenGL();

		m_count = 0.0f;

		const char* g_strVSProgram = 
			"attribute vec4 g_vVertex;											 \n"
			"attribute vec4 g_vColor;											 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
			"                         g_vVertex.z, g_vVertex.w );                \n"
			"    g_vVSColor = g_vColor;                                          \n"
			"}																	 \n";


		const char* g_strFSProgram = 
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"																	 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_FragColor = g_vVSColor;                                      \n"
			"}																	 \n";

		graphics::SHADER_ATTRIBUTE attributes[2] = {
			{"g_vVertex", 0 },
			{"g_vColor",  1 }
		};

		
		m_shader = new graphics::Shader(g_strVSProgram, g_strFSProgram, 
			attributes, sizeof(attributes) / sizeof(graphics::SHADER_ATTRIBUTE),
			false); // Don't load from file
		checkOpenGL();
	}


    virtual ~TriangleScene()
	{
		printf("TriangleScene destruct");
	}


    virtual void update( graphics::ESContext* esContext, float deltaTime )
	{
		m_count += deltaTime;

		if( m_count > 1.0f )
			m_count = 0.0f;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport( 0, 0, esContext->width, esContext->height );
		checkOpenGL();

		float aspectRatio = float(esContext->width)/float(esContext->height);
		float fSize = 0.5f;
		float VertexPositions[] =
		{
			 0.0f,  +fSize*aspectRatio, 0.0f, 1.0f,
			-fSize, -fSize*aspectRatio, 0.0f, 1.0f,
			+fSize, -fSize*aspectRatio, 0.0f, 1.0f,
		};

		float VertexColors[] = {1.0f, 0.0f, 0.0f, 1.0f,
								0.0f, 1.0f, 0.0f, 1.0f,
								0.0f, 0.0f, 1.0f, 1.0f
							   };

		// Clear the backbuffer and depth-buffer
		glClearColor( 0.0f, m_count, m_count, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		checkOpenGL();

		// Set the shader program and the texture
		m_shader->bind();
		checkOpenGL();

		// Draw the colored triangle
		glVertexAttribPointer( 0, 4, GL_FLOAT, 0, 0, VertexPositions );
		checkOpenGL();

		glEnableVertexAttribArray( 0 );
		checkOpenGL();

		glVertexAttribPointer( 1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();
		glEnableVertexAttribArray( 1 );
		checkOpenGL();

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 3 );
		checkOpenGL();

		glDisableVertexAttribArray( 0 );
		checkOpenGL();
		glDisableVertexAttribArray( 1 );
		checkOpenGL();
	}
private:
	float m_count;
	
	core::Ref<graphics::Shader> m_shader;

};


