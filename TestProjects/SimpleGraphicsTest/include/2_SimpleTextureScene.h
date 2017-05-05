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
#ifndef _SIMPLE_TEXTURE_SCENE_H_
#define _SIMPLE_TEXTURE_SCENE_H_

#include "scene.h"
#include "slmath\mat4.h"
#include <graphics/Shader.h>
#include <graphics/Image.h>

class SimpleTextureScene : public Scene
{
public:

	GLuint CreateSimpleTexture2D()
	{
		// Texture object handle
		GLuint textureId;

		// 2x2 Image, 4 bytes per pixel (R, G, B, A)
		GLubyte pixels[4 * 4] =
		{
			255,   0,   0, 255,// Red
			0, 255,   0, 255,// Green
			0,   0, 255, 255,// Blue
			255, 255,   0, 255  // Yellow
		};

		// Generate a texture object
		glGenTextures(1, &textureId);

		// Bind the texture object
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Load the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// Set the filtering mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureId;
	}



	SimpleTextureScene()
	{
		printf("SimpleTextureScene construct");
		checkOpenGL();

		char vShaderStr[] =
			"attribute vec4 a_position;			\n"
			"attribute vec2 a_texCoord;			\n"
			"uniform mat4 mvp;					\n"
			"varying vec2 v_texCoord;			\n"
			"void main()						\n"
			"{									\n"
			"gl_Position = mvp * a_position;	\n"
			"v_texCoord = a_texCoord;			\n"
			"}									\n";

		char fShaderStr[] =
			"precision mediump float;                            \n"
			"varying vec2 v_texCoord;                            \n"
			"uniform sampler2D s_texture;                        \n"
			"void main()                                         \n"
			"{                                                   \n"
			"  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
			"}                                                   \n";

		// Load the shaders and get a linked program object
		graphics::Shader::compileShaderProgram(vShaderStr, fShaderStr, &programObject);
		
		core::Ref<graphics::Image> img = graphics::Image::loadFromTGA("assets/cartridges.tga");
		tex = new graphics::Texture2D();
		tex->setData(img);

		// Get the attribute locations
		positionLoc = glGetAttribLocation(programObject, "a_position");
		texCoordLoc = glGetAttribLocation(programObject, "a_texCoord");

		// Get the sampler location
		samplerLoc = glGetUniformLocation(programObject, "s_texture");

		mvpLoc = glGetUniformLocation(programObject, "mvp");

		// Load the texture
		textureId = CreateSimpleTexture2D();



		glClearColor((100.0/255.0f), 0.0f, (100.0/255.0f), 0.0f);
		checkOpenGL();
	}


	virtual ~SimpleTextureScene()
	{
		glDeleteTextures(1, &textureId);
		glDeleteProgram(programObject);

		printf("SimpleTextureScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
			totalTime += deltaTime;
			rotateRate += 0.1f*deltaTime;


			matti = slmath::rotationY(rotateRate)*slmath::rotationZ(slmath::radians(45))*slmath::rotationX(slmath::radians(45));

	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		model = slmath::mat4(1.0f)*slmath::rotationY(totalTime)*slmath::rotationZ(totalTime)*slmath::rotationZ(totalTime);
		//model = slmath::mat4(1.0f);
		//camera
		view = slmath::lookAtRH(
			slmath::vec3(0.0f, 0.0f, 5.0f),
			slmath::vec3(0.0f, 0.0f, 0.0f),
			slmath::vec3(0.0f, 1.0f, 0.0f)
		);

		//view = slmath::translation(slmath::vec3(0,0,0));//slmath::mat4(1.0f);
		projection = slmath::mat4(slmath::perspectiveFovRH(slmath::radians(60.0), ((float)esContext->width / (float)esContext->height), 0.0001f, 10000.0f));

		/*
		//mvp = (model*view*projection);
		mvp = view;
		//mvp = slmath::mat4(1.0f);
		*/
		mvp = projection * view * model;// slmath::translation(slmath::vec3(0, 0, -5.0f));

		//	UserData *userData = esContext->userData;
		GLfloat vVertices[] = { 
			-0.5f,  0.5f, 0.5f,  // Position 0
			0.0f,  0.0f,         // TexCoord 0 
								 
			-0.5f, -0.5f, 0.5f,  // Position 1
			0.0f,  10.0f,         // TexCoord 1
								 
			0.5f, -0.5f, 0.5f,   // Position 2
			10.0f,  10.0f,         // TexCoord 2
								 
			0.5f,  0.5f, 0.5f,   // Position 3
			10.0f,  0.0f,         // TexCoord 3
								 
								 
			0.5f,  0.5f, -0.5f,  // Position 4
			0.0f,  0.0f,         // TexCoord 4
								 
			0.5f,  -0.5f, -0.5f, // Position 5
			0.0f,  10.0f,         // TexCoord 5
								 
			-0.5f,  0.5f, -0.5f, // Position 6
			10.0f,  0.0f,         // TexCoord 6

			-0.5f,  -0.5f, -0.5f,// Position 7
			10.0f,  10.0f,         // TexCoord 7


			//TOP

			-0.5f,  0.5f, -0.5f, // Position 6/8
			0.0f,  0.0f,         // TexCoord 6
								 
			0.5f,  0.5f, -0.5f,  // Position 4/9
			10.0f,  0.0f,         // TexCoord 4
								 
			-0.5f,  0.5f, 0.5f,  // Position 0/10
			0.0f,  10.0f,         // TexCoord 0 
								 
			0.5f,  0.5f, 0.5f,   // Position 3/11
			10.0f,  10.0f,         // TexCoord 3
								 
			//BOTTOM				 
			-0.5f, -0.5f, 0.5f,  // Position 1/12
			0.0f,  0.0f,         // TexCoord 1
								 
			0.5f, -0.5f, 0.5f,   // Position 2/13
			10.0f,  0.0f,         // TexCoord 2

			-0.5f,  -0.5f, -0.5f,// Position 7/14
			0.0f,  10.0f,         // TexCoord 7

			0.5f,  -0.5f, -0.5f, // Position 5/15
			10.0f,  10.0f,         // TexCoord 5



		};

		GLushort indices[] = { 
			0, 1, 2, 
			0, 2, 3,

			3, 4, 5,
			2, 3, 5,

			4, 6, 7,
			4, 5, 7,

			6, 1, 7,
			6, 0, 1,

			//top&bottom
			9, 8, 10,
			9, 11, 10,

			13, 12, 14,
			13, 15, 14,


		};

		// perspectiveFovRH(float fovy, float aspect, float znear, float zfar);

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);

		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		

		// Use the program object
		glUseProgram(programObject);

		// Load the vertex position
		glVertexAttribPointer(positionLoc, 3, GL_FLOAT,
			GL_FALSE, 5 * sizeof(GLfloat), vVertices);
		// Load the texture coordinate
		glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT,
			GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

		glEnableVertexAttribArray(positionLoc);
		glEnableVertexAttribArray(texCoordLoc);

		// Bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex->getTextureId());

		int sizeofindices = (sizeof(indices) / sizeof(indices[0]));

		// Set the sampler texture unit to 0
		glUniform1i(samplerLoc, 0);
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)&mvp);

		glDrawElements(GL_TRIANGLES, sizeofindices, GL_UNSIGNED_SHORT, indices);

		checkOpenGL();
	}

private:
	// Handle to a program object
	GLuint programObject;

	
	core::Ref<graphics::Texture2D> tex;

	// Attribute locations
	GLint  positionLoc;
	GLint  texCoordLoc;

	slmath::mat4 matti;

	slmath::mat4 model;
	slmath::mat4 view;
	slmath::mat4 projection;
	slmath::mat4 mvp;




	// Sampler location
	GLint samplerLoc;
	GLint mvpLoc;

	// Texture handle
	GLuint textureId;

	float rotateRate = 0.3f;

	float totalTime=0;
};

#endif



