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
#ifndef _ENGINE_MESH_H_
#define _ENGINE_MESH_H_
#include <core/Ref.h>
#include <slmath/mat4.h>
#include <graphics/OpenGLES/es_util.h>
#include <core/FileStream.h>
#include <stdint.h>
#include <graphics/Shader.h>

namespace graphics
{
	//
	// Index buffer contains indices to mesh vertex buffer.
	// Index buffer uses following opengl functions for GL_ELEMENT_ARRAY_BUFFER:
	//  - glGenBuffers
	//  - glDeleteBuffers
	//  - glBindBuffer
	//  - glBufferData
	//  - glDrawElements
	class IndexBuffer : public core::Object
	{
	public:
		IndexBuffer(const std::vector<uint16_t>& data);
		IndexBuffer(uint16_t* data, int dataLen);
		virtual ~IndexBuffer();
		void drawElements();
	private:
		GLuint	m_ibo;
		int		m_dataLen;
	};


	//
	// Vertex array for storing vertex data (positions, normals texture coords etc).
	class VertexArray : public core::Object
	{
	public:
		VertexArray();
		virtual ~VertexArray();
		virtual SHADER_ATTRIBUTES getSemantic() const = 0;
		virtual int getVertexCount() const = 0;
		virtual int getStride() const = 0;
		virtual int getComponentCount() const = 0;
		virtual const float* getVertex(int index) const = 0;
	};


	//
	// Actual implementation for vertex array is template class for given data type (vec2, vec3 etc).
	template <class T>
	class VertexArrayImpl : public VertexArray
	{
	public:
		VertexArrayImpl(SHADER_ATTRIBUTES semantic, T* data, int vertexCount);
		VertexArrayImpl(SHADER_ATTRIBUTES semantic, const std::vector<T>& data);
		virtual ~VertexArrayImpl();
		virtual SHADER_ATTRIBUTES getSemantic() const;
		virtual int getVertexCount() const;
		virtual int getStride() const;
		virtual int getComponentCount() const;
		virtual const float* getVertex(int index) const;
	private:
		SHADER_ATTRIBUTES	m_semantic;
		std::vector<T>		m_data;
	};

	template <class T>
	VertexArrayImpl<T>::VertexArrayImpl(SHADER_ATTRIBUTES semantic, T* data, int vertexCount)
	{
		m_semantic = semantic;
		m_data.resize(vertexCount);
		for (int i = 0; i < vertexCount; ++i)
		{
			m_data[i] = data[i];
		}
	}

	template <class T>
	VertexArrayImpl<T>::VertexArrayImpl(SHADER_ATTRIBUTES semantic, const std::vector<T>& data)
	{
		m_semantic = semantic;
		m_data = data;
	}

	template <class T>
	VertexArrayImpl<T>::~VertexArrayImpl()
	{
	}

	template <class T>
	SHADER_ATTRIBUTES VertexArrayImpl<T>::getSemantic() const
	{
		return m_semantic;
	}

	template <class T>
	int VertexArrayImpl<T>::getVertexCount() const
	{
		return (int)m_data.size();
	}

	template <class T>
	int VertexArrayImpl<T>::getStride() const
	{
		return sizeof(T);
	}

	template <class T>
	int VertexArrayImpl<T>::getComponentCount() const
	{
		return getStride() / sizeof(float);
	}

	template <class T>
	const float* VertexArrayImpl<T>::getVertex(int index) const
	{
		return (const float*)&m_data[index];
	}


	//
	// Vertex buffer contains several vertex arrays.
	// Vertex buffer uses following opengl functions for GL_ARRAY_BUFFER:
	//  - glGenBuffers
	//  - glDeleteBuffers
	//  - glBindBuffer
	//  - glBufferData
	//  - glEnableVertexAttribArray
	//  - glDisableVertexAttribArray
	//  - glVertexAttribPointer
	class VertexBuffer : public core::Object
	{
	public:
		VertexBuffer(VertexArray** vertexArrays, int count);
		virtual ~VertexBuffer();
		void bind();
		void unbind();
	private:
		bool isEnabled(int i);

		GLuint									m_vbo;
		std::vector< core::Ref<VertexArray> >	m_vertexArrays;
		std::vector<float>						m_data;
		int										m_componentsPerVertex;
	};


	// 
	// Mesh-luokka toteuttaa indeksoidun meshin. Se pit‰‰ sis‰ll‰‰n vertex bufferin ja index bufferin.
	// - VertexBuffer sis‰lt‰‰ taulukollisen Vertex arrayt‰. Yksi vertex array kuvaa, yht‰ taulukollista
	//   esimerkiksi objektin positioita, normaaleja tai vaikka tekstuurikoordinaatteja. Vertex arrayt on
	//   sidottu shaderiin SHADER_ATTRIBUTES-struktuurin avulla. VertexBufferin vertexarrayiden koon on
	//   oltava kaikilla sama (yht‰ monta positiota, kuin normaalia ja tekstuurikoordinaattia jne.)
	// - IndexBuffer sis‰lt‰‰ taulukollisen indeksej‰ vertex arrayhin. Mallin kolmiot m‰‰ritet‰‰n kolmella 
	//   k‰rkipisteell‰, joten IndexBufferin datan lukum‰‰r‰n pit‰‰ olla kolmella jaollinen.
	//
	// Example of creating mesh:
	//    //Create Index buffer for teapot mesh
	//    graphics::IndexBuffer* ib = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);
	//	
	//    // Create Vertex arrays for teapot: Positions as vec3 and Normals as vec3
	//    graphics::VertexArray* va[] =
	//    {
	//	      new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),
	//	      new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices)
	//    };
	//
	//    // Create vertex buffer from vertex arrays
	//    graphics::VertexBuffer* vb = new graphics::VertexBuffer(&va[0], sizeof(va) / sizeof(va[0]));
	//	
	//    // Create mesh from ib and vb
	//    m_mesh = new graphics::Mesh(ib, vb);
	class Mesh : public core::Object
	{
	public:
		Mesh(IndexBuffer* ib, VertexBuffer* vb);
		virtual ~Mesh();
		void render();
	private:
		core::Ref<IndexBuffer> m_ib;
		core::Ref<VertexBuffer> m_vb;
	};


}


#endif

