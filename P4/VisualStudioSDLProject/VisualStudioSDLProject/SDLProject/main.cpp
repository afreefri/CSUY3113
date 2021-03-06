#define GL_SILENCE_DEPRECATION

#include <vector>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 19
#define ENEMY_COUNT 3
#define CRATE_COUNT 1

GLuint fontTextureID;

struct GameState {
    Entity* player;
    Entity* finishArrow;
    Entity* platforms;
    Entity* enemies;
    Entity* crates;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("AI!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //font texture
    fontTextureID = LoadTexture("font1.png");




    // INITIALIZE PLAYER 

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-4, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.5f;
    state.player->textureID = LoadTexture("player.png");

    state.player->animRight = new int[3]{ 12,13,14 };
    state.player->animLeft = new int[3]{ 6,7,8 };
    state.player->animUp = new int[3]{ 18,19,20 };
    state.player->animDown = new int[3]{ 0,1,2 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 3;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 6;
    state.player->animRows = 4;

    state.player->height = 0.75f;
    state.player->width = 0.75f;
    //the player graphic has a little border around it, making the 
    //player look like it is floating, so fixed that issue by 
    //making the height slightly shorter
    //fixed the width as well 

    state.player->jumpPower = 6.7f;

    state.player->playMusic();




    // INITIALIZE PLATFORMS

    state.platforms = new Entity[PLATFORM_COUNT];

    GLuint platformTextureID = LoadTexture("Tile (2).png");

    //11 floor tiles
    state.platforms[0].entityType = PLATFORM;
    state.platforms[0].textureID = LoadTexture("Tile (7).png");
    state.platforms[0].position = glm::vec3(3.5f, -3.25f, 0);

    for (int i = 1; i < 10; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(3.5 - i, -3.25f, 0);
    }

    GLuint rightPlatformCornerTextureID = LoadTexture("Tile (8).png");
    state.platforms[10].entityType = PLATFORM;
    state.platforms[10].textureID = rightPlatformCornerTextureID;
    state.platforms[10].position = glm::vec3(4.5f, -3.25f, 0);

    //floating platform 
    GLuint floatingPlatformLeftTextureID = LoadTexture("Tile (14).png");
    GLuint floatingPlatformMiddleTextureID = LoadTexture("Tile (15).png");
    GLuint floatingPlatformRightTextureID = LoadTexture("Tile (16).png");

    state.platforms[11].entityType = PLATFORM;
    state.platforms[11].textureID = floatingPlatformLeftTextureID;
    state.platforms[11].position = glm::vec3(-0.5, -1.5, 0);

    state.platforms[12].entityType = PLATFORM;
    state.platforms[12].textureID = floatingPlatformMiddleTextureID;
    state.platforms[12].position = glm::vec3(0.5, -1.5, 0);

    state.platforms[13].entityType = PLATFORM;
    state.platforms[13].textureID = floatingPlatformRightTextureID;
    state.platforms[13].position = glm::vec3(1.5, -1.5, 0);

    GLuint sideRightTileTextureID = LoadTexture("Tile (4).png");

    //right side wall 
    int counter = 1;
    for (int i = 14; i < 17; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = sideRightTileTextureID;
        state.platforms[i].position = glm::vec3(4.5, -3.25f + counter, 0);
        counter++;
    }

    state.platforms[17].entityType = PLATFORM;
    state.platforms[17].textureID = platformTextureID;
    state.platforms[17].position = glm::vec3(4.5, -3.25f + counter, 0);

    state.platforms[18].entityType = PLATFORM;
    state.platforms[18].textureID = floatingPlatformLeftTextureID;
    state.platforms[18].position = glm::vec3(3.5, -3.25f + counter, 0);

    //update Crates 
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        // only have to update the platforms once so we can do it in initialize
        //so the deltaTimes don't really matter
        state.platforms[i].Update(0, NULL, NULL, NULL, NULL, NULL, 0, 0, 0);
    }




    // INITIALIZE OBJECTS 

    //crates
    state.crates = new Entity[CRATE_COUNT];

    GLuint crateTextureID = LoadTexture("Crate.png");

    state.crates[0].entityType = CRATE;
    state.crates[0].textureID = crateTextureID;
    state.crates[0].position = glm::vec3(1.5, -0.5, 0);

    //update crates 
    for (int i = 0; i < CRATE_COUNT; i++) {
        state.crates[i].Update(0, NULL, NULL, NULL, NULL, NULL, 0, 0, 0);
    }

    //finish arrow
    GLuint finishArrowTextureID = LoadTexture("ArrowSign.png");

    state.finishArrow = new Entity();
    state.finishArrow->entityType = FINISHARROW;
    state.finishArrow->textureID = finishArrowTextureID;
    state.finishArrow->position = glm::vec3(4.5f, -3.25f + counter + 1.0f, 0);

    //update finish arrow 
    state.finishArrow->Update(0, NULL, NULL, NULL, NULL, NULL, 0, 0, 0);




    // INITIALIZE ENEMIES

    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("ghost.png");

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].animRight = new int[4]{ 8,9,10,11 };
        state.enemies[i].animLeft = new int[4]{ 4,5,6,7 };
        state.enemies[i].animUp = new int[4]{ 0,1,2,3 }; //I made up and down the same so it's facing forward going up and down 
        state.enemies[i].animDown = new int[4]{ 0,1,2,3 };

        state.enemies[i].animIndices = state.enemies->animUp;
        state.enemies[i].animFrames = 4;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 4;
        state.enemies[i].animRows = 4;

        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].speed = 1.0f;

        state.enemies[i].height = 0.75f;
        state.enemies[i].width = 0.6f;
    }

    state.enemies[0].position = glm::vec3(1.5f, 0.5f, 0);
    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].aiState = IDLE;

    state.enemies[1].aiType = PATROLLER;
    //no gravity for this one he flies 
    state.enemies[1].speed = 1.5f;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].position = glm::vec3(-4.0f, -0.5f, 0);

    state.enemies[2].aiType = WAITANDGO;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].position = glm::vec3(3.25, -2.25, 0);
    state.enemies[2].acceleration = glm::vec3(0, -9.81f, 0);

}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                if (state.player->collidedBottom) {
                    state.player->jump = true;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }



    if (state.player->win == false && state.player->lose == false) {

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT]) {
            state.player->movement.x = -1.0f;
            state.player->animIndices = state.player->animLeft;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            state.player->movement.x = 1.0f;
            state.player->animIndices = state.player->animRight;
        }

        if (glm::length(state.player->movement) > 1.0f) {
            state.player->movement = glm::normalize(state.player->movement);
        }
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
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, state.crates, state.enemies, state.finishArrow, PLATFORM_COUNT, CRATE_COUNT, ENEMY_COUNT);

        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, state.crates, state.enemies, state.finishArrow, PLATFORM_COUNT, CRATE_COUNT, ENEMY_COUNT);
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position)
{
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {

        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;

        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            });

        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }

    for (int i = 0; i < CRATE_COUNT; i++) {
        state.crates[i].Render(&program);
    }

    state.finishArrow->Render(&program);

    state.player->Render(&program);

    //win or lose message and play win or lose sounds 
    if (state.player->lose == true) {
        DrawText(&program, fontTextureID, "YOU LOSE!", 0.5f, -0.25f, glm::vec3(-4.75f, 3.3, 0));
    }
    else if (state.player->win == true) {
        DrawText(&program, fontTextureID, "YOU WIN!", 0.5f, -0.25f, glm::vec3(-4.75f, 3.3, 0));
    }

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    state.player->deAllocateMusic();

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}