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

//music stuffs 
#include <SDL_mixer.h>

enum EntityType { PLAYER, PLATFORM, ENEMY, CRATE, FINISHARROW, NONE };
enum AIType{ WALKER, PATROLLER, FLIER, WAITANDGO, JUMPER}; 
enum AIState { IDLE, WALKING, ATTACKING };

class Entity {
public:
    Mix_Music* music;
    Mix_Chunk* killChunk;
    Mix_Chunk* winChunk;
    Mix_Chunk* loseChunk;

    bool killed = false;

    EntityType lastCollision;
    Entity* bottomCollision; 

    bool win = false; 
    bool lose = false;

    EntityType entityType;
    AIType aiType; 
    AIState aiState; 

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    float width = 1;
    float height = 1;

    bool jump = false; 
    float jumpPower = 0;

    float speed;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    
    bool isActive = true; 

    bool collidedTop = false; 
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false; 

    Entity();
    
    bool CheckCollision(Entity* other);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsY(Entity* objects, int objectCount);

    void Update(float deltaTime, Entity* player, Entity* platforms, Entity* crates, Entity* enemies, Entity* finishArrow, int platformCount, int crateCount, int enemyCount);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);

    void AI(Entity* player);
    void AIPatroller(Entity* player);
    void AIWalker(Entity* player);
    void AIWaitAndGo(Entity *player);
    void AIJumper(Entity* player);

    void playMusic();
    void deAllocateMusic();
};
