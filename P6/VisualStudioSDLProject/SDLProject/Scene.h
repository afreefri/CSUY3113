#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"


struct GameState {
	Map* map;
	Entity* player;
	Entity* objects; 
	Entity* enemies;
	Entity* key;
	Entity* fireballs;
	int enemyNumber = 0;
	int objectsNumber = 0;
	int nextScene;

	//for drawing 2d ui overlay over 3d environment
	glm::mat4 uiViewMatrix;
	glm::mat4 uiProjectionMatrix;
	GLuint fontTextureID;
	GLuint iconTextureID;

	//music stuffs
	Mix_Music* music;
	Mix_Chunk* shootChunk;
	Mix_Chunk* keyChunk;
	Mix_Chunk* portalChunk;
	Mix_Chunk* footsteps;
};

class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program) = 0;
};