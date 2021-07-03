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

#define PLATFORM_COUNT 2
#define WALL_COUNT 10

struct GameState {
    Entity* spaceship;
    Entity* walls;
    Entity* platform;
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
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lunar Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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


    // Initialize Game Objects

    // Initialize spaceship
    state.spaceship = new Entity();
    state.spaceship->entityType = SPACESHIP;
    state.spaceship->position = glm::vec3(0, 4.0f, 0);
    state.spaceship->movement = glm::vec3(0);
    state.spaceship->acceleration = glm::vec3(0, -0.5f, 0);
    state.spaceship->speed = 1.5f;
    state.spaceship->textureID = LoadTexture("enemyBlue2.png");

    state.spaceship->height = 1.0f;
    state.spaceship->width = 1.0f;

    // Initialize Walls
    state.walls = new Entity[WALL_COUNT];

    GLuint wallTextureID = LoadTexture("platformPack_tile016.png");

    float startX = -4.5f;
    for (int i = 0; i < WALL_COUNT; i++) {
        state.walls[i].entityType = WALL;
        if (i == 2 || i == 3) {
            continue;
        }
        state.walls[i].textureID = wallTextureID;
        state.walls[i].position = glm::vec3(startX+i, -3.25f, 0);
    }
    
    state.walls[2].textureID = wallTextureID;
    state.walls[2].position = glm::vec3(startX + 3.5, 0, 0);

    state.walls[3].textureID = wallTextureID;
    state.walls[3].position = glm::vec3(startX + 4.5, 0, 0);

    for (int i = 0; i < WALL_COUNT; i++) {
        state.walls[i].Update(0, NULL, 0);
    }

    //Initialize Platform
    state.platform = new Entity[PLATFORM_COUNT];

    GLuint platformTextureID = LoadTexture("platformPack_tile008.png");

    state.platform[0].textureID = platformTextureID;
    state.platform[0].entityType = PLATFORM;
    state.platform[0].position = glm::vec3(startX + 2, -3.25f, 0);

    state.platform[1].textureID = platformTextureID;
    state.platform[1].entityType = PLATFORM;
    state.platform[1].position = glm::vec3(startX + 3, -3.25f, 0);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platform[i].Update(0, NULL, 0);
    }
}

void ProcessInput() {

    state.spaceship->movement = glm::vec3(0);

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
                
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.spaceship->acceleration.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.spaceship->acceleration.x = 1.0f;
    }

    if (glm::length(state.spaceship->movement) > 1.0f) {
        state.spaceship->movement = glm::normalize(state.spaceship->movement);
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
        state.spaceship->Update(FIXED_TIMESTEP, state.walls, WALL_COUNT);
        state.spaceship->Update(FIXED_TIMESTEP, state.platform, PLATFORM_COUNT);
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

//Font texture
GLuint fontTextureID = LoadTexture("font1.png");

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < WALL_COUNT; i++) {
        state.walls[i].Render(&program);
    }

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platform[i].Render(&program);
    }

    state.spaceship->Render(&program);

    if (state.spaceship->lastCollision == WALL) { //failed
        DrawText(&program, fontTextureID, "MISSION FAILED", 0.5f, -0.25f, glm::vec3(-4.75f, 3.3, 0));
    }
    else if (state.spaceship->lastCollision == PLATFORM) {
        DrawText(&program, fontTextureID, "MISSION ACCOMPLISHED!!", 0.5f, -0.25f, glm::vec3(-4.75f, 3.3, 0));

    }
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
        Render();
    }

    Shutdown();
    return 0;
}
