#include "LoseScene.h"


GLuint loseFontTextureID;

void LoseScene::Initialize() {

    state.nextScene = -1;

    state.uiViewMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
    state.uiProjectionMatrix = glm::mat4(1.0);
    state.fontTextureID = Util::LoadTexture("font.png");
    state.iconTextureID = Util::LoadTexture("portal_key.png");

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


    state.player = new Entity[1];
    state.player->isActive = false;

    state.objects = new Entity();

    GLuint floorTextureID = Util::LoadTexture("main_menu.jpg");
    Mesh* floorMesh = new Mesh();
    floorMesh->LoadOBJ("cube.obj", 10);

    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = floorMesh;
    state.objects[0].position = glm::vec3(0, 0, 0);
    state.objects[0].entityType = FLOOR;
    state.objects[0].scale = glm::vec3(10, 10, 10);

    state.player = new Entity();
    state.player->isActive = false;

    state.enemies = new Entity();
    state.enemies->isActive = false;

    state.fireballs = new Entity();
    state.fireballs->isActive = false;

}

void LoseScene::Update(float deltaTime) {
    for (int i = 0; i < 1; i++) {
        state.objects[i].Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, 1, 0);
    }

    state.player->Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, 1, 0);

    state.enemies->Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, 1, 0);

    state.fireballs->Update(deltaTime, state.player, state.objects, state.key, state.enemies, state.fireballs, 1, 0);

}

void LoseScene::Render(ShaderProgram* program) {
    for (int i = 0; i < 1; i++) {
        state.objects[i].Render(program);
    }

    state.enemies->Render(program);

    state.fireballs->Render(program);

    //after drawing 3D, switch
    program->SetProjectionMatrix(state.uiProjectionMatrix);
    program->SetViewMatrix(state.uiViewMatrix);

    //draw text
    Util::DrawText(program, state.fontTextureID, "YOU LOST :(", 0.5, -0.1f, glm::vec3(-2, 0, 0));

}