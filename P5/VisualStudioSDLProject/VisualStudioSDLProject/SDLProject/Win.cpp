#include "Win.h"

#define WIN_WIDTH 11
#define WIN_HEIGHT 8

unsigned int win_data[] =
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

GLuint winTextureID;

void Win::Initialize() {

    state.nextScene = -1;

    winTextureID = Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tiles_packed.png");
    state.map = new Map(WIN_WIDTH, WIN_HEIGHT, win_data, mapTextureID, 1.0f, 20, 9);

    state.player = new Entity();
    state.player->isActive = false;

    state.enemies = new Entity[1];
    state.enemies[0].isActive = false;
}

void Win::Update(float deltaTime) {

}

void Win::Render(ShaderProgram* program) {
    state.map->Render(program);
    Util::DrawText(program, winTextureID, "YOU WON!!", 1.0f, -0.5f, glm::vec3(3, -3, 0));
}