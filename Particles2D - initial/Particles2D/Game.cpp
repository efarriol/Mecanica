#include "Game.h"
#include <time.h>
#include <random>
#define rand01() ((float)std::rand()/RAND_MAX)
Game::Game(std::string windowTitle, int screenWidth, int screenHeight, int maxFPS) :
	_windowTitle(windowTitle), _screenWidth(screenWidth), _screenHeight(screenHeight), _gameState(GameState::INIT), _maxFPS(maxFPS) {
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

	float mida = 0.2f;
	//	float altura = sin(M_PI / 4.0f) * (2 * mida);
	float altura = sqrt(2.0f) * mida;
	vertexData[0].push_back(glm::vec3(-mida + 0.1f, 0.0f, 0.0f));
	vertexData[0].push_back(glm::vec3(mida + 0.1f, 0.0f, 0.0f));
	vertexData[0].push_back(glm::vec3(0.1f, altura, 0.0f));


	//for (int i = 0; i < 3; i++) {
	//	vertexData[i] = vertexData[i] + glm::vec3(0.4f, 0.4f, 0.0f); //translate to point (0.4,0.4), no matter about z value. 
	//}
	glBufferData(GL_ARRAY_BUFFER, vertexData[0].size()*sizeof(glm::vec3), &vertexData[0][0], GL_STATIC_DRAW);

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
	srand(time(NULL));
	for (int i = 0; i < _Numparticles; i++) {
		sysParticles[i].setPosition(0.1f, 0.3f, 0.0f);
		sysParticles[i].setVelocity((rand01() -0.5), (rand01() + 2), 0);
		sysParticles[i].setLifetime(rand01()*10);
		sysParticles[i].setBouncing(0.8f);
		sysParticles[i].setFixed(false);

		posSysPart.resize(_Numparticles);
		posSysPart[i] = sysParticles[i].getCurrentPosition(); //Copy position values
	}
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(posSysPart), posSysPart, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, posSysPart.size()*sizeof(glm::vec3), &posSysPart[0], GL_STREAM_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//////////////////////////////////////////////////////////
	glBindVertexArray(gVAO[2]);
	// make and bind the VBO
	glGenBuffers(1, &gVBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);

	nCirclePoints = 25;
	float slice = 2 * M_PI / nCirclePoints;
	float radius = 0.3f;
	float centerX = -0.5f;
	float centerY = -0.3f;
	for (int i = 0; i < nCirclePoints; i++) {
		float angle = slice * i;
		float newX = centerX + radius * glm::cos(angle);
		float newY = centerY + radius * glm::sin(angle);
		vertexData[1].push_back(glm::vec3(newX, newY, 0.0f));
	}

	glBufferData(GL_ARRAY_BUFFER, vertexData[1].size()*sizeof(glm::vec3), &vertexData[1][0], GL_STATIC_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);





	/////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	glBindVertexArray(gVAO[3]);
	// make and bind the VBO
	glGenBuffers(1, &gVBO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[3]);
	vertexData[2].push_back(glm::vec3(0.8f, 0, 0));
	vertexData[2].push_back(glm::vec3(0.5f, 0, 0));
	vertexData[2].push_back(glm::vec3(0.5f, -0.8, 0));
	vertexData[2].push_back(glm::vec3(0.8f, -0.8, 0));



	glBufferData(GL_ARRAY_BUFFER, vertexData[2].size()*sizeof(glm::vec3), &vertexData[2][0], GL_STATIC_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//////////////////////////////////////////////////////////
	glBindVertexArray(gVAO[4]);
	// make and bind the VBO
	glGenBuffers(1, &gVBO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[4]);
	vertexData[3].push_back(glm::vec3(0.2, 0, 0));
	vertexData[3].push_back(glm::vec3(-0.1f, 0, 0));
	vertexData[3].push_back(glm::vec3(-0.1f, -0.8, 0));
	vertexData[3].push_back(glm::vec3(0.2, -0.8, 0));



	glBufferData(GL_ARRAY_BUFFER, vertexData[3].size()*sizeof(glm::vec3), &vertexData[3][0], GL_STATIC_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//----------------------
	// LOAD the Botton-Plane
	//----------------------
	_planeBottom.setPointNormal(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	_planeRight.setPointNormal(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_planeLeft.setPointNormal(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	_planeTop.setPointNormal(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
		//std::cout << "Total time = " << time << std::endl;
		//Draw the objects on the screen
		drawGame();
		//Delay (or not) the execution for allowing the expected FPS
		_fps = _fpsLimiter.end();
		//Draw the current FPS in the console
		if (frameCounter == 10) {
			//cout << "FPS:" << _fps << endl;
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

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::ivec2 mouseCoords = _inputManager.getMouseCoords();
		//mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		cout << mouseCoords.x << ", " << mouseCoords.y << endl;
	}

	float disact, disant;
	for (int a = 0; a < _Numparticles; a++) {
		if (sysParticles[a].getLifetime() < 0.009f ) {
			sysParticles[a].setPosition(0.1f, 0.3f, 0.0f);
			sysParticles[a].setVelocity((rand01() - 0.5), (rand01() + 2), 0);
			sysParticles[a].setLifetime(rand01()*10);
		}
		if (sysParticles[a].getLifetime() > 0) {
			disant = _planeBottom.distPoint2Plane(sysParticles[a].getCurrentPosition());
			sysParticles[a].setForce(0.0f, 0.0f, 0.0f);  //Avoid to accumulate
			sysParticles[a].addForce(0.0f, -5.622f, 0.0f); //gravity
			sysParticles[a].updateParticle(_dt, Particle::UpdateMethod::EulerSemi);

			//Check for floor collisions
			sysParticles[a].setLifetime(sysParticles[a].getLifetime() - _dt); //lifetime is decreased

			disact = _planeBottom.distPoint2Plane(sysParticles[a].getCurrentPosition());
			if (disant*disact < 0.0f) {
				//only valid for the plane y=0 (floor plane)
				glm::vec3 correcPos = -(1 + sysParticles[a].getBouncing()) * disact *_planeBottom.normal;
				glm::vec3 correcVel = -(1 + sysParticles[a].getBouncing()) * (sysParticles[a].getVelocity()*_planeBottom.normal)*_planeBottom.normal;
				sysParticles[a].setPosition(sysParticles[a].getCurrentPosition() + correcPos);
				sysParticles[a].setVelocity(sysParticles[a].getVelocity() + correcVel);
			}

			disact = _planeRight.distPoint2Plane(sysParticles[a].getCurrentPosition());
			if (disant*disact < 0.0f) {
				//only valid for the plane y=0 (floor plane)
				glm::vec3 correcPos = -(1 + sysParticles[a].getBouncing()) * disact *_planeRight.normal;
				glm::vec3 correcVel = -(1 + sysParticles[a].getBouncing()) * (sysParticles[a].getVelocity()*_planeRight.normal)*_planeRight.normal;
				sysParticles[a].setPosition(sysParticles[a].getCurrentPosition() + correcPos);
				sysParticles[a].setVelocity(sysParticles[a].getVelocity() + correcVel);
			}
			disact = _planeLeft.distPoint2Plane(sysParticles[a].getCurrentPosition());
			if (disant*disact < 0.0f) {
				//only valid for the plane y=0 (floor plane)
				glm::vec3 correcPos = -(1 + sysParticles[a].getBouncing()) * disact *_planeLeft.normal;
				glm::vec3 correcVel = -(1 + sysParticles[a].getBouncing()) * (sysParticles[a].getVelocity()*_planeLeft.normal)*_planeLeft.normal;
				sysParticles[a].setPosition(sysParticles[a].getCurrentPosition() + correcPos);
				sysParticles[a].setVelocity(sysParticles[a].getVelocity() + correcVel);
			}
			disact = _planeTop.distPoint2Plane(sysParticles[a].getCurrentPosition());
			if (disant*disact < 0.0f) {
				//only valid for the plane y=0 (floor plane)
				glm::vec3 correcPos = -(1 + sysParticles[a].getBouncing()) * disact *_planeTop.normal;
				glm::vec3 correcVel = -(1 + sysParticles[a].getBouncing()) * (sysParticles[a].getVelocity()*_planeTop.normal)*_planeTop.normal;
				sysParticles[a].setPosition(sysParticles[a].getCurrentPosition() + correcPos);
				sysParticles[a].setVelocity(sysParticles[a].getVelocity() + correcVel);
			}

			float alpha = 5; //valor por defecto que indica que en un inicio no esta en el segmento
			int intersectCount = 0;
			glm::vec3 r(0);
			for (int x = 0; x < NumGameObj - 1; x++) {
				for (int i = 0; i < vertexData[x].size(); i++) {
				/*	if (i < vertexData[x].size() - 1) {
					//	if (vertexData[x][i + 1].y != vertexData[x][i].y) {
							alpha = (sysParticles[a].getCurrentPosition().y - vertexData[x][i].y) / (vertexData[x][i + 1].y - vertexData[x][i].y);
					//	}
					}
					else if (i == vertexData[x].size() - 1) {
					//	if (vertexData[x][0].y != vertexData[x][i].y) {
							alpha = (sysParticles[a].getCurrentPosition().y - vertexData[x][i].y) / (vertexData[x][0].y - vertexData[x][i].y);
					//	}
					}
					if (alpha > 0 && alpha < 1) {
						if (i ==  vertexData[x].size() - 1) r = vertexData[x][i] + alpha*(vertexData[x][0] - vertexData[x][i]);
						else  r = vertexData[x][i] + alpha*(vertexData[x][i + 1] - vertexData[x][i]); 
						if(r.x > sysParticles[a].getCurrentPosition().x) intersectCount++;
					}
				}
				*/
				if (i == vertexData[x].size() - 1) {
					if (vertexData[x][i].y != vertexData[x][0].y) {
						float alfa = (sysParticles[a].getCurrentPosition().y - vertexData[x][i].y) / (vertexData[x][0].y - vertexData[x][i].y);
						if (alfa > 0 && alfa < 1) {
							glm::vec3 r = vertexData[x][i] + alfa * (vertexData[x][0] - vertexData[x][i]);
							if (r.x > sysParticles[a].getCurrentPosition().x) intersectCount++;
						}
					}
				}
				else {
					if (vertexData[x][i].y != vertexData[x][i + 1].y) {
						float alfa = (sysParticles[a].getCurrentPosition().y - vertexData[x][i].y) / (vertexData[x][i + 1].y - vertexData[x][i].y);
						if (alfa > 0 && alfa < 1) {
							glm::vec3 r = vertexData[x][i] + alfa * (vertexData[x][i + 1] - vertexData[x][i]);
							if (r.x > sysParticles[a].getCurrentPosition().x) intersectCount++;
						}
					}
				}
			}


				if (intersectCount % 2 != 0) {
					glm::vec3 n(0);
					glm::vec3 collisionVector(0);
					float d;
					for (int i = 0; i < vertexData[x].size(); i++) {
						if (i <  vertexData[x].size() - 1) collisionVector = vertexData[x][i + 1] - vertexData[x][i];
						else if (i == vertexData[x].size() - 1) collisionVector = vertexData[x][0] - vertexData[x][i];
						n = (glm::cross(collisionVector, glm::vec3(0,0,1)) / glm::length(collisionVector));
						d = glm::dot(-n, vertexData[x][i]);
						alpha = (-d - glm::dot(n, sysParticles[a].getPreviousPosition())) / (glm::dot(n, sysParticles[a].getCurrentPosition() - sysParticles[a].getPreviousPosition()));
						if (alpha > 0 && alpha < 1) {
							glm::vec3 correcPos = -(1 + sysParticles[a].getBouncing())*(glm::dot(n, sysParticles[a].getCurrentPosition()) + d)*n;
							glm::vec3 correcVel = -(1 + sysParticles[a].getBouncing())*glm::dot(n, sysParticles[a].getVelocity())*n;
							sysParticles[a].setPosition(sysParticles[a].getPreviousPosition() + correcPos);
							sysParticles[a].setVelocity(sysParticles[a].getVelocity() + correcVel);
							break;
						}
					}

				}
			}
		}


		glm::vec3 posicio = sysParticles[a].getCurrentPosition();
		posSysPart[a] = posicio;
		// Pass to OpenGL
		glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, posSysPart.size()*sizeof(glm::vec3), &posSysPart[0], GL_STREAM_DRAW);
		// connect the xyz to the "vert" attribute of the vertex shader
		glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
		glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
		// unbind the VBO and VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// bind the VAO (the triangle)
	glBindVertexArray(gVAO[0]);
	// draw the VAO
	glDrawArrays(GL_TRIANGLES, 0, vertexData[0].size());
	// unbind the VAO
	glBindVertexArray(0);


	glBindVertexArray(gVAO[2]);
	// draw the VAO
	glDrawArrays(GL_LINE_LOOP, 0, vertexData[1].size());
	// unbind the VAO
	glBindVertexArray(0);

	// bind the VAO (the Particle)
	glBindVertexArray(gVAO[1]);
	// draw the VAO
	glPointSize(6);

	glDrawArrays(GL_POINTS, 0, _Numparticles);
	// unbind the VAO
	glBindVertexArray(0);

	/////////
	glBindVertexArray(gVAO[3]);
	// draw the VAO
	glDrawArrays(GL_LINE_LOOP, 0, vertexData[2].size());
	// unbind the VAO
	glBindVertexArray(0);

	glBindVertexArray(gVAO[4]);
	// draw the VAO
	glDrawArrays(GL_LINE_LOOP, 0, vertexData[3].size());
	// unbind the VAO
	glBindVertexArray(0);
	//Unbind the program
	_glProgram.unuse();

	//Swap the display buffers (displays what was just drawn)
	_window.swapBuffer();
}

