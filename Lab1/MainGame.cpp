#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>



Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0, 0.0)),
					Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5, 1.0)),
					Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0, 0.0)) };

unsigned int indices[] = { 0, 1, 2 };

Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display();
	Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
	Mesh* mesh4();
	Texture* texture();
	Texture* texture1();
	Texture* texture2();
	Shader* shader();
	Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay();

	bkgdAudio = audioDevice.loadSound("..\\res\\bh.wav");
	dingSound = audioDevice.loadSound("..\\res\\ding.wav");

	mesh1.loadModel("..\\res\\submarine.obj");
	mesh2.loadModel("..\\res\\monkey3.obj");
	mesh3.loadModel("..\\res\\sphere1.obj");
	mesh4.loadModel("..\\res\\cube.obj");

	texture.init("..\\res\\subTexture.jpg");
	texture1.init("..\\res\\fur.jpg");
	texture2.init("..\\res\\trgtTxtr.jpg");

	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag"); //new shader

	shaderSkybox.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag"); //Skybox Shader

	counter = 1.0f;

	//initialises my variables
	weapon = false;
	timer = 500;
	velocity = 1;
	positionx = 1;
	positiony = 1;
	positionz = 1;
	collided = false;
	collided1 = false;
	cameraChange = true;
	textureTest = 0;
	land = false;
	landed = false;

	vector<std::string> faces
	{
		"..\\res\\skybox1\\right.jpg",
		"..\\res\\skybox1\\left.jpg",
		"..\\res\\skybox1\\top.jpg",
		"..\\res\\skybox1\\bottom.jpg",
		"..\\res\\skybox1\\front.jpg",
		"..\\res\\skybox1\\back.jpg"
	};

	vector<std::string> faces2
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap
	cubemapTexture2 = skybox2.loadCubemap(faces2); //Load the cubemap

	float skyboxVertices[] = {          
		-48.0f,  48.0f, -48.0f,
		-48.0f, -48.0f, -48.0f,
		 48.0f, -48.0f, -48.0f,
		 48.0f, -48.0f, -48.0f,
		 48.0f,  48.0f, -48.0f,
		-48.0f,  48.0f, -48.0f,

		-48.0f, -48.0f,  48.0f,
		-48.0f, -48.0f, -48.0f,
		-48.0f,  48.0f, -48.0f,
		-48.0f,  48.0f, -48.0f,
		-48.0f,  48.0f,  48.0f,
		-48.0f, -48.0f,  48.0f,

		48.0f, -48.0f, -48.0f,
		48.0f, -48.0f,  48.0f,
		48.0f,  48.0f,  48.0f,
		48.0f,  48.0f,  48.0f,
		48.0f,  48.0f, -48.0f,
		48.0f, -48.0f, -48.0f,

		-48.0f, -48.0f,  48.0f,
		-48.0f,  48.0f,  48.0f,
		 48.0f,  48.0f,  48.0f,
		 48.0f,  48.0f,  48.0f,
		 48.0f, -48.0f,  48.0f,
		-48.0f, -48.0f,  48.0f,

		-48.0f,  48.0f, -48.0f,
		 48.0f,  48.0f, -48.0f,
		 48.0f,  48.0f,  48.0f,
		 48.0f,  48.0f,  48.0f,
		-48.0f,  48.0f,  48.0f,
		-48.0f,  48.0f, -48.0f,

		-48.0f, -48.0f, -48.0f,
		-48.0f, -48.0f,  48.0f,
		 48.0f, -48.0f, -48.0f,
		 48.0f, -48.0f, -48.0f,
		-48.0f, -48.0f,  48.0f,
		 48.0f, -48.0f,  48.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		playAudio(bkgdAudio, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) //Get and process events
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

			//set up so I can use the different mouse buttons if need be
		case SDL_MOUSEBUTTONDOWN:
			if (evnt.button.button == SDL_BUTTON_LEFT)
			{
				if (cameraChange == false) {
					if (weapon == false)
					{
						weapon = true;

					}
				}
			}
			/*else if (evnt.button.button == SDL_BUTTON_RIGHT)
			{
				//myCamera.MoveForward(-0.1f);
				//fire projectile
			}*/
			break;

		}

		//allows me to move the model based on key presses
		//and the if statements prevent the model from moving out of the level area
		//based on it's x, y and z positions
		switch (evnt.key.keysym.sym)
		{
		case SDLK_w:
			if (positionz < 16)
			{
				positionz += 1;
			}

			break;
		case SDLK_s:
			if (positionz > -16)
			{
				positionz -= 1;
			}

			break;
		case SDLK_a:
			if (positionx < 23)
			{
				positionx += 1;
			}

			break;
		case SDLK_d:
			if (positionx > -23)
			{
				positionx -= 1;
			}

			break;
			case SDLK_q: //positionx -= 1;
				//myCamera.Pitch(0.1f);
				//cameraChange = true;
				break;
			case SDLK_e: //positionx -= 1;
				//myCamera.RotateY(0.1f);
				//cameraChange = false;
				break;
			case SDLK_RETURN: 
				//this part controls when the player lands on the planet
				//only allowing it when the player is near the planet
				if (land == true)
				{
					land = false;
					landed = true;
					cameraChange = false;
				};
				break;
				
		}
	}

}void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	ALint state;
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	if (AL_PLAYING != state)
	{
		audioDevice.playSound(Source, pos);
	}
}


void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	//Skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);


}

void MainGame::Skybox2()
{
	glDepthFunc(GL_LEQUAL);
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	//Skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture2);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);


}

//this is used to calculate the distance between the two collision spheres
//if the distance is less the the radius of the spheres, this will return true
bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x) * (m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) * (m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z) * (m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		return true;

	}
	else
	{
		return false;
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	if (landed) {

		if (collided == false)
		{

			//is no longer drawn when player collides with it

			//collectables
			transform.SetPos(glm::vec3(2, -sin(counter) * 15, 9));
			transform.SetRot(glm::vec3(0, 0, 0));
			transform.SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

			mesh2.updateSphereData(*transform.GetPos(), 1.0f);
			shader.Bind();
			shader.Update(transform, myCamera);
			texture1.Bind(0);
			mesh2.draw();

			
		}

		if (collided1 == false)
		{
			transform.SetPos(glm::vec3(-sin(counter) * 15, 1, 8));
			transform.SetRot(glm::vec3(counter / 10, 0, counter / 5));
			transform.SetScale(glm::vec3(0.25f, 0.25f, 0.25f));

			mesh4.updateSphereData(*transform.GetPos(), 1.0f);
			shader.Bind();
			shader.Update(transform, myCamera);
			texture2.Bind(0);
			mesh4.draw();
		}
	}



	projectile = glm::vec3(0, 1, velocity / -10);

	//ship
	transform.SetPos(glm::vec3(positionx, 1.0f, positionz));

	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(0.08f, 0.08f, 0.05f));

	mesh1.updateSphereData(*transform.GetPos(), 1.0f);
	shader.Bind();
	shader.Update(transform, myCamera);
	texture.Bind(0);
	mesh1.draw();
	//fire projectile

	velocity += 0.5;
	timer--;

	if (timer <= 0)
	{
		weapon = false;
		timer = 500;
		velocity = 0;
	}

	if (landed == false) {
		//planet
		transform.SetPos(glm::vec3(8, 0, 5));
		transform.SetRot(glm::vec3(counter, counter, counter));
		transform.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

		mesh3.updateSphereData(*transform.GetPos(), 1.0f);
		shader.Bind();
		shader.Update(transform, myCamera);
	}
	//I can't get the texture for the planet to work properly, it only really show the colour and none of the detail...
	//so, I've decided to make the planet show up grey and then alter the .Bind value to make it turn red
	//when the player is near
	texture2.Bind(textureTest);
	mesh3.draw();


	//this console output was used to find out where to put the boundaries of the game world
	//cout << "x: " << positionx << " - z: " << positionz << endl;

	//this if statement uses the collision bool function to tell the game there has been a collision
	if (collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius()))
	{
		cout << "COLLIDED" << endl;
		collided = true;
		mesh2.updateSphereData(glm::vec3(100, 100, 100), 0.0f);
		playAudio(dingSound, glm::vec3(0.0f, 0.0f, 0.0f));
	}
	if (collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh4.getSpherePos(), mesh4.getSphereRadius()))
	{
		cout << "COLLIDED" << endl;
		collided1 = true;
		mesh4.updateSphereData(glm::vec3(100, 100, 100), 0.0f);
		playAudio(dingSound, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	if (collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh3.getSpherePos(), mesh3.getSphereRadius()))
	{
		//cout << "COLLIDED PLANET" << endl;
		land = true;
		textureTest = 0;
		
		//playAudio(dingSound, glm::vec3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		textureTest = 1;
		land = false;
	}


	if (landed == true)
	{
		mesh3.updateSphereData(glm::vec3(100, 100, 100), 0.0f);
	}

	//these use the cameraChange bool to initialise different cameras based on the bool's state
	//allowing the user to switch between views when they land on a planet
	//The multiple skybox functions also change the skybox view, so it looks like the player has landed.
	if (cameraChange)
	{
		myCamera.initCamera(glm::vec3(0, 40, 0), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
		Skybox();
	}
	else 
	{
		myCamera.initCamera2(glm::vec3(positionx, positiony + 2.0f, positionz - 20), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
		Skybox2();
	}

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();
}