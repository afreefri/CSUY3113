#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    rotation = glm::vec3(0);
    scale = glm::vec3(1);

    modelMatrix = glm::mat4(1.0f);
    
    speed = 0.0f;

    billboard = false; 
    isActive = true;

    keyFound = false; 
    win = false; 
    lose = false; 

    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;

    fire = false;

}

bool Entity::CheckCollision(Entity* other)
{
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;

    return false;
}

void Entity::Update(float deltaTime, Entity* player, Entity* objects, Entity* portalKey, Entity* enemies, Entity* fireballs, int objectCount, int enemyCount)
{
    if (isActive == false) return;

    glm::vec3 previousPosition = position;



    //rotating ENEMY billboard to face player 
    if (billboard && entityType==ENEMY) {
        //turn towards the player 
        float directionX = position.x - player->position.x;
        float directionZ = position.z - player->position.z;
        rotation.y = glm::degrees(atan2f(directionX, directionZ));

        //go forward if you're close to the player 
        if (glm::distance(position, player->position) < 3.0f) {
            velocity.z = cos(glm::radians(rotation.y)) * -1.0f;
            velocity.x = sin(glm::radians(rotation.y)) * -1.0f;
            playFootsteps = true;
        }
        else {
            playFootsteps = false;
        }
        
    }





    //play portal sound
    if (entityType == PORTAL) {
        if (glm::distance(position, player->position) < 3.0f) {
            playPortalChunk = true;
        }
        else {
            playPortalChunk = false;
        }
    }


    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;




    //FIREBALLS
    if (fire) {
        fireballs->isActive = true;
        fireballs->position = glm::vec3(player->position.x, 0.5f, player->position.z);

        float directionX = position.x - player->position.x;
        float directionZ = position.z - player->position.z;

        fireballs->rotation.x = 270;
        fireballs->rotation.z = player->rotation.y + 180;

        fireballs->velocity.z = cos(glm::radians(player->rotation.y)) * -6.0f;
        fireballs->velocity.x = sin(glm::radians(player->rotation.y)) * -6.0f;

        fire = false;
    }



    //CHECK COLLISIONS
    if (entityType == PLAYER || entityType == ENEMY) {

        //collision with objects
        for (int i = 0; i < objectCount; i++)
        {
            if (objects[i].isActive == false) continue;

            // Ignore collisions with the floor and portal
            if (objects[i].entityType == FLOOR || objects[i].entityType == PORTAL) continue;

            if (CheckCollision(&objects[i])) {
                position = previousPosition;
                break;
            }
        }

        //collision with enemies
        for (int i = 0; i < enemyCount; i++) {
            if (entityType == ENEMY) break;
            if (enemies[i].isActive == false) continue;

            if (CheckCollision(&enemies[i])) {
                player->lose = true;
            }
        }

        //collision with key 
        if (portalKey->isActive) {
            if(CheckCollision(portalKey)){
                keyFound = true;
                portalKey->isActive = false;
            }
        }

        //check if left through portal 
        if (position.z >= 10) {
            win = true;
        }

    }

    //FIREBALL COLLISIONS
    if (entityType == FIREBALL) {
        for (int i = 0; i < enemyCount; i++) { //DEFEAT ENEMY
            if (CheckCollision(&enemies[i])) {
                enemies[i].isActive = false;
                isActive = false;
            }
        }

        for (int i = 0; i < objectCount; i++) {
            if (CheckCollision(&objects[i])) {
                isActive = false;
            }
        }
    }



    if (entityType == PORTALKEY) { //rotating continuously 
        //rotation.x += -45 * deltaTime; 
        rotation.y += 45 * deltaTime;

    }
    else if (entityType == ENEMY) {
        //rotation.y += 30 * deltaTime;
    }
    
    


    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);
    
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (billboard) {
        DrawBillboard(program);
    }
    else {
        mesh->Render(program);
    }
}

void Entity::DrawBillboard(ShaderProgram* program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

