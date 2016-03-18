#pragma once

#include <GL/glew.h>			
#include <glm/glm.hpp>
#include "Window.h"
#include "FpsLimiter.h"
#include "InputManager.h"
#include "GLSLProgram.h"
#include "Particle.h"
#include "Constants.h"
#include "GeometryPlane.h"


// standard C++ libraries
#include <iostream>
//#include <math.h>

//Game has two possible states: PLAY or EXIT
enum class GameState{INIT, PLAY, EXIT};

const int NumGameObj = 3; //Triangle, SysParticles, Circle

//This class manages the game execution
class Game {
	public:			
			//Constructor			
		Game(std::string windowTitle, int screenWidth, int screenHeight, int maxFPS);
		~Game();				//Destructor
		void run();					//Game execution

	private:
			//Attributes	
		std::string _windowTitle;
		int _screenWidth;			//Screen width in pixels				
		int _screenHeight;			//Screen height in pixels				
		int _maxFPS;				//The maximum FPS that we will generate
		GameState _gameState;		//It describes the game state		
		GLuint gVAO[NumGameObj];
		GLuint gVBO[NumGameObj];
	
			//Internal methods
		void initSystems();
		void loadShaders();
		void loadGameObjects(const int& NumGameObj);
//		void loadTextures();
		void gameLoop();
		void processInput();
		void executeActions();
		void drawGame();

		Window _window;
		FpsLimiter _fpsLimiter;
		InputManager _inputManager;
		GLSLProgram _glProgram;
		vector<glm::vec3> posSysPart; //Only position is needed for draw
		vector<Particle> sysParticles;
		int _Numparticles = 1;
		float _dt = 0.01; //Simulation time step
		Plane _planeBottom; //
		Plane _planeRight;
		Plane _planeLeft;
		Plane _planeTop;
//		Polygon _polTriang;
};

