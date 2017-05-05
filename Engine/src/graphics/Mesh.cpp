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
#include <graphics/Mesh.h>
#include <core/Ref.h>
#include <slmath/mat4.h>
#include <graphics/OpenGLES/es_util.h>
#include <core/FileStream.h>

namespace graphics
{

	inline char* BUFFER_OFFSET(int i)
	{
		return ((char *)NULL + (i));
	}


	IndexBuffer::IndexBuffer(const std::vector<uint16_t>& data)
	{
		m_dataLen = data.size()*sizeof(uint16_t);
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_dataLen, &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(uint16_t* data, int dataLen)
		: Object()
		, m_dataLen(dataLen)
	{
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataLen*sizeof(uint16_t), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ibo);
	}

	void IndexBuffer::drawElements()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glDrawElements(GL_TRIANGLES, m_dataLen, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}



	VertexArray::VertexArray()
	{
	}

	VertexArray::~VertexArray()
	{
	}





	VertexBuffer::VertexBuffer(VertexArray** vertexArrays, int count)
		: Object()
	{
		m_vertexArrays.resize(count);

		assert(count > 0);
		m_componentsPerVertex = 0;
		int numOfVertices = vertexArrays[0]->getVertexCount();
		for (int vaIndex = 0; vaIndex < count; ++vaIndex)
		{
			VertexArray* va = vertexArrays[vaIndex];
			m_vertexArrays[vaIndex] = va;
			assert(va->getVertexCount() == numOfVertices);
			m_componentsPerVertex += va->getComponentCount();
		}

		m_data.resize(m_componentsPerVertex * numOfVertices);

		for (int vertexIndex = 0; vertexIndex < numOfVertices; ++vertexIndex)
		{
			float* destVertex = &m_data[vertexIndex*m_componentsPerVertex];

			int offs = 0;
			for (int vaIndex = 0; vaIndex < count; ++vaIndex)
			{
				VertexArray* va = m_vertexArrays[vaIndex];
				const float* srcVertex = va->getVertex(vertexIndex);
				memcpy(&destVertex[offs], srcVertex, va->getStride());
				offs += va->getComponentCount();
			}
		}

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_data.size()*sizeof(float), &m_data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	VertexBuffer::~VertexBuffer()
	{
	}

	void VertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		int offs = 0;
		for (size_t i = 0; i < m_vertexArrays.size(); ++i)
		{
			VertexArray* va = m_vertexArrays[i];
			//assert(!isEnabled(va->getSemantic()));
			glEnableVertexAttribArray(va->getSemantic());
			assert(isEnabled(va->getSemantic()));
			glVertexAttribPointer(va->getSemantic(), va->getComponentCount(),
				GL_FLOAT, GL_FALSE, m_componentsPerVertex*sizeof(float), BUFFER_OFFSET(offs*sizeof(GLfloat)));

			offs += va->getComponentCount();
		}
	}

	void VertexBuffer::unbind()
	{
		for (size_t i = 0; i < m_vertexArrays.size(); ++i)
		{
			VertexArray* va = m_vertexArrays[i];
			//assert(isEnabled(va->getSemantic()));
			glDisableVertexAttribArray(va->getSemantic());
			assert(!isEnabled(va->getSemantic()));
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	bool VertexBuffer::isEnabled(int i)
	{
		GLint numAttribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttribs);
		assert(i < numAttribs);
		GLint params;
		glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &params);
		return params ? true : false;
	}





	Mesh::Mesh(IndexBuffer* ib, VertexBuffer* vb)
		: Object()
		, m_ib(ib)
		, m_vb(vb)
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::render()
	{
		m_vb->bind();
		m_ib->drawElements();
		m_vb->unbind();
	}


}
