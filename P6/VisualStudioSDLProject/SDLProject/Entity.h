#pragma once
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

#include "Mesh.h"


enum EntityType { PLAYER, ENEMY, FLOOR, BUSH, PORTAL, PORTALKEY, FIREBALL };

class Entity {
public:

    EntityType entityType;
    bool isActive;
    
    bool keyFound;
    bool win; 
    bool lose;

    bool fire;
    bool playFootsteps;
    bool playPortalChunk;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    glm::vec3 scale; 
    
    float speed;
    
    bool billboard;
    float width;
    float height;
    float depth;

    GLuint textureID;

    Mesh *mesh;
    
    glm::mat4 modelMatrix;
    
    Entity();
    
    void Update(float deltaTime, Entity* player, Entity* objects, Entity* portalKey, Entity* enemies, Entity* fireballs, int objectCount, int enemyCount);
    void Render(ShaderProgram *program);
    void DrawBillboard(ShaderProgram* program);
    bool CheckCollision(Entity* other);
};



