#pragma once


//Third-party libraries
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>			//OpenGL Mathematics 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Window.h"
#include "GLSLProgram.h"
#include "FPSLimiter.h"
#include "OpenGLBuffers.h"
#include "Vertex.h"
#include "Geometry.h"
#include "InputManager.h"
#include "Constants.h"
#include "Camera.h"
#include <time.h>
#include <random> 
#include "Particle.h"
#include "GeometryPlane.h"


//Game has four possible states: INIT (Preparing environment), PLAY (Playing), EXIT (Exit from the game) or MENU (Game menu)
enum class GameState{INIT, PLAY, EXIT, MENU};

//This class manages the game execution
class Game {
	public:						
		Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS);	//Constructor
		~Game();					//Destructor
		void run();					//Game execution
			
	private:
			//Attributes	
		std::string _windowTitle;		//Window Title
		int _screenWidth;				//Screen width in pixels				
		int _screenHeight;				//Screen height in pixels	
		int _screenType;				//Screen type Persp/Ortho
		GameState _gameState;			//It describes the game state				
		Window _window;					//Manage the OpenGL context
		GLSLProgram _colorProgram;		//Manage the shader programs
		FPSLimiter _fpsLimiter;			//Manage the synchronization between frame rate and refresh rate
		OpenGLBuffers _openGLBuffers;	//Manage the openGL buffers
		Geometry _gameElements;			//Manage the game elements
		InputManager _inputManager;		//Manage the input devices
		GLuint modelMatrixUniform;
			//Internal methods
		void initSystems();
		void initShaders();		
		void gameLoop();
		void processInput();
		void doPhysics();
		void executePlayerCommands();
		void renderGame();	

		// De aqui a arriba es CG hacia abajo Mecanica
	
		GLuint gVAO[NUMBASICOBJECTS];
		GLuint gVBO[NUMBASICOBJECTS];
		//Internal methods
		vector<glm::vec3> posSysPart; //Only position is needed for draw
		vector<Particle> sysParticles;
		int _Numparticles = 15;
		float _dt = 0.01f; //Simulation time step
		Plane _planeBottom; //
		Plane _planeRight;
		Plane _planeLeft;
		Plane _planeTop;
		Plane _intersectPlane;
};

