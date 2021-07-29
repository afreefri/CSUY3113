#include "Level3.h"

#define LEVEL3_WIDTH 32
#define LEVEL3_HEIGHT 15

#define LEVEL3_ENEMY_COUNT 2

unsigned int level3_data[] =
{
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 111, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 131, 000, 000, 000, 000, 000, 000, 000,
    123, 000, 000, 000, 000, 000, 000, 000, 000, 153, 155, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 131, 000, 000, 000, 000, 000, 000, 000,
    122, 102, 102, 103, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 101, 103,  34,  34, 101, 102, 102, 102, 102, 102, 102, 102, 102,
    122, 122, 122, 123, 000, 153, 154, 155, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 121, 123,  54,  54, 121, 122, 122, 122, 122, 122, 122, 122, 122,
    122, 122, 122, 123, 000, 000, 000, 000, 000, 000, 000, 153, 154, 155, 000, 000, 000, 108, 000, 121, 123,  54,  54, 121, 122, 122, 122, 122, 122, 122, 122, 122,
    122, 122, 122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 101, 102, 102, 102, 122, 123,  74,  74, 121, 122, 122, 122, 122, 122, 122, 122, 122,
    122, 122, 122, 123, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 121, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122,
     53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53
};

void Level3::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tiles_packed.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 20, 9);

    // Move over all of the player and enemy code from initialization.
    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2.5, -9, 0);
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
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("characters.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].patrolRangeRight = 10;
    state.enemies[0].patrolRangeLeft = 3;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(3, -7, 0);

    state.enemies[0].animRight = new int[3]{ 24,25,26 };

    state.enemies[0].animIndices = state.enemies[0].animRight;
    state.enemies[0].animFrames = 3;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 9;
    state.enemies[0].animRows = 3;

    state.enemies[0].height = 0.8f;
    state.enemies[0].width = 0.65f;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = UPDOWNPATROLLER;
    state.enemies[1].patrolRangeTop = -5.0f;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 2;
    state.enemies[1].acceleration = glm::vec3(0, -0.8f, 0);
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(15, -4, 0);

    state.enemies[1].animUp = new int[3]{ 24,25,26 };
    state.enemies[1].animDown = new int[3]{ 25,25,25 };

    state.enemies[1].animIndices = state.enemies[1].animUp;
    state.enemies[1].animFrames = 3;
    state.enemies[1].animIndex = 0;
    state.enemies[1].animTime = 0;
    state.enemies[1].animCols = 9;
    state.enemies[1].animRows = 3;

    state.enemies[1].height = 0.8f;
    state.enemies[1].width = 0.65f;
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    }

    if (state.player->killed) {
        state.nextScene = 3; //start over
    }

    //pass the finsh line
    if (state.player->position.x >= 25) {
        state.nextScene = 4; //play win scene
    }

}

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}