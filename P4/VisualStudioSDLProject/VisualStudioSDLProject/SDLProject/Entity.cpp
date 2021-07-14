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
    music = Mix_LoadMUS("graveyard.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    //SOUND
    killChunk = Mix_LoadWAV("bounce.wav");
    Mix_VolumeChunk(killChunk, MIX_MAX_VOLUME / 4);

    winChunk = Mix_LoadWAV("win.wav");
    Mix_VolumeChunk(winChunk, MIX_MAX_VOLUME / 4);

    loseChunk = Mix_LoadWAV("lose.wav");
    Mix_VolumeChunk(loseChunk, MIX_MAX_VOLUME / 4);
}

void Entity::deAllocateMusic() {
    Mix_FreeMusic(music);
    Mix_FreeChunk(killChunk);
    Mix_FreeChunk(winChunk);
    Mix_FreeChunk(loseChunk);
}

bool Entity::CheckCollision(Entity* other) {
    if (other == this) return false;
    //you wouldn't be able to collide with yourself 

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

void Entity::AIWalker(Entity* player) {
    movement = glm::vec3(-1, 0, 0);
}

void Entity::AIPatroller(Entity* player) {
    switch (aiState) {
        case IDLE:
            aiState = WALKING;
            movement = glm::vec3(1, 0, 0); //start moving to the right 
            animIndices = animRight; 
            break;

        case WALKING:
            //deal with collisions: 
            if (collidedRight) {
                movement = glm::vec3(-1, 0, 0); //move left
                animIndices = animLeft;
            }
            else if (collidedLeft) {
                movement = glm::vec3(1, 0, 0); //move right
                animIndices = animRight; 
            }

            //deal with moving off the page
            //I have it patroll slightly off the page 
            if (position.x < -8.0f) {
                movement = glm::vec3(1, 0, 0); //move right
                animIndices = animRight;
            }
            else if (position.x > 6.0f) {
                movement = glm::vec3(-1, 0, 0); //move left
                animIndices = animLeft; 
            }
            
            break;
    }
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

void Entity::AIJumper(Entity* player) {
    switch (aiState) {
        case IDLE:
            if (collidedBottom) {
                aiState = WALKING;
            }
            break;

        case WALKING:
            //deal with collisions 
            if (collidedTop) {
                aiState = ATTACKING;
            }
            else if (position.y > 4.0f) { //off the page 
                aiState = ATTACKING;
            }
            else if (collidedBottom) {
                acceleration = glm::vec3(0); //turn off gravity to float up
                movement = glm::vec3(0, 1.0f, 0); 
            }

            //if player within a certain distance of you, attack 
            if (glm::distance(position, player->position) < 1.0f) {
                aiState = ATTACKING;
            }
            break;

        case ATTACKING:
            acceleration = glm::vec3(0, -9.81f, 0); //turn on gravity again to jump down
            movement = glm::vec3(0, -2.0f, 0); 

            if(collidedBottom){ //once you hit the bottom, start "walking" up again
                aiState = WALKING;
            }

            break; 
    }
}
void Entity::AI(Entity* player) {
    if (player->win || player->lose) {
        movement = glm::vec3(0);
        acceleration = glm::vec3(0);
        velocity = glm::vec3(0);
        return; 
    }
    switch (aiType) {
        case WALKER:
            AIWalker(player);
            break; 
        case PATROLLER:
            AIPatroller(player);
            break;
        case WAITANDGO:
            AIWaitAndGo(player);
            break; 
        case JUMPER:
            AIJumper(player);
            break;
    }
}

void Entity::Update(float deltaTime, Entity *player, Entity *platforms, Entity *crates, Entity* enemies, Entity* finishArrow, int platformCount, int crateCount, int enemyCount)
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

    if (entityType == ENEMY) { //enemies can move up and down 
        velocity.y = movement.y * speed;
    }

    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime;           // Move on Y
    CheckCollisionsY(platforms, platformCount);     // Fix if needed
    CheckCollisionsY(crates, crateCount);
    CheckCollisionsY(enemies, enemyCount);

    position.x += velocity.x * deltaTime;           // Move on X
    CheckCollisionsX(platforms, platformCount);     // Fix if needed
    CheckCollisionsX(crates, crateCount);
    CheckCollisionsX(enemies, enemyCount);

    if (entityType == ENEMY) {
        AI(player); //takes care of enemy movement and everything else
    }

    //CHECK COLLISION WITH AI AND FINISH CHECKPOINT
    if (entityType == PLAYER) {

        if (win || lose) {
            return;
        }

        if (CheckCollision(finishArrow)) {
            win = true;
            Mix_HaltMusic();
            Mix_PlayChannel(-1, winChunk, 0);
        }
        else if (bottomCollision != NULL && bottomCollision->entityType == ENEMY) { //enemy defeated
            Mix_PlayChannel(-1, killChunk, 0);
            killed = true;
            bottomCollision->isActive = false; 

        }
        else if (lastCollision == ENEMY) { //enemy collided into you 
            lose = true; 
            Mix_HaltMusic();
            Mix_PlayChannel(-1, loseChunk, 0);
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
