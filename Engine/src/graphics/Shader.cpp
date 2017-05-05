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
#include <graphics/Shader.h>
//#include <graphics/ShaderUtils.h>
#include <core/Ref.h>
#include <slmath/mat4.h>
#include <vector>
#include <string>
#include <core/FileStream.h>

namespace graphics
{
	namespace
	{
		bool FrmLoadFile(const char* strFileName, void** ppData)
		{
			core::Ref<core::FileStream> fs = new core::FileStream(strFileName, core::FileStream::READ_ONLY);
			int size = fs->available();
			if (0 == size)
			{
				return false;
			}

			if (ppData)
			{
				char* pData = new char[size + 1];
				fs->read(pData, size);
				pData[size] = 0;

				(*ppData) = pData;
			}

			return true;
		}


		//--------------------------------------------------------------------------------------
		// Name: FrmUnloadFile()
		// Desc: 
		//--------------------------------------------------------------------------------------
		void FrmUnloadFile(void* pData)
		{
			delete[](char*)pData;
		}


		//--------------------------------------------------------------------------------------
		// Name: FrmCompileShaderFromString()
		// Desc: 
		//--------------------------------------------------------------------------------------
		bool FrmCompileShaderFromString(const char* strShaderSource, GLuint hShaderHandle)
		{
			glShaderSource(hShaderHandle, 1, &strShaderSource, NULL);
			glCompileShader(hShaderHandle);

			// Check for compile success
			GLint nCompileResult = 0;
			glGetShaderiv(hShaderHandle, GL_COMPILE_STATUS, &nCompileResult);
			if (0 == nCompileResult)
			{
				char strInfoLog[1024];
				GLint nLength;
				glGetShaderInfoLog(hShaderHandle, 1024, &nLength, strInfoLog);
				printf("Unable to compile shader: %s", strInfoLog);
				printf(strShaderSource);
				assert(0);
				return false;
			}

			return true;
		}


		//--------------------------------------------------------------------------------------
		// Name: FrmBindShaderAttributes()
		// Desc: Bind the attributes to the shader program
		//--------------------------------------------------------------------------------------
		void FrmBindShaderAttributes(GLuint hShaderProgram,
			const SHADER_ATTRIBUTE* pAttributes,
			size_t nNumAttributes)
		{

			for (size_t i = 0; i<nNumAttributes; i++)
			{
				glBindAttribLocation(hShaderProgram, pAttributes[i].nLocation,
					pAttributes[i].strName);
			}


		}


		//--------------------------------------------------------------------------------------
		// Name: FrmLinkShaderProgram()
		// Desc: Helper function to link a shader program
		//--------------------------------------------------------------------------------------
		bool FrmLinkShaderProgram(GLuint hShaderProgram)
		{
			// Link the whole program together
			glLinkProgram(hShaderProgram);

			// Check for link success
			GLint LinkStatus;
			glGetProgramiv(hShaderProgram, GL_LINK_STATUS, &LinkStatus);
			if (!LinkStatus)
			{
				char  strInfoLog[1024];
				GLint nLength;
				glGetProgramInfoLog(hShaderProgram, 1024, &nLength, strInfoLog);
				printf(strInfoLog, "\n");
				return false;
			}

			return true;
		}

		bool FrmLoadShaderObjectFromFile(const char* strFileName, GLuint hShaderHandle)
		{
			char* strShaderSource;
			if (!FrmLoadFile(strFileName, (void**)&strShaderSource))
			{
				printf("ERROR: Could not load shader file");
				return false;
			}

			bool bResult = FrmCompileShaderFromString(strShaderSource, hShaderHandle);
			if (!bResult)
			{
				printf("ERROR: Could compile shader file");
			}

			FrmUnloadFile(strShaderSource);
			return bResult;
		}

		//--------------------------------------------------------------------------------------
		// Name: FrmCompileShaderProgramFromFile()
		// Desc: 
		//--------------------------------------------------------------------------------------
		bool FrmCompileShaderProgramFromFile(const char* strVertexShaderFileName,
			const char* strFragmentShaderFileName,
			GLuint* pShaderProgramHandle,
			const SHADER_ATTRIBUTE* pAttributes,
			size_t nNumAttributes)
		{
			// Create the object handles
			GLuint hVertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLuint hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			// Compile the shaders
			if (!FrmLoadShaderObjectFromFile(strVertexShaderFileName, hVertexShader))
			{
				glDeleteShader(hVertexShader);
				glDeleteShader(hFragmentShader);
				return false;
			}
			if (!FrmLoadShaderObjectFromFile(strFragmentShaderFileName, hFragmentShader))
			{
				glDeleteShader(hVertexShader);
				glDeleteShader(hFragmentShader);
				return false;
			}


			// Attach the individual shaders to the common shader program
			GLuint hShaderProgram = glCreateProgram();
			glAttachShader(hShaderProgram, hVertexShader);
			glAttachShader(hShaderProgram, hFragmentShader);

			// Bind the attributes to the shader program
			FrmBindShaderAttributes(hShaderProgram, pAttributes, nNumAttributes);

			// Link the vertex shader and fragment shader together
			if (!FrmLinkShaderProgram(hShaderProgram))
			{
				glDeleteProgram(hShaderProgram);
				return false;
			}

			// It's now safe to toss away the original shader handles
			glDeleteShader(hVertexShader);
			glDeleteShader(hFragmentShader);

			// Return the shader program
			(*pShaderProgramHandle) = hShaderProgram;
			return true;
		}

	}

	Shader::Shader(const char* const strVertexShaderFileName,
		const char* const strFragmentShaderFileName,
		const SHADER_ATTRIBUTE* attributes,
		int numAttributes, bool compileFromFile)
		: Object()
		, m_program(0)
	{
		if (compileFromFile)
		{
			FrmCompileShaderProgramFromFile(strVertexShaderFileName,
				strFragmentShaderFileName, &m_program, attributes, numAttributes);
		}
		else
		{
			// Compile from string
			compileShaderProgram(strVertexShaderFileName,
				strFragmentShaderFileName, &m_program, attributes, numAttributes);
		}
	}

	Shader::~Shader()
	{
	}

	GLuint Shader::getProgram() const
	{
		return m_program;
	}

	void Shader::bind()
	{
		glUseProgram(m_program);
	}


	ShaderUniforms::ShaderUniforms(Shader* shader)
		: m_shader(shader)
		, m_initDone(false)
	{
		assert(shader != 0);
	}

	ShaderUniforms::~ShaderUniforms()
	{
	}

	void ShaderUniforms::bind()
	{
		if (!m_initDone)
		{
			getUniformLocations(m_shader);
			m_initDone = true;
		}

		bind(m_shader);
	}

	Shader* ShaderUniforms::getShader() const
	{
		return m_shader.ptr();
	}



	//--------------------------------------------------------------------------------------
	// Name: FrmCompileShaderProgram()
	// Desc: 
	//--------------------------------------------------------------------------------------
	bool Shader::compileShaderProgram(const char* strVertexShader,
		const char* strFragmentShader,
		GLuint* pShaderProgramHandle,
		const SHADER_ATTRIBUTE* pAttributes,
		size_t nNumAttributes)
	{
		// Create the object handles
		GLuint hVertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Compile the shaders
		if (!FrmCompileShaderFromString(strVertexShader, hVertexShader))
		{
			glDeleteShader(hVertexShader);
			glDeleteShader(hFragmentShader);
			return false;
		}
		if (!FrmCompileShaderFromString(strFragmentShader, hFragmentShader))
		{
			glDeleteShader(hVertexShader);
			glDeleteShader(hFragmentShader);
			return false;
		}

		// Attach the individual shaders to the common shader program
		GLuint hShaderProgram = glCreateProgram();
		glAttachShader(hShaderProgram, hVertexShader);
		glAttachShader(hShaderProgram, hFragmentShader);

		// Bind the shader attributes
		FrmBindShaderAttributes(hShaderProgram, pAttributes, nNumAttributes);

		// Link the vertex shader and fragment shader together
		if (!FrmLinkShaderProgram(hShaderProgram))
		{
			glDeleteProgram(hShaderProgram);
			return false;
		}

		// It's now safe to toss away the original shader handles
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);

		// Return the shader program
		(*pShaderProgramHandle) = hShaderProgram;
		return true;
	}
}