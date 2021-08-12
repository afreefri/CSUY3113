#include "Level1.h"

#define OBJECT_COUNT 234
#define ENEMY_COUNT 3


void Level1::Initialize() {

    state.nextScene = -1; 

    state.enemyNumber = ENEMY_COUNT;
    state.objectsNumber = OBJECT_COUNT;

    //PLAYER STUFFS
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-9, 0.75f, -9);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    state.player->rotation = glm::vec3(0, 180, 0); //have the player face inside the maze when game started


    //FLOOR
    state.objects = new Entity[OBJECT_COUNT];

    GLuint floorTextureID = Util::LoadTexture("floor.jpg");
    Mesh* floorMesh = new Mesh();
    floorMesh->LoadOBJ("cube.obj", 20);

    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = floorMesh;
    state.objects[0].position = glm::vec3(0, -0.25f, 0);
    state.objects[0].entityType = FLOOR;
    state.objects[0].scale = glm::vec3(20, 0.5f, 20);



    //PERIMETER BUSHES
    GLuint bushTextureID = Util::LoadTexture("bush.jpg");
    Mesh* bushMesh = new Mesh();
    bushMesh->LoadOBJ("cube.obj", 1);

    //left wall
    for (int i = 0; i < 11; i++) {
        state.objects[i+1].textureID = bushTextureID;
        state.objects[i+1].mesh = bushMesh;
        state.objects[i+1].entityType = BUSH;
        state.objects[i+1].position = glm::vec3(i, 0.5f, -10);
    }

    for (int i = 0; i < 11; i++) {
        state.objects[i + 12].textureID = bushTextureID;
        state.objects[i + 12].mesh = bushMesh;
        state.objects[i + 12].entityType = BUSH;
        state.objects[i + 12].position = glm::vec3(i, 1.5f, -10);
    }

    for (int i = 0; i < 11; i++) {
        state.objects[i + 23].textureID = bushTextureID;
        state.objects[i + 23].mesh = bushMesh;
        state.objects[i + 23].entityType = BUSH;
        state.objects[i + 23].position = glm::vec3(-i, 0.5f, -10);
    }

    for (int i = 0; i < 11; i++) {
        state.objects[i + 34].textureID = bushTextureID;
        state.objects[i + 34].mesh = bushMesh;
        state.objects[i + 34].entityType = BUSH;
        state.objects[i + 34].position = glm::vec3(-i, 1.5f, -10);
    }

    //bottom wall
    for (int i = 0; i < 20; i++) {
        state.objects[i + 45].textureID = bushTextureID;
        state.objects[i + 45].mesh = bushMesh;
        state.objects[i + 45].entityType = BUSH;
        state.objects[i + 45].position = glm::vec3(-10, 0.5f, -9 + i);
    }

    for (int i = 0; i < 20; i++) {
        state.objects[i + 65].textureID = bushTextureID;
        state.objects[i + 65].mesh = bushMesh;
        state.objects[i + 65].entityType = BUSH;
        state.objects[i + 65].position = glm::vec3(-10, 1.5f, -9 + i);
    }

    //top wall
    for (int i = 0; i < 20; i++) {
        state.objects[i + 85].textureID = bushTextureID;
        state.objects[i + 85].mesh = bushMesh;
        state.objects[i + 85].entityType = BUSH;
        state.objects[i + 85].position = glm::vec3(10, 0.5f, -9 + i);
    }

    for (int i = 0; i < 20; i++) {
        state.objects[i + 105].textureID = bushTextureID;
        state.objects[i + 105].mesh = bushMesh;
        state.objects[i + 105].entityType = BUSH;
        state.objects[i + 105].position = glm::vec3(10, 1.5f, -9 + i);
    }

    //right wall
    for (int i = 0; i < 19; i++) {
        state.objects[i + 124].textureID = bushTextureID;
        state.objects[i + 124].mesh = bushMesh;
        state.objects[i + 124].entityType = BUSH;
        state.objects[i + 124].position = glm::vec3(9 - i, 0.5f, 10);
    }

    for (int i = 0; i < 19; i++) {
        state.objects[i + 143].textureID = bushTextureID;
        state.objects[i + 143].mesh = bushMesh;
        state.objects[i + 143].entityType = BUSH;
        state.objects[i + 143].position = glm::vec3(9 - i, 1.5f, 10);
    }

    //big ol' mess of me setting up the inner maze parts 

    //1
    for (int i = 0; i < 4; i++) {
        state.objects[i + 162].textureID = bushTextureID;
        state.objects[i + 162].mesh = bushMesh;
        state.objects[i + 162].entityType = BUSH;
        state.objects[i + 162].position = glm::vec3(-9 + i, 0.5f, 2);
    }

    //2
    for (int i = 0; i < 5; i++) {
        state.objects[i + 166].textureID = bushTextureID;
        state.objects[i + 166].mesh = bushMesh;
        state.objects[i + 166].entityType = BUSH;
        state.objects[i + 166].position = glm::vec3(-9 + i, 0.5f, 5);
    }

    //3
    for (int i = 0; i < 4; i++) {
        state.objects[i + 171].textureID = bushTextureID;
        state.objects[i + 171].mesh = bushMesh;
        state.objects[i + 171].entityType = BUSH;
        state.objects[i + 171].position = glm::vec3(-5, 0.5f, -9 + i);
    }

    //4
    for (int i = 0; i < 5; i++) {
        state.objects[i + 175].textureID = bushTextureID;
        state.objects[i + 175].mesh = bushMesh;
        state.objects[i + 175].entityType = BUSH;
        state.objects[i + 175].position = glm::vec3(-7+i, 0.5f, -2);
    }

    //5
    for (int i = 0; i < 12; i++) {
        state.objects[i + 180].textureID = bushTextureID;
        state.objects[i + 180].mesh = bushMesh;
        state.objects[i + 180].entityType = BUSH;
        state.objects[i + 180].position = glm::vec3(-2, 0.5f, -5+i);
    }

    //6
    for (int i = 0; i < 3; i++) {
        state.objects[i + 192].textureID = bushTextureID;
        state.objects[i + 192].mesh = bushMesh;
        state.objects[i + 192].entityType = BUSH;
        state.objects[i + 192].position = glm::vec3(-1+i, 0.5f, -5);
    }

    //7
    for (int i = 0; i < 5; i++) {
        state.objects[i + 195].textureID = bushTextureID;
        state.objects[i + 195].mesh = bushMesh;
        state.objects[i + 195].entityType = BUSH;
        state.objects[i + 195].position = glm::vec3(-1 + i, 0.5f, 0);
    }

    //8
    for (int i = 0; i < 3; i++) {
        state.objects[i + 200].textureID = bushTextureID;
        state.objects[i + 200].mesh = bushMesh;
        state.objects[i + 200].entityType = BUSH;
        state.objects[i + 200].position = glm::vec3(-1, 0.5f, 9-i);
    }

    //9
    for (int i = 0; i < 6; i++) {
        state.objects[i + 203].textureID = bushTextureID;
        state.objects[i + 203].mesh = bushMesh;
        state.objects[i + 203].entityType = BUSH;
        state.objects[i + 203].position = glm::vec3(9-i, 0.5f, -3);
    }

    //10
    for (int i = 0; i < 2; i++) {
        state.objects[i + 209].textureID = bushTextureID;
        state.objects[i + 209].mesh = bushMesh;
        state.objects[i + 209].entityType = BUSH;
        state.objects[i + 209].position = glm::vec3(6, 0.5f, -5+i);
    }

    //11
    for (int i = 0; i < 3; i++) {
        state.objects[i + 211].textureID = bushTextureID;
        state.objects[i + 211].mesh = bushMesh;
        state.objects[i + 211].entityType = BUSH;
        state.objects[i + 211].position = glm::vec3(7-i, 0.5f, -6);
    }

    //12
    for (int i = 0; i < 4; i++) {
        state.objects[i + 214].textureID = bushTextureID;
        state.objects[i + 214].mesh = bushMesh;
        state.objects[i + 214].entityType = BUSH;
        state.objects[i + 214].position = glm::vec3(5, 0.5f, 9-i);
    }

    //13
    for (int i = 0; i < 5; i++) {
        state.objects[i + 218].textureID = bushTextureID;
        state.objects[i + 218].mesh = bushMesh;
        state.objects[i + 218].entityType = BUSH;
        state.objects[i + 218].position = glm::vec3(7-i, 0.5f, 5);
    }

    //14
    for (int i = 0; i < 5; i++) {
        state.objects[i + 223].textureID = bushTextureID;
        state.objects[i + 223].mesh = bushMesh;
        state.objects[i + 223].entityType = BUSH;
        state.objects[i + 223].position = glm::vec3(7, 0.5f, 4-i);
    }

    //15
    for (int i = 0; i < 2; i++) {
        state.objects[i + 228].textureID = bushTextureID;
        state.objects[i + 228].mesh = bushMesh;
        state.objects[i + 228].entityType = BUSH;
        state.objects[i + 228].position = glm::vec3(-5, 0.5f, 6+i);
    }




    //INITIALIZE PORTAL TILES
    GLuint portalTextureID = Util::LoadTexture("portal.png");

    state.objects[230].textureID = portalTextureID;
    state.objects[230].mesh = bushMesh;
    state.objects[230].entityType = PORTAL;
    state.objects[230].position = glm::vec3(7, 0.5f, 10);
    state.objects[230].isActive = false;

    state.objects[231].textureID = portalTextureID;
    state.objects[231].mesh = bushMesh;
    state.objects[231].entityType = PORTAL;
    state.objects[231].position = glm::vec3(7, 1.5f, 10);
    state.objects[231].isActive = false;

    state.objects[232].textureID = portalTextureID;
    state.objects[232].mesh = bushMesh;
    state.objects[232].entityType = PORTAL;
    state.objects[232].position = glm::vec3(6, 0.5f, 10);
    state.objects[232].isActive = false;

    state.objects[233].textureID = portalTextureID;
    state.objects[233].mesh = bushMesh;
    state.objects[233].entityType = PORTAL;
    state.objects[233].position = glm::vec3(6, 1.5f, 10);
    state.objects[233].isActive = false;




    ///PORTAL KEY
    state.key = new Entity();

    GLuint portalKeyTextureID = Util::LoadTexture("portal_key.png");
    
    state.key->billboard = true;
    state.key->textureID = portalKeyTextureID;
    state.key->position = glm::vec3(-7, 0.5, 7);
    state.key->scale = glm::vec3(0.5f, 0.5f, 0.5f);
    state.key->entityType = PORTALKEY;



    //ENEMIES
    state.enemies = new Entity[ENEMY_COUNT];

    GLuint enemyTextureID = Util::LoadTexture("witch.png");

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].billboard = true;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].scale = glm::vec3(2, 2, 2);
        state.enemies[i].speed = 0.5f;
        state.enemies[i].width = 0.7f;

        state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5f, rand() % 20 - 10);

        for (int j = 0; j < OBJECT_COUNT; j++) {
            while (state.enemies[i].CheckCollision(&state.objects[j])) {
                state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5f, rand() % 20 - 10);
            }
        }
    }



    //FIREBALLS
    state.fireballs = new Entity();

    GLuint fireballTextureID = Util::LoadTexture("fireball.png");

    state.fireballs->billboard = true;
    state.fireballs->entityType = FIREBALL;
    state.fireballs->textureID = fireballTextureID;
    state.fireballs->scale = glm::vec3(0.5f, 0.5f, 0.5f);
    state.fireballs->isActive = false;

    



    //MUSIC
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096); //open audio 
    state.music = Mix_LoadMUS("music.mp3");
    Mix_PlayMusic(state.music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

    //SOUND
    state.shootChunk = Mix_LoadWAV("shoot.wav");
    Mix_VolumeChunk(state.shootChunk, MIX_MAX_VOLUME / 6);

    state.keyChunk = Mix_LoadWAV("portal_key.wav");
    Mix_VolumeChunk(state.keyChunk, MIX_MAX_VOLUME / 4);

    state.portalChunk = Mix_LoadWAV("portal.wav");
    Mix_VolumeChunk(state.portalChunk, MIX_MAX_VOLUME / 4);

    state.footsteps = Mix_LoadWAV("footsteps.wav");
    Mix_VolumeChunk(state.footsteps, MIX_MAX_VOLUME / 4);

}

void Level1::Update(float deltaTime) {

    //UPDATE PLAYER
    state.player->Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, OBJECT_COUNT, ENEMY_COUNT);

    //UPDATE OBJECTS
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, OBJECT_COUNT, ENEMY_COUNT);
    }

    //UPDATE KEY
    state.key->Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, OBJECT_COUNT, ENEMY_COUNT);

    //UPDATE ENEMIES
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, OBJECT_COUNT, ENEMY_COUNT);
    }
    
    //UPDATE FIREBALLS
    state.fireballs->Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, OBJECT_COUNT, ENEMY_COUNT);

    //COLLISION WITH KEY
    if (state.player->keyFound) {

        //open up portal
        for (int i = 230; i < 234; i++) {
            state.objects[i].isActive = true;
        }

        //get rid of the bushes 
        state.objects[126].isActive = false;
        state.objects[127].isActive = false;
        state.objects[145].isActive = false;
        state.objects[146].isActive = false;
    }

    //CHECK FOR WIN OR LOSE
    if (state.player->win) {
        state.nextScene = 2;
    }
    else if (state.player->lose) {
        state.nextScene = 3;
    }

}

void Level1::Render(ShaderProgram* program) {

    //RENDER OBJECTS
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }

    //RENDER KEY
    state.key->Render(program);

    
    //RENDER ENEMIES
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    //RENDER FIREBALLS
    state.fireballs->Render(program);
}