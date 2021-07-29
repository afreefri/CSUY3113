#include "Lose.h"

#define LOSE_WIDTH 11
#define LOSE_HEIGHT 8

unsigned int lose_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122
};

GLuint loseTextureID;

void Lose::Initialize() {

    state.nextScene = -1;

    loseTextureID = Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tiles_packed.png");
    state.map = new Map(LOSE_WIDTH, LOSE_HEIGHT, lose_data, mapTextureID, 1.0f, 20, 9);

    state.player = new Entity();
    state.player->isActive = false;

    state.enemies = new Entity[1];
    state.enemies[0].isActive = false;

}

void Lose::Update(float deltaTime) {

}

void Lose::Render(ShaderProgram* program) {
    state.map->Render(program);
    Util::DrawText(program, loseTextureID, "YOU LOST :(", 1.0f, -0.5f, glm::vec3(3, -3, 0));
}