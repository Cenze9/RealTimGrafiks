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
#ifndef _SHADER_H_
#define _SHADER_H_
#include <core/Object.h>
#include <core/Ref.h>
#include <GLES2/gl2.h>

namespace graphics
{
	struct SHADER_ATTRIBUTE
	{
		const char* strName;
		GLuint      nLocation;
	};

	enum SHADER_ATTRIBUTES {
		ATTRIB_POSITION,
		ATTRIB_NORMAL,
		ATTRIB_UV,
		ATTRIB_TANGENT,
		ATTRIB_BINORMAL
	};

	//
	// Shader class.
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	class Shader : public core::Object
	{
	public:
		// Creates and compiles shader either from source strings or from file .vs and .fs files.
		Shader(const char* const strVertexShaderFileName,
			const char* const strFragmentShaderFileName,
			const SHADER_ATTRIBUTE* attributes,
			int numAttributes, bool compileFromFile = true);

		virtual ~Shader();

		GLuint getProgram() const;

		// Binds shader
		void bind();

		// Helper function for shader compiling from sources.
		static bool compileShaderProgram(const char* strVertexShader,
			const char* strFragmentShader,
			GLuint* pShaderProgramHandle,
			const SHADER_ATTRIBUTE* pAttributes = 0,
			size_t nNumAttributes = 0);
	private:
		GLuint m_program;
	};


	//
	// Abstract class for shader uniforms.
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	class ShaderUniforms : public core::Object
	{
	public:
		ShaderUniforms(Shader* shader);

		virtual ~ShaderUniforms();

		void bind();

		Shader* getShader() const;

		// Retrieves uniform values to member variables. Called once per material instantiation.
		virtual void getUniformLocations(Shader* shader) = 0;

		// Binds shader uniform values. Called each time, when material is bind (used).
		virtual void bind(Shader* shader) = 0;

	private:
		core::Ref<Shader>	m_shader;
		bool				m_initDone;

	};


}

#endif

