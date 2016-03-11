#include "Game.h"

Game::Game(std::string windowTitle, int screenWidth, int screenHeight, int maxFPS) :
	_windowTitle(windowTitle), _screenWidth(screenWidth), _screenHeight(screenHeight),_gameState(GameState::INIT), _maxFPS(maxFPS) {

}

Game::~Game()
{
}

/*
* Game execution
*/
void Game::run() {
		//System initializations
	initSystems();
		//Start the game if all the elements are ready
	gameLoop();
	//system("pause"); //Don't close the command window
}

/*
* Initializes all the game engine components
*/
void Game::initSystems() {
		//Create an Opengl window using SDL
	_window.create(_windowTitle, _screenWidth, _screenHeight, 0);
		//Set the max fps
	_fpsLimiter.setMaxFPS(_maxFPS);
		//Compile and Link shader
	loadShaders();
		//Load all the Game Objects
	loadGameObjects(NumGameObj);
		//Load and prepare the textures
	//loadTextures();	
}

/*
* Compiles, sets the variables between C++ and the Shader program and links the shader program
*/
void Game::loadShaders() {
		//Compile the shaders
	_glProgram.addShader(GL_VERTEX_SHADER, "./vertex-shader.txt");
	_glProgram.addShader(GL_FRAGMENT_SHADER, "./fragment-shader.txt");
	_glProgram.compileShaders();
		//Attributes must be added before linking the code
	_glProgram.addAttribute("vert");
		//Link the compiled shaders
	_glProgram.linkShaders();
}

/*
* Loads all the element that has to be displayed on the screen
*/
void Game::loadGameObjects(const int& NumGameObj) {
	// make and bind the VAO
	glGenVertexArrays(NumGameObj, &gVAO[0]);
	
	//----------------------
	// LOAD the Triangle
	//----------------------

	// bind the VAO for the Triangle
	glBindVertexArray(gVAO[0]);
	// make and bind the VBO
	glGenBuffers(1, &gVBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);

	// define vertex coordinates
	std::vector<glm::vec3> vertexData;
	float mida = 0.2f;
//	float altura = sin(M_PI / 4.0f) * (2 * mida);
	float altura = sqrt(2.0f) * mida;
	vertexData.push_back(glm::vec3(-mida+0.1f, 0.0f, 0.0f));
	vertexData.push_back(glm::vec3(mida+0.1f, 0.0f, 0.0f));
	vertexData.push_back(glm::vec3(0.1f, altura, 0.0f));


	//for (int i = 0; i < 3; i++) {
	//	vertexData[i] = vertexData[i] + glm::vec3(0.4f, 0.4f, 0.0f); //translate to point (0.4,0.4), no matter about z value. 
	//}
	glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);
	
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//-------------------------------------
	// INIT and LOAD the Particle System
	//-------------------------------------
	
	// make and bind the VAO
	glBindVertexArray(gVAO[1]);
	// make and bind the VBO
	glGenBuffers(1, &gVBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);

	sysParticles.resize(_Numparticles);
	// Initialize Particles
	sysParticles[0].setPosition(0.0f, 0.8f, 0.0f);
	sysParticles[0].setVelocity(0.5f, 0.0f, 0.0f);
	sysParticles[0].setLifetime(50.0f);
	sysParticles[0].setBouncing(0.8f);
	sysParticles[0].setFixed(false);

	posSysPart.resize(_Numparticles);
	posSysPart[0] = sysParticles[0].getCurrentPosition(); //Copy position values

//	glBufferData(GL_ARRAY_BUFFER, sizeof(posSysPart), posSysPart, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, posSysPart.size()*sizeof(glm::vec3), &posSysPart[0], GL_STREAM_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//----------------------
	// LOAD the Botton-Plane
	//----------------------
	_planeBottom.setPointNormal(glm::vec3(0.0f, -1.0f, 0.0f),glm::vec3(0.0f, -1.0f, 0.0f));
}

/*
* Loads and prepares the textures
*/
//void Game::loadTextures() {
	//	
//}


/*
* Game execution: Gets input events, processes game logic and draws objects on the screen
*/
void Game::gameLoop() {
	float _fps;
	int frameCounter = 0;
	_gameState = GameState::PLAY;
	float time = 0.0f;
	while (_gameState != GameState::EXIT) {
			//Start the fps limiter for the current frame
		_fpsLimiter.begin();
			//Process the input information (keyboard and mouse)
		processInput();
			//Execute actions based on the input events
		executeActions();
		time = time + _dt;
		std::cout << "Total time = " << time << std::endl;
			//Draw the objects on the screen
		drawGame();			
			//Delay (or not) the execution for allowing the expected FPS
		_fps = _fpsLimiter.end();
			//Draw the current FPS in the console
		if (frameCounter == 10) {
			cout << "FPS:" << _fps << endl;
			frameCounter = 0;
		}
		frameCounter++;
	}
}


/*
* Processes input with SDL
*/
void Game::processInput() {
	//Review https://wiki.libsdl.org/SDL_Event to see the different kind of events
	//Moreover, table show the property affected for each event type
	SDL_Event evnt;

		//Update the input event states. Current states are moved to the previous states for being able to detect pressed keys
	_inputManager.update();

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		default:
			break;
		}
	}
}

/**
* Moves the objects based on their logics and the input events
*/
void Game::executeActions() {

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)){
		glm::ivec2 mouseCoords = _inputManager.getMouseCoords();
		//mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		cout << mouseCoords.x << ", " << mouseCoords.y << endl;
	}

	float disact, disant;

	if (sysParticles[0].getLifetime() > 0) {
		disant = _planeBottom.distPoint2Plane(sysParticles[0].getCurrentPosition());
		sysParticles[0].setForce(0.0f, 0.0f, 0.0f);  //Avoid to accumulate
		sysParticles[0].addForce(0.0f, -9.8f, 0.0f); //gravity
		sysParticles[0].updateParticle(_dt, Particle::UpdateMethod::EulerSemi);

		//Check for floor collisions
		sysParticles[0].setLifetime(sysParticles[0].getLifetime() - _dt); //lifetime is decreased
		disact = _planeBottom.distPoint2Plane(sysParticles[0].getCurrentPosition());
		if (disant*disact < 0.0f) {
			//only valid for the plane y=0 (floor plane)
			glm::vec3 correcPos = -(1 + sysParticles[0].getBouncing()) * disact *_planeBottom.normal;
			glm::vec3 correcVel = -(1 + sysParticles[0].getBouncing()) * (sysParticles[0].getVelocity()*_planeBottom.normal)*_planeBottom.normal;
			sysParticles[0].setPosition(sysParticles[0].getCurrentPosition()+correcPos);
			sysParticles[0].setVelocity(sysParticles[0].getVelocity()+correcVel);
		}
	}



	glm::vec3 posicio=sysParticles[0].getCurrentPosition();
	posSysPart[0] = posicio;
	// Pass to OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, posSysPart.size()*sizeof(glm::vec3), &posSysPart[0], GL_STREAM_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

/**
* Draw objects on the screen
*/
void Game::drawGame() {
	//Set the base depth to 1.0
	glClearDepth(1.0);

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind the GLSL program. Only one code GLSL can be used at the same time
	_glProgram.use();

	/////////
	// bind the VAO (the triangle)
	glBindVertexArray(gVAO[0]);
	// draw the VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// unbind the VAO
	glBindVertexArray(0);

	// bind the VAO (the Particle)
	glBindVertexArray(gVAO[1]);
	// draw the VAO
	glPointSize(6);
	glDrawArrays(GL_POINTS, 0, 1);
	// unbind the VAO
	glBindVertexArray(0);

	/////////

		//Unbind the program
	_glProgram.unuse();

		//Swap the display buffers (displays what was just drawn)
	_window.swapBuffer();
}

