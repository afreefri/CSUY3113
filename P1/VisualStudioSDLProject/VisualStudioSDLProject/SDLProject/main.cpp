#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <iostream>;
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

//LOADING AN IMAGE WITH STB IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, playerMatrix, projectionMatrix;
glm::mat4 grassMatrixArray[10]; //GRASS ARRAY
glm::mat4 cloudMatrixArray[2]; //CLOUD ARRAY
glm::mat4 flowerMatrixArray[3]; //FLOWER ARRAY

//VARIABLES TO KEEP TRACK OF OBJECT POSITIONS 
float player_x = -3.5;
float player_y = -1.0;
float flower_rotate = 0;
float cloud_x = -4;

//ADD THIS FOR THE RENDER FUNCTION 
GLuint playerTextureID, grassTextureID, cloudTextureID, flowerTextureID; 

//LOAD TEXTURE FUNCTION
//returns textureID
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
    displayWindow = SDL_CreateWindow("2D Scene!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    //LOADING A SHADER THAT CAN HANDLE TEXTURES 
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    //OBJECT MATRICES
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    playerMatrix = glm::mat4(1.0f);

    //GRASS TILES
    float x = -4.5;
    float y = -3.25;
    for (int i = 0; i < 10; i++) { //there are 10 grass tiles 
        grassMatrixArray[i] = glm::mat4(1.0f);
        grassMatrixArray[i] = glm::translate(grassMatrixArray[i], glm::vec3(x+i, y, 0.0f)); //I translated once because these tiles will never change
    }

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.7f, 0.6f, 1.0f); //background color 

    //TURN ON BLENDING
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //LOAD OUR TEXTURE HERE
    playerTextureID = LoadTexture("ninjapusheen.png");
    grassTextureID = LoadTexture("grass.png");
    flowerTextureID = LoadTexture("flower.png");
    cloudTextureID = LoadTexture("cloud.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;

void Update() {

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    //SCALING AND TRANSLATING PLAYER
    player_x += 1.0f * deltaTime; 
    playerMatrix = glm::mat4(1.0f);
    playerMatrix = glm::scale(playerMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
    playerMatrix = glm::translate(playerMatrix, glm::vec3(player_x, player_y, 0.0f));
        
    //ROTATING FLOWERS
    flower_rotate += 90.0f * deltaTime;
    for (int i = 0; i < 3; i++) {
        flowerMatrixArray[i] = glm::mat4(1.0f);
        flowerMatrixArray[i] = glm::translate(flowerMatrixArray[i], glm::vec3(-3.5f + 3.0f * i, player_y + 1.5f, 0.0f));
        flowerMatrixArray[i] = glm::rotate(flowerMatrixArray[i], glm::radians(flower_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    //TRANSLATING CLOUDS 
    for (int i = 0; i < 2; i++) {
        cloudMatrixArray[i] = glm::mat4(1.0f);
        cloudMatrixArray[i] = glm::scale(cloudMatrixArray[i], glm::vec3(2.0f, 2.0f, 1.0f)); 
        cloudMatrixArray[i] = glm::translate(cloudMatrixArray[i], glm::vec3(cloud_x, 1.5f, 0.0f));
        cloud_x += 3 * i * deltaTime;
        if (cloud_x > 5.0f) {
            cloud_x = -4.0f; 
        }
    }
}

void drawPlayer() {
    program.SetModelMatrix(playerMatrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawGrass() {
    for (glm::mat4 elem: grassMatrixArray) {
        program.SetModelMatrix(elem);
        glBindTexture(GL_TEXTURE_2D, grassTextureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

}

void drawFlowers() {
    for (glm::mat4 elem : flowerMatrixArray) {
        program.SetModelMatrix(elem);
        glBindTexture(GL_TEXTURE_2D, flowerTextureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void drawClouds() {
    for (glm::mat4 elem : cloudMatrixArray) {
        program.SetModelMatrix(elem);
        glBindTexture(GL_TEXTURE_2D, cloudTextureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

//RENDER FUNCTION FROM SLIDES
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    drawPlayer();
    drawGrass();
    drawFlowers();
    drawClouds();
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

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