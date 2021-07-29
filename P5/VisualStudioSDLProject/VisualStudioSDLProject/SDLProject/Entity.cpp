#include "Entity.h"

Entity::Entity()
{
    lastCollision = NONE;
    bottomCollision = NULL;
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

void Entity::playMusic() {
    //MUSIC
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096); //open audio 
    music = Mix_LoadMUS("Late Night Radio.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    //SOUND
    killChunk = Mix_LoadWAV("bounce.wav");
    Mix_VolumeChunk(killChunk, MIX_MAX_VOLUME / 4);

    winChunk = Mix_LoadWAV("win.wav");
    Mix_VolumeChunk(winChunk, MIX_MAX_VOLUME / 4);

    loseChunk = Mix_LoadWAV("lose.wav");
    //Mix_VolumeChunk(loseChunk, MIX_MAX_VOLUME / 4);
}

void Entity::playWinChunk() {
    Mix_PlayChannel(-1, winChunk, 0);
}

void Entity::playLoseChunk() {
    Mix_PlayChannel(-1, loseChunk, 0);
}

void Entity::playKillChunk() {
    Mix_PlayChannel(-1, killChunk, 0);
}

void Entity::deAllocateMusic() {
    Mix_FreeMusic(music);
    Mix_FreeChunk(killChunk);
    Mix_FreeChunk(winChunk);
    Mix_FreeChunk(loseChunk);
}

void Entity::haltMusic() {
    Mix_HaltMusic();
}

bool Entity::CheckCollision(Entity* other) {
    if (other == this) return false;

    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) {
        lastCollision = other->entityType; 
        return true;
    }

    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                bottomCollision = object;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true; 
            }
        }
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
}

void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}

void Entity::AIWalker() {
    movement = glm::vec3(-1, 0, 0);
}

void Entity::AIWaitAndGo(Entity *player) {
    switch (aiState) {
        case IDLE:
            if (glm::distance(position, player->position) < 3.0f) {
                aiState = WALKING; //if someone near me, switch to walking
            }
            break;
        
        case WALKING:
            if (player->position.x < position.x) { //player to the right of me
                movement = glm::vec3(-1, 0, 0); //start moving to the left 
            }
            else { //player to right of me 
                movement = glm::vec3(1, 0, 0); //start moving to the right
            }
            break;

        case ATTACKING:
            break;
    }
}

void Entity::AIPatroller(Entity* player) {
    switch (aiState) {
    case IDLE:
        movement = glm::vec3(1, 0, 0);
        aiState = WALKING;
        break;

    case WALKING:
        if (collidedRight) {
            movement = glm::vec3(-1, 0, 0);
        }
        else if (collidedLeft) {
            movement = glm::vec3(1, 0, 0);
        }
        if (patrolRangeRight != -999 && position.x >= patrolRangeRight) {
            movement = glm::vec3(-1, 0, 0);
        }
        if (patrolRangeLeft != -999 && position.x < patrolRangeLeft) {
            movement = glm::vec3(1, 0, 0);
        }
        break;

    case ATTACKING:
        break;
    }
}

void Entity::AIUpDownPatroller(Entity* player) {
    switch (aiState) {
    case IDLE:
        movement = glm::vec3(0, 1, 0);
        animIndices = animUp;
        aiState = WALKING; 
        break;

    case WALKING:
        if (collidedTop) {
            movement = glm::vec3(0, -1, 0);
            animIndices = animDown;
        }
        else if (collidedBottom) {
            movement = glm::vec3(0, 1, 0);
            animIndices = animUp;
        }
        if (patrolRangeTop != -999 && position.y >= patrolRangeTop) {
            movement = glm::vec3(0, -1, 0);
            animIndices = animDown;
        }
        if (patrolRangeBottom != -999 && position.y <= patrolRangeBottom) {
            movement = glm::vec3(0, 1, 0);
            animIndices = animUp;
        }
        break;

    case ATTACKING:
        break;
    }
}

void Entity::AI(Entity* player) {
    switch (aiType) {
        case WALKER:
            AIWalker(); 
            //if it is a walker, call the function made to deal w it
            break; 
        case WAITANDGO:
            AIWaitAndGo(player);
            break; 
        case PATROLLER:
            AIPatroller(player);
            break;
        case UPDOWNPATROLLER:
            AIUpDownPatroller(player);
            break;
    }
}

void Entity::Update(float deltaTime, Entity *player, Entity* objects, int objectCount, Map* map)
{
    if (isActive == false) return;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false; 
    collidedRight = false; 
    lastCollision = NONE;
    bottomCollision = NULL; 
    killed = false;

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }
    
    if (jump) {
        jump = false; 
        velocity.y += jumpPower;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;

    if (entityType == ENEMY && aiType==UPDOWNPATROLLER) {
        velocity.y = movement.y * speed;
    }
    
    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount); // Fix if needed

    //CHECK COLLISIONS WITH TILEMAP
    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount); // Fix if needed

    if (entityType == ENEMY) {
        AI(player); //takes care of enemy movement and everything else
    }

    //CHECK COLLISION WITH AI
    if (entityType == PLAYER) {
        if (bottomCollision != NULL && bottomCollision->entityType == ENEMY) { //enemy defeated
            bottomCollision->isActive = false; 
            playKillChunk();
        }
        else if (lastCollision == ENEMY) { //collided with enemy 
            killed = true; 
        }
        else if (player->position.y < -15) { //fell 
            killed = true;
        }
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {

    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

