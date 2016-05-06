#include "Game.h"

/**
* Constructor
* Note: It uses an initialization list to set the parameters
* @param windowTitle is the window title
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
Game::Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS) :
	_windowTitle(windowTitle), 
	_screenWidth(screenWidth), 
	_screenHeight(screenHeight),
	_gameState(GameState::INIT), 
	_fpsLimiter(enableLimiterFPS, maxFPS, printFPS) {
	_screenType = 1;
}

/**
* Destructor
*/
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
}

/*
* Initializes all the game engine components
*/
void Game::initSystems() {
		//Create an Opengl window using SDL
	_window.create(_windowTitle, _screenWidth, _screenHeight, 0);		
		//Compile and Link shader
	initShaders();
		//Set up the openGL buffers
	_openGLBuffers.initializeBuffers(_colorProgram);
		//Load the current scenario
	_gameElements.loadGameElements("./resources/scene2D.txt");
	_gameElements.loadBasic3DObjects();
	loadParticles();
	Ke = 200.0f;
	Kd = 0.5f;
	_planeBottom.setPointNormal(glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//_planeRight.setPointNormal(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//_planeLeft.setPointNormal(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	//_planeTop.setPointNormal(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

void Game::loadParticles(){
	float zIncrement = 0;
	float xIncrement = 0;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			sysParticles[i][j].setPosition(-2 + xIncrement, 3, 4 - zIncrement);
			//sysParticles[j].setVelocity(0, 0, 0);
			sysParticles[i][j].setLifetime(500);
			sysParticles[i][j].setBouncing(0.5f);
			if (i != 0)sysParticles[i][j].setFixed(false);
			else sysParticles[i][j].setFixed(true);
			zIncrement += 0.5f;
		}
		zIncrement = 0;
		xIncrement += 0.5f;
	}
	for (int i = 1; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			_gameElements.getGameParticle(i * COLUMNS + j)._translate.x = sysParticles[i][j].getCurrentPosition().x;
			_gameElements.getGameParticle(i * COLUMNS + j)._translate.y = sysParticles[i][j].getCurrentPosition().y;
			_gameElements.getGameParticle(i * COLUMNS + j)._translate.z = sysParticles[i][j].getCurrentPosition().z;
		}
	}
}

/*
* Initialize the shaders:
* Compiles, sets the variables between C++ and the Shader program and links the shader program
*/
void Game::initShaders() {
		//Compile the shaders
	_colorProgram.addShader(GL_VERTEX_SHADER, "./resources/shaders/vertex-shader.txt");
	_colorProgram.addShader(GL_FRAGMENT_SHADER, "./resources/shaders/fragment-shader.txt");
	_colorProgram.compileShaders();
		//Attributes must be added before linking the code
	_colorProgram.addAttribute("vertexPositionGame");
	_colorProgram.addAttribute("vertexColor");
		//Link the compiled shaders
	_colorProgram.linkShaders();
		//Bind the uniform variables. You must enable shaders before gettting the uniforme variable location
	_colorProgram.use();
	modelMatrixUniform = _colorProgram.getUniformLocation("modelMatrix");
	viewMatrixUniform = _colorProgram.getUniformLocation("viewMatrix");
	projectionMatrixUniform = _colorProgram.getUniformLocation("projectionMatrix");

	_colorProgram.unuse();
}

/*
* Game execution: Gets input events, processes game logic and draws sprites on the screen
*/
void Game::gameLoop() {	
	_gameState = GameState::PLAY;
	while (_gameState != GameState::EXIT) {	
			//Start synchronization between refresh rate and frame rate
		_fpsLimiter.startSynchronization();
			//Process the input information (keyboard and mouse)
		processInput();
			//Execute the player actions (keyboard and mouse)
		executePlayerCommands();
			//Update the game status
		doPhysics();
			//Draw the objects on the screen
		renderGame();	
			//Force synchronization
		_fpsLimiter.forceSynchronization();
	}
}

/*
* Processes input with SDL
*/
void Game::processInput() {
	_inputManager.update();
	//Review https://wiki.libsdl.org/SDL_Event to see the different kind of events
	//Moreover, table show the property affected for each event type
	SDL_Event evnt;
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
* Executes the actions sent by the user by means of the keyboard and mouse
*/
void Game::executePlayerCommands() {
	Camera camera(WIDTH, HEIGHT, _screenType);
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)){
		glm::ivec2 mouseCoords = _inputManager.getMouseCoords();
		cout << mouseCoords.x << ", " << mouseCoords.y << endl;
	}

	if (_inputManager.isKeyDown(SDLK_p)) {
		_screenType = PERSP_CAM;
	}
	if (_inputManager.isKeyDown(SDLK_o)) {
		_screenType = ORTHO_CAM;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (_inputManager.isKeyDown(SDLK_i)) {
		_screenType = AUTO_CAM;
	}

}

void Game::executeActions() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (sysParticles[i][j].getLifetime()>0) {
				sysParticles[i][j].updateParticle(_dt, Particle::UpdateMethod::EulerSemi);
				sysParticles[i][j].setLifetime(sysParticles[i][j].getLifetime() - _dt);
				float disact, disant;
				//PLANES
				disant = _planeBottom.distPoint2Plane(sysParticles[i][j].getPreviousPosition());
				disact = _planeBottom.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
				if (disant*disact < 0.0f) {
					//only valid for the plane y=0 (floor plane)
					glm::vec3 correcPos = -(1 + sysParticles[i][j].getBouncing()) * disact *_planeBottom.normal;
					glm::vec3 correcVel = -(1 + sysParticles[i][j].getBouncing()) * (sysParticles[i][j].getVelocity()*_planeBottom.normal)*_planeBottom.normal;
					sysParticles[i][j].setPosition(sysParticles[i][j].getCurrentPosition() + correcPos);
					sysParticles[i][j].setVelocity(sysParticles[i][j].getVelocity() + correcVel);
				}
				disant = _planeBottom.distPoint2Plane(sysParticles[i][j].getPreviousPosition());
				disact = _planeRight.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
				if (disant*disact < 0.0f) {
					//only valid for the plane y=0 (floor plane)
					glm::vec3 correcPos = -(1 + sysParticles[i][j].getBouncing()) * disact *_planeRight.normal;
					glm::vec3 correcVel = -(1 + sysParticles[i][j].getBouncing()) * (sysParticles[i][j].getVelocity()*_planeRight.normal)*_planeRight.normal;
					sysParticles[i][j].setPosition(sysParticles[i][j].getCurrentPosition() + correcPos);
					sysParticles[i][j].setVelocity(sysParticles[i][j].getVelocity() + correcVel);
				}
				disant = _planeBottom.distPoint2Plane(sysParticles[i][j].getPreviousPosition());
				disact = _planeLeft.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
				if (disant*disact < 0.0f) {
					//only valid for the plane y=0 (floor plane)
					glm::vec3 correcPos = -(1 + sysParticles[i][j].getBouncing()) * disact *_planeLeft.normal;
					glm::vec3 correcVel = -(1 + sysParticles[i][j].getBouncing()) * (sysParticles[i][j].getVelocity()*_planeLeft.normal)*_planeLeft.normal;
					sysParticles[i][j].setPosition(sysParticles[i][j].getCurrentPosition() + correcPos);
					sysParticles[i][j].setVelocity(sysParticles[i][j].getVelocity() + correcVel);
				}
				disant = _planeBottom.distPoint2Plane(sysParticles[i][j].getPreviousPosition());
				disact = _planeTop.distPoint2Plane(sysParticles[i][j].getCurrentPosition());
				if (disant*disact < 0.0f) {
					//only valid for the plane y=0 (floor plane)
					glm::vec3 correcPos = -(1 + sysParticles[i][j].getBouncing()) * disact *_planeTop.normal;
					glm::vec3 correcVel = -(1 + sysParticles[i][j].getBouncing()) * (sysParticles[i][j].getVelocity()*_planeTop.normal)*_planeTop.normal;
					sysParticles[i][j].setPosition(sysParticles[i][j].getCurrentPosition() + correcPos);
					sysParticles[i][j].setVelocity(sysParticles[i][j].getVelocity() + correcVel);
				}
				//SPHERE
					if (sysParticles[i][j].isInsideSphere(_gameElements.getGameElement(0)._radious, _gameElements.getGameElement(0)._center)) {
					sysParticles[i][j].setPosition(sysParticles[i][j].correctPosition(_gameElements.getGameElement(0)._radious, _gameElements.getGameElement(0)._center));
					sysParticles[i][j].setVelocity(sysParticles[i][j].correctVelocity(_gameElements.getGameElement(0)._radious, _gameElements.getGameElement(0)._center));
				}

			}
			_gameElements.getGameParticle(i * COLUMNS + j)._translate.x = sysParticles[i][j].getCurrentPosition().x;
			_gameElements.getGameParticle(i * COLUMNS + j)._translate.y = sysParticles[i][j].getCurrentPosition().y;
			_gameElements.getGameParticle(i * COLUMNS + j)._translate.z = sysParticles[i][j].getCurrentPosition().z;
		}
	}
}





/*
* Update the game objects based on the physics
*/
void Game::doPhysics() {
	float distRepos = 0.5f; //insertar valor aqui

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			sysParticles[i][j].setForce(0.0f, 0.0f, 0.0f);  //Avoid to accumulate
			sysParticles[i][j].setForce(0.0f, -9.81f, 0.0f);
		}
	}

	for (int i = 0; i < ROWS - 1; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			glm::vec3 posDifference = sysParticles[i + 1][j].getCurrentPosition() - sysParticles[i][j].getCurrentPosition();
			glm::vec3 velDifference = sysParticles[i + 1][j].getVelocity() - sysParticles[i][j].getVelocity();
			glm::vec3 force1 = (Ke * (glm::length(posDifference) - distRepos) + Kd * glm::dot(velDifference, posDifference / glm::length(posDifference))) * (posDifference / glm::length(posDifference));
			glm::vec3 force2 = -force1;
			sysParticles[i][j].addForce(force1);
			sysParticles[i + 1][j].addForce(force2);
		}
	}
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS - 1; j++) {
			glm::vec3 posDifference = sysParticles[i][j + 1].getCurrentPosition() - sysParticles[i][j].getCurrentPosition();
			glm::vec3 velDifference = sysParticles[i][j + 1].getVelocity() - sysParticles[i][j].getVelocity();
			glm::vec3 force1 = (Ke * (glm::length(posDifference) - distRepos) + Kd * glm::dot(velDifference, posDifference / glm::length(posDifference))) * (posDifference / glm::length(posDifference));
			glm::vec3 force2 = -force1;
			sysParticles[i][j].addForce(force1);
			sysParticles[i][j + 1].addForce(force2);
		}
	}
	for (int j = 0; j < COLUMNS - 1; j++) {
		for (int i = 1; i < ROWS; i++) {
			glm::vec3 posDifference = sysParticles[i - 1][j + 1].getCurrentPosition() - sysParticles[i][j].getCurrentPosition();
			glm::vec3 velDifference = sysParticles[i - 1][j + 1].getVelocity() - sysParticles[i][j].getVelocity();
			glm::vec3 force1 = (Ke * (glm::length(posDifference) - distRepos) + Kd * glm::dot(velDifference, posDifference / glm::length(posDifference))) * (posDifference / glm::length(posDifference));
			glm::vec3 force2 = -force1;
			sysParticles[i][j].addForce(force1);
			sysParticles[i - 1][j + 1].addForce(force2);
		}
	}
	for (int j = 0; j < COLUMNS - 1; j++) {
		for (int i = 0; i < ROWS - 1; i++) {
			glm::vec3 posDifference = sysParticles[i + 1][j + 1].getCurrentPosition() - sysParticles[i][j].getCurrentPosition();
			glm::vec3 velDifference = sysParticles[i + 1][j + 1].getVelocity() - sysParticles[i][j].getVelocity();
			glm::vec3 force1 = (Ke * (glm::length(posDifference) - distRepos) + Kd * glm::dot(velDifference, posDifference / glm::length(posDifference))) * (posDifference / glm::length(posDifference));
			glm::vec3 force2 = -force1;
			sysParticles[i][j].addForce(force1);
			sysParticles[i + 1][j + 1].addForce(force2);
		}
	}
	executeActions();								
}

/**
* Draw the sprites on the screen
*/
void Game::renderGame() {
	//Temporal variable
	Camera _camera(WIDTH, HEIGHT, _screenType);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Bind the GLSL program. Only one code GLSL can be used at the same time
	_colorProgram.use();
	//For each one of the elements: Each object MUST BE RENDERED based on its position, rotation and scale data
	for (int i = 0; i < _gameElements.getNumGameElements(); i++) {	
		renderObject(_gameElements.getGameElement(i));
	}
	for (int i = 0; i < NUMPARTICLES; i++) {
		renderObject(_gameElements.getGameParticle(i));
	}
	//Unbind the program
	_colorProgram.unuse();

	//Swap the display buffers (displays what was just drawn)
	_window.swapBuffer();
} 

void Game::renderObject(GameObject ganeElement) {
	//Temporal variable
	Camera _camera(WIDTH, HEIGHT, _screenType);
	
	GameObject currentRenderedGameElement = ganeElement;
	glm::mat4 modelMatrix;

	modelMatrix = glm::translate(modelMatrix, currentRenderedGameElement._translate);
	if (currentRenderedGameElement._angle != 0) {
		modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRenderedGameElement._angle), currentRenderedGameElement._rotation);
	}
	modelMatrix = glm::scale(modelMatrix, currentRenderedGameElement._scale);

	//glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	//Send data to GPU

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.viewMatrix()));
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.projectionMatrix()));

	_openGLBuffers.sendDataToGPU(_gameElements.getData(currentRenderedGameElement._objectType), _gameElements.getNumVertices(currentRenderedGameElement._objectType), currentRenderedGameElement._objectType);

}