#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"


enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Mesh mesh4;
	Camera myCamera;
	Texture texture; 
	Texture texture1;
	Texture texture2;
	Shader shader;

	void Skybox();
	void Skybox2();
	GLuint skyboxVAO, skyboxVBO, cubemapTexture, cubemapTexture2;
	vector<std::string> faces;
	vector<std::string> faces2;
	Texture skybox;
	Texture skybox2;
	Shader shaderSkybox;

	float counter;

	Audio audioDevice;
	unsigned int bkgdAudio;
	unsigned int dingSound;
	void playAudio(unsigned int Source, glm::vec3 pos);

	//my variables
	bool weapon;
	float timer;
	float velocity;
	float positionx;
	float positiony;
	float positionz;
	bool collided;
	bool collided1;
	bool land;
	bool landed;

	float colX;
	float colY;
	float colZ;

	bool cameraChange;

	glm::vec3 projectile;

	float textureTest;


};

