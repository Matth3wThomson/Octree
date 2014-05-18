#include "Sphere.h"
#include <random>
#include "GameTimer.h"
#include "Verlet.h"
#include <bitset>

#include <sfml/Window.hpp>
#include <sfml/Graphics.hpp>
#include <GL/glew.h>
#include <sfml/OpenGL.hpp>

#include "SRenderer.h"

using std::bitset;

#define UNITS 15
#define RANGE 50

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

/**
CONTROLS:
W/S		Translate camera forward/backward from origin
A/D		Rotate camera around the y axis, 0.5 degree per press
UP/DOWN Rotate camera around the x axis, 0.5 degree per press
Y		Toggle display of Octree
G		Toggle gravity
*/
int main()
{

	//Create window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Gaming Simulations");
	window.setVerticalSyncEnabled(true);

	//Create Renderer
	SRenderer r(SCREEN_WIDTH, SCREEN_HEIGHT, false);

	//INIT GL
	if (GLEW_OK != glewInit()){
		printf("ERROR INIT GLEW");
		return -1;
	}

	//Set some gl properties to nice values
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//Initialise graphical asset managers
	ShaderManager::Create();
	MeshManager::Create();
	TextureManager::Create();

	//Load in all assets required
	ShaderManager::Instance().AddShader("basic", "testVert.glsl", "testFrag.glsl");
	MeshManager::Instance().AddMesh("cube.obj");
	MeshManager::Instance().AddMesh("sphere2.obj");
	MeshManager::Instance().AddMesh("quad");
	TextureManager::Instance().AddTexture("green.png");
	TextureManager::Instance().AddTexture("yellow.png");
	TextureManager::Instance().AddTexture("blue.png");
	TextureManager::Instance().AddTexture("brown.png");

	//Create Physics Engine
	Verlet v(Vector3(RANGE,RANGE,RANGE), 2, 3);

	//Create some Sphere
	for (int i=0; i < UNITS; ++i){
		//Create a sphere at a random position, with random radius, and random mass, drag of 0.99, and elasticty of 0.9
		Sphere* s = v.CreateSphere(
			Vector3(
			((rand() % RANGE) - (RANGE/2)) * 0.5f,
			((rand() % RANGE) - (RANGE/2)) * 0.5f,
			((rand() % RANGE) - (RANGE/2)) * 0.5f),
			static_cast<float>((rand() % ((RANGE/UNITS))) + 1), static_cast<float>(rand() % 40 + 1), 0.99999f, 0.3f);

		//Give them a default velocity
		s->setVelocity(Vector3( static_cast<float>((rand() % 2) - 1), static_cast<float>((rand() % 2) - 1), static_cast<float>((rand() % 2) - 1)), 0.1f);
	}

	//Create all 4 planes to make the cube that the spheres are contained in,
	//at the range of the simulation
	v.CreatePlane(Vector3(0, 1, 0), RANGE/2, Vector3(RANGE/2, RANGE/2, RANGE/2));
	v.CreatePlane(Vector3(0, -1, 0), RANGE/2, Vector3(RANGE/2, RANGE/2, RANGE/2));
	v.CreatePlane(Vector3(1, 0, 0), RANGE/2, Vector3(RANGE/2, RANGE/2, RANGE/2));
	v.CreatePlane(Vector3(-1, 0, 0), RANGE/2, Vector3(RANGE/2, RANGE/2, RANGE/2));
	v.CreatePlane(Vector3(0, 0, 1), RANGE/2, Vector3(RANGE/2, RANGE/2, RANGE/2));
	v.CreatePlane(Vector3(0, 0, -1), RANGE/2, Vector3(RANGE/2, RANGE/2, RANGE/2));

	//Initialise a timer
	GameTimer timer;

	//Set booleans for toggles
	bool quit = false;
	bool changeGravity = false;
	bool gravity = false;
	bool octRender = true;

	float currentTime = timer.GetTime() * 0.001f;
	float accum = 0.0;
	float dt = 1.0f/60.0f;

	//Set camera position variables
	float z = -100, y = 0, x = 0;

	//Main loop
	while (!quit){
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type){
			case sf::Event::Closed:
				quit = true;
				break;
			case sf::Event::Resized:
				glViewport(0,0,event.size.width, event.size.height);
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
					//Rotate around origin right (ABOUT Y AXIS)
					y += 1.0f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
					//Rotate around origin left (ABOUT Y AXIS)
					y -= 1.0f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
					//Move back from origin
					z -= 0.5f;

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
					//Move closer to origin
					z += 0.5f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
					//Rotate around x axis
					x -= 0.5f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
					//Rotate around x axis
					x+=0.5f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)){
					//Toggle rendering of octree
					octRender = !octRender;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)){
					//Toggle change gravity
					changeGravity = true;
				}
				break;
			}
		}

		//Toggle gravity code
		if (changeGravity){
			gravity = !gravity;
			if (gravity){
				v.ApplyGravity();
			} else {
				v.RemoveAccelFromAll();
			}
			changeGravity = false;
		}

		

		//Obtain time since last frame
		float newTime = timer.GetTime() * 0.001f;
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		accum += frameTime;

		while ( accum >= dt )
		{
			v.update(dt);
			accum -= dt;
		}
		
		//We step our physics engine at a semi fixed timestep.
		//http://gafferongames.com/game-physics/fix-your-timestep/
		v.update(dt);

		//clear back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set the view matrix to variables defined
		r.SetViewMatrix(Matrix4::Translation(Vector3(0, 0, z)) *
			Matrix4::Rotation(y, Vector3(0, 1, 0)) *
			Matrix4::Rotation(x, Vector3(1,0,0)));

		//Draw phsyics using renderer.
		v.Draw(r, octRender);

		//Swap buffers
		window.display();
		
	}

	//Perform clean up
	MeshManager::Destroy();
	TextureManager::Destroy();
	ShaderManager::Destroy();

	return 0;
}