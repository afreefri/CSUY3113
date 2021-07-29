#include "Level2.h"

#define LEVEL2_WIDTH 32
#define LEVEL2_HEIGHT 15

#define LEVEL2_ENEMY_COUNT 2

unsigned int level2_data[] =
{
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    122, 143, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,   6, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000,  14,  13,  12,  13,  15, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 111, 000, 000, 000, 000, 000,
    121,   6, 000, 000, 000, 000, 000,  32, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 131, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000,  52, 000, 000, 000,  14,  12,  15, 000,   6,   6, 000, 000, 000, 000, 000, 000, 000, 000,  61,  62,  62,  62,  62,  62,  63,
    123, 000, 000, 153, 155, 000, 000,  32, 000, 000, 000, 000,  32, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 121, 122, 122, 122, 122, 122, 123,
    123, 000, 000, 000, 000, 000, 000,  52, 000, 000, 000, 000,  52, 000, 000, 000, 000,  61,  62,  62,  63,  34,  34,  61,  62, 122, 122, 122, 122, 122, 122, 123,
    122,  62,  62,  62,  62,  63, 000,  72, 000, 000, 000, 000,  32, 000, 000, 000, 000, 121, 122, 122, 123,  54,  54, 121, 122, 122, 122, 122, 122, 122, 122, 123,
    122, 122, 122, 122, 122, 122,  62,  62,  63, 000, 000, 000,  72, 000, 000,  61,  62, 122, 122, 122, 123,  74,  74, 121, 122, 122, 122, 122, 122, 122, 122, 123,
     53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  60,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,
     73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73, 140,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73,  73
};

void Level2::Initialize() {

    state.nextScene = -1; 

    GLuint mapTextureID = Util::LoadTexture("tiles_packed.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 20, 9);

    // Move over all of the player and enemy code from initialization.
    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2.5, -10, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.5f;
    state.player->textureID = Util::LoadTexture("character_0003.png");

    state.player->height = 0.8f;
    state.player->width = 0.65f;
    //the player graphic has a little border around it, making the 
    //player look like it is floating, so we fixed that issue by 
    //making the height slightly shorter
    //we fixed the width as well 

    state.player->jumpPower = 6.5f;

    // Initialize Enemies 
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("characters.png");

    //bug thingie
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(3, -4, 0);

    state.enemies[0].animRight = new int[3]{ 24,25,26 };

    state.enemies[0].animIndices = state.enemies[0].animRight;
    state.enemies[0].animFrames = 3;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 9;
    state.enemies[0].animRows = 3;

    state.enemies[0].height = 0.8f;
    state.enemies[0].width = 0.65f;

    //crab
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = PATROLLER;
    state.enemies[1].patrolRangeLeft = 17.0f;
    state.enemies[1].patrolRangeRight = 20.0f;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 1;
    state.enemies[1].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(17, -10, 0);

    state.enemies[1].animRight = new int[1]{ 21 };

    state.enemies[1].animIndices = state.enemies[1].animRight;
    state.enemies[1].animFrames = 1;
    state.enemies[1].animIndex = 0;
    state.enemies[1].animTime = 0;
    state.enemies[1].animCols = 9;
    state.enemies[1].animRows = 3;

    state.enemies[1].height = 0.8f;
    state.enemies[1].width = 0.65f;
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    }

    //pass the finish line
    if (state.player->position.x >= 27) {
        state.nextScene = 3;
    }

    if (state.player->killed) {
        state.nextScene = 2; //start over
    }
}

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}