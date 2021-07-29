#include "Level1.h"

#define LEVEL1_WIDTH 32
#define LEVEL1_HEIGHT 15

#define LEVEL1_ENEMY_COUNT 1

unsigned int level1_data[] =
{
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 116, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 116, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 116, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 116, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 116, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 116, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 116, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 143, 000, 000, 000, 000, 000, 000, 000, 000, 000, 116, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 138, 139, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 111, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000,  98,  99, 136, 000, 000, 000,  36,  16, 000, 000, 000, 000, 000, 000, 000, 131, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000,   6, 000, 000,  96, 000, 000,  16,  56, 000, 000, 000, 000, 000, 000, 021, 022, 022, 022, 022, 022, 022, 022, 022, 022,
	123, 000, 000, 000, 001, 002, 003, 000, 000, 000, 000, 116, 000, 000, 000,  76, 000, 000, 153, 154, 155, 000, 121, 122, 122, 122, 122, 122, 122, 122, 122, 122,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 137, 000, 000, 000, 137, 000, 000, 000, 000, 000, 000, 121, 122, 122, 122, 122, 122, 122, 122, 122, 122,
    122, 022, 022, 022, 023, 000, 000, 021, 022, 022, 022, 022, 023, 000, 000, 021, 022, 023, 000, 000, 000, 000, 121, 122, 122, 122, 122, 122, 122, 122, 122, 122,
	122, 122, 122, 122, 123, 000, 000, 121, 122, 122, 122, 122, 123, 000, 000, 121, 122, 123, 000, 000, 000, 000, 121, 122, 122, 122, 122, 122, 122, 122, 122, 122
};

void Level1::Initialize() {

    state.nextScene = -1; 

	GLuint mapTextureID = Util::LoadTexture("tiles_packed.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 20, 9);

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2.5, -11, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.5f;
    state.player->textureID = Util::LoadTexture("character_0003.png");

    state.player->height = 0.8f;
    state.player->width = 0.65f;

    state.player->jumpPower = 6.5f;

    // Initialize Enemies 
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("characters.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(15, -10, 0);

    state.enemies[0].animRight = new int[3]{ 24,25,26 };

    state.enemies[0].animIndices = state.enemies[0].animRight;
    state.enemies[0].animFrames = 3;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 9;
    state.enemies[0].animRows = 3;

    state.enemies[0].height = 0.8f;
    state.enemies[0].width = 0.65f;
}

void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    state.enemies[0].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);

    //pass the finish line
    if (state.player->position.x >= 25) { 
        state.nextScene = 2; 
    }

    if (state.player->killed) {
        state.nextScene = 1; //start over
    }
}

void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    state.enemies[0].Render(program);
}