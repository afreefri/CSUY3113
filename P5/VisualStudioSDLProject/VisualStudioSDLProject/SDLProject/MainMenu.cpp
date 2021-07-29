#include "MainMenu.h"

#define MAIN_MENU_WIDTH 11
#define MAIN_MENU_HEIGHT 8

unsigned int mainMenu_data[] =
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

GLuint fontTextureID; 

void MainMenu::Initialize() {

    state.nextScene = -1;

    fontTextureID = Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tiles_packed.png");
    state.map = new Map(MAIN_MENU_WIDTH, MAIN_MENU_HEIGHT, mainMenu_data, mapTextureID, 1.0f, 20, 9);

    state.player = new Entity();
    state.player->isActive = false;

    state.enemies = new Entity[1];
    state.enemies[0].isActive = false;
}

void MainMenu::Update(float deltaTime) {
    
}

void MainMenu::Render(ShaderProgram* program) {
    state.map->Render(program);
    Util::DrawText(program, fontTextureID, "MUSHROOM CITY", 1.0f, -0.5f, glm::vec3(2, -3, 0));
    Util::DrawText(program, fontTextureID, "press enter to start", 0.5f, -0.25f, glm::vec3(2.6, -4.0, 0));
}