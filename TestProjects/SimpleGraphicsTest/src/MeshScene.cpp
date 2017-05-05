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
#include "3_MeshScene.h"

namespace
{
#if 0
float dX = 1000.0f;
float dY = -500.0f;
float dZ = -300.0f;
#else
float dX = 0;
float dY = 0;
float dZ = 0;
#endif

const float CAM_X = dX + 0.f;
const float CAM_Y = dY + 70.f;
const float CAM_Z = dZ + 70.f;

const float LIGHT_X = dX + 0.f;
const float LIGHT_Y = dY + 70.f;
const float LIGHT_Z = dZ + 70.f;


const float OBJ_X = dX + 0.f;
const float OBJ_Y = dY + 0.f;
const float OBJ_Z = dZ + 0.f;

const float OBJ_DELTA = 30.0f;
const float CAM_LOOK_DELTA = 15.0f;

}


MeshScene::MeshScene()
	: Scene()
{		
	printf("MeshScene construct");
	checkOpenGL();
	m_materials.resize(2);
	m_sharedValues = new SharedShaderValues();
	m_totalTime = 0.0f;
		
	graphics::SHADER_ATTRIBUTE attributes[2] =
	{
		{ "g_vPositionOS", graphics::ATTRIB_POSITION },
		{ "g_vNormalOS", graphics::ATTRIB_NORMAL }
	};

	// Load materials
	{
		core::Ref<graphics::Shader> shader =
			new graphics::Shader("assets/Blinn-Phong.vs", "assets/Blinn-Phong.fs",
			attributes, sizeof(attributes) / sizeof(graphics::SHADER_ATTRIBUTE));
		
		SimpleMaterialUniforms* simpleMaterialUniforms = new SimpleMaterialUniforms(shader,m_sharedValues);
			
		// Material values for mesh
		simpleMaterialUniforms->vAmbient	= slmath::vec4(1.0f, 0.2f, 0.5f, 1.0f);
		simpleMaterialUniforms->vDiffuse	= slmath::vec4(1.0f, 0.2f, 0.5f, 1.0f);
		simpleMaterialUniforms->vSpecular	= slmath::vec4(1.0f, 1.0f, 1.0f, 10.0f);
		m_materials[0] = simpleMaterialUniforms;
	}

	{	
		core::Ref<graphics::Shader> shader =
			new graphics::Shader("assets/Blinn-Phong.vs", "assets/Blinn-Phong.fs", 
			attributes, sizeof(attributes) / sizeof(graphics::SHADER_ATTRIBUTE));
			
		SimpleMaterialUniforms* simpleMaterialUniforms = new SimpleMaterialUniforms(shader,m_sharedValues);
			
		// Material values for mesh
		simpleMaterialUniforms->vAmbient	= slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		simpleMaterialUniforms->vDiffuse	= slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		simpleMaterialUniforms->vSpecular	= slmath::vec4(1.0f, 1.0f, 1.0f, 5.0f);
		m_materials[1] = simpleMaterialUniforms;
	}

	//Create Index buffer for teapot mesh
	graphics::IndexBuffer* ib = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);
		
	//Create Vertex arrays for teapot
	graphics::VertexArray* va[] =
	{
		// Positions as vec3
		new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),
		// Normals as vec3
		new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices)
	};

	// Create vertex buffer from vertex arrays
	graphics::VertexBuffer* vb = new graphics::VertexBuffer(&va[0], sizeof(va) / sizeof(va[0]));
		
	// Create mesh from ib and vb
	m_mesh = new graphics::Mesh(ib, vb);
		
	checkOpenGL();
}


MeshScene::~MeshScene()
{
	printf("MeshScene destruct");
}


void MeshScene::update(graphics::ESContext* esContext, float deltaTime)
{
	m_totalTime += deltaTime;

	{
		// Set camera view and projection matrix.
		// Needs actually set only once on init in this case
		const float CAM_NEAR = 5.f;
		const float CAM_FAR = 1000.f;
		float fAspect = (float)esContext->width/ (float)esContext->height;
		m_matProjection = slmath::perspectiveFovRH(slmath::radians(45.0f), fAspect, CAM_NEAR, CAM_FAR);
		m_matView = slmath::lookAtRH(slmath::vec3(CAM_X, CAM_Y, CAM_Z), slmath::vec3(OBJ_X, OBJ_Y+CAM_LOOK_DELTA, OBJ_Z), slmath::vec3(0.f, 1.f, 0.f));
	}

	m_matModel.resize(2);

	// Update teapot on left
	m_matModel[0] = slmath::rotationX(-3.1415f*0.5f);
	m_matModel[0] = slmath::rotationY(m_totalTime) * m_matModel[0];
	m_matModel[0] = slmath::translation(slmath::vec3(OBJ_X-OBJ_DELTA,OBJ_Y,OBJ_Z)) * m_matModel[0];
		
	// Update teapot on right
	m_matModel[1] = slmath::rotationX(-3.1415f*0.5f);
	m_matModel[1] = slmath::rotationY(m_totalTime) * m_matModel[1];
	m_matModel[1] = slmath::translation(slmath::vec3(OBJ_X+OBJ_DELTA,OBJ_Y,OBJ_Z)) * m_matModel[1];
}


void MeshScene::render(graphics::ESContext* esContext)
{
	checkOpenGL();

	// Set the viewport
	glViewport( 0, 0, esContext->width, esContext->height );
   
	// Clear the backbuffer and depth-buffer
	glClearColor( 0.0f, 0.8f, 0.8f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	checkOpenGL();

	// Initialize GL state.
	glDisable(GL_BLEND);   // no blending
	glEnable(GL_CULL_FACE); // bacface culling enabled
	glEnable(GL_DEPTH_TEST); // depth test enabled
	glDepthFunc(GL_LEQUAL); // less or equal
	checkOpenGL();


	// Calculate needed stuff for m_sharedValues
	slmath::vec3 lightPos				= slmath::vec3(LIGHT_X, LIGHT_Y, LIGHT_Z);
	m_sharedValues->matView				= m_matView;
	m_sharedValues->matProj				= m_matProjection;
	m_sharedValues->lightPos			= lightPos;
	m_sharedValues->camPos				= slmath::vec3(CAM_X, CAM_Y, CAM_Z);

	for( int i=0; i<2; ++i )
	{
		// These values are needed to update for each mesh which is rendered (different model matrix)
		m_sharedValues->matModel			= m_matModel[i];
		slmath::mat4 matModelView			= m_matView * m_matModel[i];
		slmath::mat4 matModelViewProj		= m_matProjection * matModelView;
		slmath::mat4 matNormal				= slmath::transpose(slmath::inverse(matModelView));
		m_sharedValues->matModelView		= matModelView;
		m_sharedValues->matNormal			= matNormal;
		m_sharedValues->matModelViewProj	= matModelViewProj;

		// Bind material (sets uniform values)
		m_materials[i]->bind();
		checkOpenGL();

		// Render the mesh using active material.
		m_mesh->render();
		checkOpenGL();
	}
}


