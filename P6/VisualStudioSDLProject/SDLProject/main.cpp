#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Scene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "WinScene.h"
#include "LoseScene.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

bool mainMenu = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;


//SCENE STUFFS
Scene* currentScene;
Scene* sceneList[4];

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");


    //VIEW STUFFS
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);
    //45.0f is the field of view
    //1.777f is the aspect ratio 1280/720
    //0.1f is the nearPlane. closer won't render
    //100.f is farPlane. further won't render 

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.2f, 0.1f, 0.2f, 1.0f); //background color


    //INITIALIZE SCENES
    sceneList[0] = new MainMenu();
    sceneList[1] = new Level1();
    sceneList[2] = new WinScene();
    sceneList[3] = new LoseScene();

    SwitchToScene(sceneList[0]);


    /*
    GLuint crateTextureID = Util::LoadTexture("crate1_diffuse.png");
    Mesh* crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj", 1);

    state.objects[1].textureID = crateTextureID;
    state.objects[1].mesh = crateMesh;
    state.objects[1].position = glm::vec3(0, 0.5f, -5);
    state.objects[1].entityType = CRATE;

    state.objects[2].textureID = crateTextureID;
    state.objects[2].mesh = crateMesh;
    state.objects[2].position = glm::vec3(-1, 0.5f, -5);
    state.objects[2].entityType = CRATE;
    
    state.objects[3].textureID = crateTextureID;
    state.objects[3].mesh = crateMesh;
    state.objects[3].position = glm::vec3(0, 1.5f, -5);
    state.objects[3].entityType = CRATE;

    //ENEMIES

    state.enemies = new Entity[ENEMY_COUNT];

    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5f, rand() % 20 - 10);
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
    }

    
    GLuint cubeTextureID = Util::LoadTexture("crate1_diffuse.png");

    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj");

    state.objects[0].textureID = cubeTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(0, 0, -5);
    state.objects[0].entityType = CUBE;

    //state.objects[0].rotation = glm::vec3(45.0f, 0.0f, 0.0f); 
    //cube rotating 45 degrees on the x axis

    GLuint marioTextureID = Util::LoadTexture("mario_body.png");

    Mesh* marioMesh = new Mesh();
    marioMesh->LoadOBJ("mario.obj");

    state.objects[1].textureID = marioTextureID;
    state.objects[1].mesh = marioMesh;
    state.objects[1].position = glm::vec3(-10, 0, -40);
    state.objects[1].scale = glm::vec3(0.25f, 0.25f, 0.25f);
    //keep all the dimensions the same if you want to keep the original proportions 
    //you can change it if you want to squash the objects or something 
    state.objects[1].entityType = ENEMY;

    GLuint pikachuTextureID = Util::LoadTexture("pikachu.png");

    Mesh* pikachuMesh = new Mesh();
    pikachuMesh->LoadOBJ("pikachu.obj");

    state.objects[2].textureID = pikachuTextureID;
    state.objects[2].mesh = pikachuMesh;
    state.objects[2].position = glm::vec3(2, 0, -4);
    state.objects[2].entityType = ENEMY;
    */

}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                    if (mainMenu) {
                        currentScene->state.nextScene = 1;
                        mainMenu = false;
                    }
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                        
                }
                break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_A]) {
        currentScene->state.player->rotation.y += 0.75f;
    }
    else if (keys[SDL_SCANCODE_D]) {
        currentScene->state.player->rotation.y -= 0.75f;
    }
    else if (keys[SDL_SCANCODE_F]) { //SHOOT
        if (currentScene->state.fireballs->isActive == false) {
            currentScene->state.player->fire = true;
            Mix_PlayChannel(-1, currentScene->state.shootChunk, 0);
        }
    }
    
    currentScene->state.player->velocity.x = 0;
    currentScene->state.player->velocity.z = 0;

    if (keys[SDL_SCANCODE_W]) {
        currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * -2.0f;
        currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * -2.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * 2.0f;
        currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * 2.0f;
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {

        currentScene->Update(FIXED_TIMESTEP);

        //play key sounds
        if (currentScene->state.player->keyFound) {
            Mix_PlayChannel(-1, currentScene->state.keyChunk, 0);
            currentScene->state.player->keyFound = false;
        }

        //play footstep sounds 
        for (int i = 0; i < currentScene->state.enemyNumber; i++) {
            if (currentScene->state.enemies[i].isActive == false) continue;

            if (currentScene->state.enemies[i].playFootsteps) {
                Mix_PlayChannel(-1, currentScene->state.footsteps, 0);
            }
        }

        /*
        //play portal sounds
        for (int i = 230; i < 231; i++) {
            if (currentScene->state.objectsNumber == 0) break;
            if (currentScene->state.objects[i].playPortalChunk) {
                Mix_PlayChannel(-1, currentScene->state.portalChunk, 0);
            }
        }
        */

        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(currentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -currentScene->state.player->position);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    currentScene->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();

        //SWITCH SCENES
        if (currentScene->state.nextScene > 0) {
            Mix_HaltMusic();
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        }

        Render();
    }
    
    //FREE MUSIC
    for (int i = 0; i < 4; i++) {
        Mix_FreeMusic(sceneList[i]->state.music);
        Mix_FreeChunk(sceneList[i]->state.shootChunk);
        Mix_FreeChunk(sceneList[i]->state.keyChunk);
        Mix_FreeChunk(sceneList[i]->state.portalChunk);
        Mix_FreeChunk(sceneList[i]->state.footsteps);
    }

    Shutdown();
    return 0;
}
