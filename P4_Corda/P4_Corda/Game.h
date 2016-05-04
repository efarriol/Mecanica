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
		int _screenType;	
		float Ke;
		float Kd;
		glm::vec3 force1;
		glm::vec3 force2;
		glm::vec3 previousForce2;
		glm::vec3 totalForce;
		float elasticTerm;
		glm::vec3 dampingTerm;
		glm::vec3 normalizedVector;
		//Screen type Persp/Ortho
		GameState _gameState;			//It describes the game state				
		Window _window;					//Manage the OpenGL context
		GLSLProgram _colorProgram;		//Manage the shader programs
		FPSLimiter _fpsLimiter;			//Manage the synchronization between frame rate and refresh rate
		OpenGLBuffers _openGLBuffers;	//Manage the openGL buffers
		Geometry _gameElements;			//Manage the game elements
		InputManager _inputManager;		//Manage the input devices
		GLuint modelMatrixUniform;
		GLuint viewMatrixUniform;
		GLuint projectionMatrixUniform;
			//Internal methods
		void initSystems();
		void initShaders();		
		void gameLoop();
		void processInput();
		void doPhysics();
		void executePlayerCommands();
		void renderGame();	
		void renderObject(GameObject ganeElement);
		// De aqui a arriba es CG hacia abajo Mecanica

		//Internal methods
		vector<Particle> sysParticles;
		void loadParticles();
		float _dt = 0.01f;
};

