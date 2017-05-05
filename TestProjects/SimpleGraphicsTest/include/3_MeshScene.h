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
#ifndef _MESH_SCENE_H_
#define _MESH_SCENE_H_


#include <scene.h>
#include <teapot.h>
#include <core/Ref.h>
#include <slmath/mat4.h>
#include <core/ElapsedTimer.h>
#include <graphics/Shader.h>
#include <graphics/Mesh.h>
#include "ExampleMaterials.h"


class MeshScene : public Scene
{
public:
	MeshScene();
	virtual ~MeshScene();
	virtual void update(graphics::ESContext* esContext, float deltaTime);
	virtual void render(graphics::ESContext* esContext);
private:
	// Projection and view matrixes.
	slmath::mat4 m_matProjection;
    slmath::mat4 m_matView;
	
	// For each mesh has its model matrix and material.
	std::vector<slmath::mat4>						m_matModel;
	std::vector<core::Ref<graphics::ShaderUniforms> > m_materials;

	// Some counters.
	//float			m_count;
	float			m_totalTime;

	// Teapot mesh
	core::Ref<graphics::Mesh>	m_mesh;

	// Global shader values, which is shared between shaders.
	core::Ref<SharedShaderValues> m_sharedValues;

};


#endif

