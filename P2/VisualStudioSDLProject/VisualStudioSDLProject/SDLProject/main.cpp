#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameStarted = false;
bool gameOngoing = true; 

ShaderProgram program;
glm::mat4 viewMatrix, paddleOneMatrix, paddleTwoMatrix, ballMatrix, projectionMatrix;

float paddleWidth = 0.5;
float paddleHeight = 2.0;
float ballWidth = 0.25;
float ballHeight = 0.25; 

//Paddle movement stuffs 
glm::vec3 paddleOnePosition = glm::vec3(-5.0 + 0.25, 0.0f, 0.0f); //starting position 
glm::vec3 paddleOneMovement = glm::vec3(0, 0, 0); // Don’t go anywhere (yet).
glm::vec3 paddleTwoPosition = glm::vec3(5.0 - 0.25, 0.0f, 0.0f);
glm::vec3 paddleTwoMovement = glm::vec3(0, 0, 0); 
float paddleSpeed = 6.0f;

//Ball movement stuffs 
glm::vec3 ballPosition = glm::vec3(0, 0, 0);
glm::vec3 ballMovement = glm::vec3(0, 0, 0); 
float ballSpeed = 6.0f;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("PONG!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    //Initializing paddles and ball
    paddleOneMatrix = glm::mat4(1.0f);
    paddleOneMatrix = glm::translate(paddleOneMatrix, paddleOnePosition);
    paddleTwoMatrix = glm::mat4(1.0f);
    paddleTwoMatrix = glm::translate(paddleTwoMatrix, paddleTwoPosition);
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::scale(ballMatrix, glm::vec3(0.5f, 0.5f, 1.0f));

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(0.8f, 0.3f, 0.4f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ProcessInput() {
    //reset so it doesn't move if nothing is pressed 
    paddleOneMovement = glm::vec3(0);
    paddleTwoMovement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        while(gameOngoing){
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    if (gameStarted == false) { // start game 
                        ballMovement.y = 0.75f;
                        ballMovement.x = 0.75f;
                    }
                    break; //SLDK_SPACE
                }
                break; // SDL_KEYDOWN
            }
        }
    }

    //DETECT IF A KEY IS BEING HELD DOWN 
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_W]) {
        paddleOneMovement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        paddleOneMovement.y = -1.0f;
    }
    if (keys[SDL_SCANCODE_UP]) {
        paddleTwoMovement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        paddleTwoMovement.y = -1.0f;
    }
}

float paddleCenterY, topY, bottomY;
glm::vec3 intendedPosition;

bool validPaddleOneMovement(float deltaTime) { 
    //calculate where the player wants to move
    intendedPosition = paddleOnePosition; 
    intendedPosition += paddleOneMovement * paddleSpeed * deltaTime;

    paddleCenterY = intendedPosition.y;
    topY = paddleCenterY + (paddleHeight / 2);
    bottomY = paddleCenterY - (paddleHeight / 2);
    
    if (topY > 3.75) { //going off the top of the screen 
        return false;
    }
    else if (bottomY < -3.75) { //going off the bottom of the screen
        return false;
    }
    return true; //otherwise the movement is valid
}

bool validPaddleTwoMovement(float deltaTime) {
    //calculate where the player wants to move
    intendedPosition = paddleTwoPosition;
    intendedPosition += paddleTwoMovement * paddleSpeed * deltaTime;

    paddleCenterY = intendedPosition.y;
    topY = paddleCenterY + (paddleHeight / 2);
    bottomY = paddleCenterY - (paddleHeight / 2);

    if (topY > 3.75f) { //going off the top of the screen 
        return false;
    }
    else if (bottomY < -3.75f) { //going off the bottom of the screen
        return false;
    }
    return true; //otherwise the movement is valid
}

glm::vec3 intendedBallPosition = glm::vec3(0, 0, 0);

void ballMove(float deltaTime) {
    intendedBallPosition = ballPosition;
    intendedBallPosition += ballMovement * ballSpeed * deltaTime;

    float ballCenterX = intendedBallPosition.x;
    float ballCenterY = intendedBallPosition.y; 

    float paddleOneCenterX = paddleOnePosition.x;
    float paddleOneCenterY = paddleOnePosition.y; 

    float paddleTwoCenterX = paddleTwoPosition.x; 
    float paddleTwoCenterY = paddleTwoPosition.y; 

    float xDiff, yDiff, xDist, yDist;

    //check if paddle one and ball are colliding 
    xDiff = fabs(ballCenterX - paddleOneCenterX);
    yDiff = fabs(ballCenterY - paddleOneCenterY);
    xDist = xDiff - (ballWidth + paddleWidth) / 2;
    yDist = yDiff - (ballHeight + paddleHeight) / 2;
    if (xDist < 0 && yDist < 0) { //ball and paddle 1 colliding 
        ballMovement.x = -1 * ballMovement.x;
    }

    //check if paddle two and ball are colliding
    xDiff = fabs(ballCenterX - paddleTwoCenterX);
    yDiff = fabs(ballCenterY - paddleTwoCenterY);
    xDist = xDiff - (ballWidth + paddleWidth) / 2;
    yDist = yDiff - (ballHeight + paddleHeight) / 2;
    if (xDist < 0 && yDist < 0) { //ball and paddle 1 colliding 
        ballMovement.x = -1 * ballMovement.x;
    }

    //check if ball is colliding with top or bottom of page 
    float ballTopY = ballCenterY + (ballHeight / 2);
    float ballBottomY = ballCenterY - (ballHeight / 2);
    if (ballTopY > 3.75f || ballBottomY < -3.75f) {  
        ballMovement.y = -1 * ballMovement.y;
    }

    //check if ball is colliding with edge of the screen 
    float ballRightX = ballCenterX + (ballHeight / 2);
    float ballLeftX = ballCenterX - (ballHeight / 2);
    if (ballRightX > 5.0f || ballLeftX < -5.0f) {
        gameOngoing = false;
        ballMovement = glm::vec3(0, 0, 0); //ball can't move anymore 
    }

    ballPosition += ballMovement * ballSpeed * deltaTime;
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    //Update paddle one
    if (validPaddleOneMovement(deltaTime)) {
        paddleOnePosition = intendedPosition;
        paddleOneMatrix = glm::mat4(1.0f);
        paddleOneMatrix = glm::translate(paddleOneMatrix, paddleOnePosition);
    }

    //Update paddle two 
    if (validPaddleTwoMovement(deltaTime)) {
        paddleTwoPosition = intendedPosition;
        paddleTwoMatrix = glm::mat4(1.0f);
        paddleTwoMatrix = glm::translate(paddleTwoMatrix, paddleTwoPosition);
    }

    //Update ball
    ballMove(deltaTime);
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ballPosition);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = { -0.25, -1.0, 0.25, -1.0, 0.25, 1.0, -0.25, -1.0, 0.25, 1.0, -0.25, 1.0 };
    float texCoords[] = { 0.0, 2.0, 0.5, 2.0, 2.0, 0.0, 0.0, 2.0, 0.5, 0.0, 0.0, 0.0 };

    //Render Paddle One
    program.SetModelMatrix(paddleOneMatrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    //Render Paddle Two
    program.SetModelMatrix(paddleTwoMatrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    //Render Ball
    float vertices2[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    float texCoords2[] = { 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0 };

    program.SetModelMatrix(ballMatrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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

