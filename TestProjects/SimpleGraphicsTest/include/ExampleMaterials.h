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
#ifndef _EXAMPLE_MATERIALS_H_
#define _EXAMPLE_MATERIALS_H_

#include <graphics/Shader.h>
#include <graphics/Texture.h>


// Shared "global values", which is used by each shader program. 
// Typically this consists of different matrix values, light
// related values etc. which might be needed by each shader.
// Usage: Create one object of type SharedShaderValues to the 
// scene and pass it for each shader as uniforms to be used.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
struct SharedShaderValues : public core::Object
{
	slmath::mat4 matModel;				// Model matrix (object world matrix)
	slmath::mat4 matView;				// View matrix. (inverse of camera world matrix) 
	slmath::mat4 matProj;				// Projision matrix of the camera
	slmath::mat4 matModelView;			// Model view matrix. Used to transform position vertices to camera space.
	slmath::mat4 matNormal;				// Model view matrix. Used to transform normal/binormal/tangent vertices to camera space.
	slmath::mat4 matModelViewProj;		// Model view projection matrix. Used to transform position vertices to clip space.
	slmath::vec3 lightPos;				// World poition of point light.
	slmath::vec3 camPos;				// World position of camera.
};

// Class for uniforms, which are used by each shader. Sets values
// of SharedShaderValues-object to shader program in bind.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class GlobalShaderUniforms : public graphics::ShaderUniforms
{
public:
	GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* shaderShaderValues = 0)
		: ShaderUniforms(shader)
		, m_shaderShaderValues(shaderShaderValues)
	{
	}

	virtual ~GlobalShaderUniforms()
	{
	}

	virtual void getUniformLocations(graphics::Shader* shader)
	{
		// Get uniform locations
		m_ids[0] = glGetUniformLocation(shader->getProgram(),	"g_matModel");			// Model-matrix
		m_ids[1] = glGetUniformLocation(shader->getProgram(),	"g_matView");			// View-matrix
		m_ids[2] = glGetUniformLocation(shader->getProgram(),	"g_matProj");			// Projection-matrix
		m_ids[3] = glGetUniformLocation(shader->getProgram(),	"g_matModelView");		// Combined model-view-matrix
		m_ids[4] = glGetUniformLocation(shader->getProgram(),	"g_matNormal");			// Normal matrix (only object rotation)
		m_ids[5] = glGetUniformLocation(shader->getProgram(),	"g_matModelViewProj");	// Combined model-view-projection-matrix
		m_ids[6] = glGetUniformLocation(shader->getProgram(),	"g_lightPos");			// Light position
		m_ids[7] = glGetUniformLocation(shader->getProgram(),	"g_camPos");			// Camera position
 	}


	virtual void bind(graphics::Shader* shader)
	{
		shader->bind();
		if( m_shaderShaderValues )
		{
			// Set shared values
			glUniformMatrix4fv( m_ids[0], 1, GL_FALSE, &m_shaderShaderValues->matModel[0][0]);
			glUniformMatrix4fv( m_ids[1], 1, GL_FALSE, &m_shaderShaderValues->matView[0][0]);
			glUniformMatrix4fv( m_ids[2], 1, GL_FALSE, &m_shaderShaderValues->matProj[0][0]);
			glUniformMatrix4fv( m_ids[3], 1, GL_FALSE, &m_shaderShaderValues->matModelView[0][0]);
			glUniformMatrix4fv( m_ids[4], 1, GL_FALSE, &m_shaderShaderValues->matNormal[0][0]);
			glUniformMatrix4fv( m_ids[5], 1, GL_FALSE, &m_shaderShaderValues->matModelViewProj[0][0]);		
			glUniform3f( m_ids[6], m_shaderShaderValues->lightPos.x, m_shaderShaderValues->lightPos.y, m_shaderShaderValues->lightPos.z);
			glUniform3f( m_ids[7], m_shaderShaderValues->camPos.x, m_shaderShaderValues->camPos.y, m_shaderShaderValues->camPos.z);
		}
	}
private:
	const SharedShaderValues*	m_shaderShaderValues;
	GLint						m_ids[8];
};

// Simple material. Consists of material ambient, diffuse and specular colors.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class SimpleMaterialUniforms : public graphics::ShaderUniforms
{
public:
	slmath::vec4  vAmbient; /// Ambient color of the material (rgba)
	slmath::vec4  vDiffuse; /// Ambient color of the material (rgba)
	slmath::vec4  vSpecular; /// Specular color of the material (rgb). Specular exponent (a)

public:
	SimpleMaterialUniforms(graphics::Shader* shader, SharedShaderValues* sharedValues = 0)
		: ShaderUniforms(shader)
		, m_globalShaderUniforms( new GlobalShaderUniforms(shader, sharedValues) )
	{
	}

	virtual ~SimpleMaterialUniforms()
	{
	}

	virtual void getUniformLocations(graphics::Shader* shader)
	{
		m_globalShaderUniforms->getUniformLocations(shader);
		m_materialAmbientLoc	= glGetUniformLocation(shader->getProgram(), "g_Material.vAmbient");
		m_materialDiffuseLoc	= glGetUniformLocation(shader->getProgram(), "g_Material.vDiffuse");
		m_materialSpecularLoc	= glGetUniformLocation(shader->getProgram(), "g_Material.vSpecular");
 	}

	virtual void bind(graphics::Shader* shader)
	{
		shader->bind();
		m_globalShaderUniforms->bind(shader);
		glUniform4fv(m_materialAmbientLoc,  1, &vAmbient.x );
		glUniform4fv(m_materialDiffuseLoc,	1, &vDiffuse.x );
		glUniform4fv(m_materialSpecularLoc, 1, &vSpecular.x );
	}

private:
	core::Ref<GlobalShaderUniforms>	m_globalShaderUniforms;
	GLint								m_materialAmbientLoc;	
    GLint								m_materialDiffuseLoc;	
    GLint								m_materialSpecularLoc;
};

//
// Simple material with texture sampler. Adds diffuse map to SimpleMaterialUniforms.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class SimpleMaterialWithTextureUniforms : public SimpleMaterialUniforms
{
public:
	core::Ref<graphics::Texture> diffuseMap;

	SimpleMaterialWithTextureUniforms(graphics::Shader* shader, SharedShaderValues* sharedValues)
		: SimpleMaterialUniforms(shader,sharedValues)
	{
	}

	virtual ~SimpleMaterialWithTextureUniforms()
	{
	}

	virtual void getUniformLocations(graphics::Shader* shader)
	{
		SimpleMaterialUniforms::getUniformLocations(shader);

		m_diffuseMapLoc	= glGetUniformLocation(shader->getProgram(), "s_diffuseMap");
 	}


	virtual void bind(graphics::Shader* shader)
	{
		SimpleMaterialUniforms::bind(shader);
		// Bind diffuse texture to taxture sampler unit # 0 
		glActiveTexture ( GL_TEXTURE0 + 0);
		glBindTexture ( GL_TEXTURE_2D, diffuseMap->getTextureId() );
		
		// Set sampler unit 0 to be used as sampler for diffuse map uniform.
		glUniform1i( m_diffuseMapLoc, 0 );
	}

private:
    GLint m_diffuseMapLoc;
};



#endif

