#include "Level1.h"

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

#define LEVEL_1_ENEMY_COUNT 1

unsigned int level1_data[] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("blocks.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 5, 4);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(4, 4, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.5f;
    state.player->textureID = Util::LoadTexture("characters.gif");
    
    state.player->animRight = new int[3] {16, 17, 18};
    state.player->animLeft = new int[3] {11, 10, 9};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 3;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 28;
    state.player->animRows = 1;
    state.player->height = 2;
    state.player->width = 1;
    state.player->jumpPower = 7;
    
    // Initialize Enemies
    state.enemies = new Entity[LEVEL_1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemies.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(11, 3, 0);
    state.enemies[0].movement = glm::vec3(1, 0, 0);
    state.enemies[0].speed = 0.7;
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
    
    state.enemies[0].animRight = new int[2] {0, 1};
    state.enemies[0].animLeft = new int[2] {0, 1};
    
    state.enemies[0].animIndices = state.enemies[0].animRight;
    state.enemies[0].animFrames = 2;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 2;
    state.enemies[0].animRows = 1;
    state.enemies[0].height = 1;
    state.enemies[0].width = 1;
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL_1_ENEMY_COUNT, state.map);
    state.enemies[0].Update(deltaTime, state.enemies, state.player, 1, state.map);
    
    if (not state.enemies[0].isActive) {
        state.nextScene = 1;
    }
}
void Level1::Render(ShaderProgram *program) {
    if (not state.player->startGame) {
        GLuint fontID = Util::LoadTexture("font.png");
        Util::DrawText(program, fontID, "Start", 1.3f, 0, glm::vec3(2.5f, -2, 0));
        Util::DrawText(program, fontID, "Game", 1.3f, 0, glm::vec3(3, -3.2f, 0));
        Util::DrawText(program, fontID, "Press Enter", 0.3f, 0, glm::vec3(3.5f, -5, 0));
    }
    else if (state.player->gameOver) {
        GLuint fontID = Util::LoadTexture("font.png");
        Util::DrawText(program, fontID, "Game Over", 1, 0, glm::vec3(state.player->position.x - 3.25f, -3, 0));
        state.map->Render(program);
        state.player->Render(program);
        state.enemies[0].Render(program);
    }
    else {
        state.map->Render(program);
        state.player->Render(program);
        state.enemies[0].Render(program);
    }
}
