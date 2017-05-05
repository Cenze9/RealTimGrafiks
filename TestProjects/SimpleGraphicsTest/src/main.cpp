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
#include "1_TriangleScene.h"
#include "2_SimpleTextureScene.h"
#include "3_MeshScene.h"
#include "4_TexturedMeshScene.h"

#include <core/Ref.h>
#include <core/RefCounter.h>
#include <core/Input.h>

core::Ref<Scene> m_currentScene = 0;
static const int NUM_SCENES = 4;
static int sceneIndex = 0;


core::RefCounter refs;
	
#if defined(ANDROID)
Engine::ElapsedTimer timer;
#endif

void changeCurrentScene(int index)
{
#if defined(ANDROID)
	timer.reset();
#endif
	
	m_currentScene = 0;
	assert( index >= 0 && index<NUM_SCENES);
	switch( index )
	{
	case 0: m_currentScene = new TriangleScene(); break;
	case 1: m_currentScene = new SimpleTextureScene(); break;
	//case 2: m_currentScene = new MeshScene(); break;
	//case 3: m_currentScene = new TexturedMeshScene(); break;
	default:
		break;
	}
}

void update(graphics::ESContext* esContext, float deltaTime);



// Initialize the game
bool init(graphics::ESContext *esContext)
{
	changeCurrentScene(sceneIndex);
	glEnable(GL_DEPTH_TEST);
	update(esContext,0.0f);
	return true;
}


// Deinitialize the game
void deinit(graphics::ESContext *esContext)
{
	m_currentScene = 0;
}


// Draw game
void draw(graphics::ESContext *esContext)
{
	if( m_currentScene )
		m_currentScene->render(esContext);
}


// update game
void update(graphics::ESContext* esContext, float deltaTime)
{
#if defined(_WIN32)
	bool nowPressed = GetAsyncKeyState(VK_SPACE) ? true : false;

	static bool prevPressed = false;
	if( !nowPressed && prevPressed )
	{
		sceneIndex = (sceneIndex+1) % NUM_SCENES;
		changeCurrentScene(sceneIndex);
	}

	prevPressed = nowPressed;
#endif

#if defined(ANDROID)
	if( timer.getTime() > 10.0 )
	{
		sceneIndex = (sceneIndex+1) % NUM_SCENES;
		changeCurrentScene(sceneIndex);
	}
#endif	

	if( m_currentScene )
		m_currentScene->update(esContext,deltaTime);
}


int main ( int argc, char *argv[] )
{
	graphics::ESContext esContext;
	graphics::esInitContext(&esContext);
	graphics::esCreateWindow(&esContext, 1280, 720, graphics::ES_WINDOW_DEFAULT);
   
	graphics::esRegisterInitFunc(&esContext, init);
	graphics::esRegisterDrawFunc(&esContext, draw);
	graphics::esRegisterUpdateFunc(&esContext, update);
	graphics::esRegisterDeinitFunc(&esContext, deinit);
	
	graphics::esMainLoop(&esContext);
	return 0;
}



