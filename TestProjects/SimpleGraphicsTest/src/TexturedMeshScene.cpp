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
#include "4_TexturedMeshScene.h"

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



TexturedMeshScene::TexturedMeshScene()
	: Scene()
{
	printf("TexturedMeshScene construct");

	checkOpenGL();

	m_sharedValues = new SharedShaderValues;
	
	m_totalTime = 0.0f;
	
	graphics::SHADER_ATTRIBUTE attributes[] = 
	{
			{"g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL },
			{ "g_vTexCoordOS", graphics::ATTRIB_UV }
	};

	// Load materials
	{
		core::Ref<graphics::Shader> shader =
			new graphics::Shader("assets/Blinn-Phong_Textured.vs", "assets/Blinn-Phong_Textured.fs", 
			attributes, sizeof(attributes) / sizeof(graphics::SHADER_ATTRIBUTE));
		
		SimpleMaterialWithTextureUniforms* simpleMaterialUniforms = new SimpleMaterialWithTextureUniforms(shader,m_sharedValues);
			
		// Load texture data from TreeBark.tga
		core::Ref<graphics::Image> img = graphics::Image::loadFromTGA("assets/cartridges.tga");
		core::Ref<graphics::Texture2D> tex = new graphics::Texture2D();
		tex->setData(img);



		// Material values for mesh
		simpleMaterialUniforms->vAmbient	= slmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		simpleMaterialUniforms->vDiffuse	= slmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		simpleMaterialUniforms->vSpecular	= slmath::vec4(1.0f, 1.0f, 1.0f, 10.0f);
		simpleMaterialUniforms->diffuseMap	= tex;
		m_materials[0] = simpleMaterialUniforms;
	}

	{	
		core::Ref<graphics::Shader> shader =
			new graphics::Shader("assets/Blinn-Phong_Textured.vs", "assets/Blinn-Phong_Textured.fs", 
			attributes, sizeof(attributes) / sizeof(graphics::SHADER_ATTRIBUTE));
			
		SimpleMaterialWithTextureUniforms* simpleMaterialUniforms = new SimpleMaterialWithTextureUniforms(shader,m_sharedValues);
			
		// Create simpletesture 
		core::Ref<graphics::Texture> tex = createSimpleTexture2D();

		// Material values for mesh
		simpleMaterialUniforms->vAmbient	= slmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		simpleMaterialUniforms->vDiffuse	= slmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		simpleMaterialUniforms->vSpecular	= slmath::vec4(1.0f, 1.0f, 1.0f, 10.0f);
		simpleMaterialUniforms->diffuseMap	= tex;
		m_materials[1] = simpleMaterialUniforms;
	}
		
	//Create Index buffer
	graphics::IndexBuffer* ib = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);
		
	//Create Vertex arrays
	graphics::VertexArray* va[] =
	{
		new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),
		new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices),
		new graphics::VertexArrayImpl<slmath::vec3>(graphics::ATTRIB_UV, (slmath::vec3*)TeapotData::texCoords, TeapotData::numVertices)
	};

	// Create vertex buffer from vertex arrays
	graphics::VertexBuffer* vb = new graphics::VertexBuffer(&va[0], sizeof(va) / sizeof(va[0]));
		
	// Create mesh from ib and vb
	m_mesh = new graphics::Mesh(ib, vb);
		
//	m_fpsTimer.reset();
//	m_numFrames = 0;

	checkOpenGL();
}


TexturedMeshScene::~TexturedMeshScene()
{
	printf("TexturedMeshScene destruct");
}


void TexturedMeshScene::update(graphics::ESContext* esContext, float deltaTime)
{
	m_totalTime += deltaTime;
		
	{
		// Needs actually set only once on init in this case
		const float CAM_NEAR = 5.f;
		const float CAM_FAR = 1000.f;
		float fAspect = (float)esContext->width/ (float)esContext->height;
		m_matProjection = slmath::perspectiveFovRH(slmath::radians(45.0f), fAspect, CAM_NEAR, CAM_FAR);
		m_matView = slmath::lookAtRH(slmath::vec3(CAM_X, CAM_Y, CAM_Z), slmath::vec3(OBJ_X, OBJ_Y+CAM_LOOK_DELTA, OBJ_Z), slmath::vec3(0.f, 1.f, 0.f));
	}

	// Update teapot on left
	m_matModel[0] = slmath::rotationX(-3.1415f*0.5f);
	m_matModel[0] = slmath::rotationY(m_totalTime*0.5f) * m_matModel[0];
	m_matModel[0] = slmath::translation(slmath::vec3(OBJ_X-OBJ_DELTA,OBJ_Y,OBJ_Z)) * m_matModel[0];
		
	// Update teapot on right
	m_matModel[1] = slmath::rotationX(-3.1415f*0.5f);
	m_matModel[1] = slmath::rotationY(-m_totalTime*0.5f) * m_matModel[1];
	m_matModel[1] = slmath::translation(slmath::vec3(OBJ_X+OBJ_DELTA,OBJ_Y,OBJ_Z)) * m_matModel[1];
}
	
void TexturedMeshScene::render(graphics::ESContext* esContext)
{
	checkOpenGL();
	
	// Set the viewport
	glViewport( 0, 0, esContext->width, esContext->height );
   
	// Clear the backbuffer and depth-buffer
	glClearColor( 0.0f, 0.8f, 0.8f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	checkOpenGL();

	// Initialize GL state.
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	checkOpenGL();

	// Calculate needed stuff for m_sharedValues
	slmath::vec3 lightPos				= slmath::vec3(LIGHT_X, LIGHT_Y, LIGHT_Z);
	m_sharedValues->matView				= m_matView;
	m_sharedValues->matProj				= m_matProjection;
	m_sharedValues->lightPos			= lightPos;

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

graphics::Texture* TexturedMeshScene::createSimpleTexture2D()
{
	graphics::Texture* tex = new graphics::Texture();
	
	// 2x2 Image, 3 bytes per pixel (R, G, B)
	GLubyte pixels[4 * 4] =
	{  
		255,   0,   0, 255,// Red
		0, 255,   0, 255,// Green
		0,   0, 255, 255,// Blue
		255, 255,   0, 255  // Yellow
	};

	// Bind the texture object
	glBindTexture ( GL_TEXTURE_2D, tex->getTextureId() );

	// Load the texture
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

	// Set the filtering mode
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	checkOpenGL();
	return  tex;
}


