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
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <core/Object.h>
#include <core/Ref.h>
#include <string>
#include <graphics/OpenGLES/es_util.h>

namespace graphics
{
	class Image;

	class Texture : public core::Object
	{
	public:
		Texture();
		Texture(GLuint id);

		virtual ~Texture();

		GLuint getTextureId() const;


		enum FilteringMode
		{
			NO_FILTERING,
			LINEAR_FILTERING,
			BILINEAR_FILTERING,
			TRILINEAR_FILTERING
		};

		enum WrappingMode
		{
			CLAMP,
			REPEAT
		};
	private:

		Texture(const Texture&);
		Texture& operator=(const Texture&);

		GLuint m_nativeId;
	};


	class Texture2D : public Texture
	{
	public:
		Texture2D(int width, int height, GLint fmt, GLenum type, Texture::FilteringMode filtering = TRILINEAR_FILTERING, Texture::WrappingMode wrapping = REPEAT);
		Texture2D();
		virtual ~Texture2D();
		void setData(Image*, Texture::FilteringMode filtering = TRILINEAR_FILTERING, Texture::WrappingMode wrapping = REPEAT);
		//void setData(Image*, Texture::FilteringMode filtering = TRILINEAR_FILTERING, Texture::WrappingMode wrapping = REPEAT);

		//	void setData(Image*);
	private:
		Texture2D(const Texture2D&);
		Texture2D& operator=(const Texture2D&);
	};

	class TextureCube : public Texture
	{
	public:
		TextureCube();
		~TextureCube();

		void setData(Image* images[6], Texture::FilteringMode filtering = LINEAR_FILTERING);
	private:

	};

	class TextureDepth : public Texture
	{
	public:
		TextureDepth(int width, int height, GLenum type);
		virtual ~TextureDepth();
	private:
		TextureDepth(const TextureDepth&);
		TextureDepth& operator=(const TextureDepth&);
	};

	class RenderTarget : public core::Object
	{
	public:
		RenderTarget(int width, int height, bool createDepthBuffer, const GLenum format = GL_RGBA, const GLenum type = GL_UNSIGNED_BYTE);
		~RenderTarget();
		void shareDepthBuffer(TextureDepth* depthBuffer);

		void bind();
		void unbind();

		Texture* getColorBuffer() const { return 	m_colorBuffer.ptr(); }
		Texture* getDepthBuffer() const { return 	m_depthBuffer.ptr(); }

	private:
		// frame buffer object
		GLuint  m_FBO;

		// re-using buffers as samplers
		core::Ref<Texture>		m_colorBuffer;
		core::Ref<Texture>		m_depthBuffer;

		int m_width;
		int m_height;
	};



}


#endif 


