#include "Level1.h"

#define LEVEL1_WIDTH 30
#define LEVEL1_HEIGHT 20

#define LEVEL_1_ENEMY_COUNT 1

unsigned int level1_data[] = {
    91, 55, 55, 55, 55, 55, 55, 55, 55, 91, 91, 91, 91, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 91, 91, 55, 55, 55, 55, 55,
    91, 55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 91, 55,  3,  4,  5, 55,
    91, 55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 91, 55, 21, 22, 23, 55,
    91, 55,  0,  0,  0,  0,  0,  0, 55, 55, 55, 55, 55, 55,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 91, 55,  0,  0,  0, 55,
    91, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 91, 55,  0,  0,  0, 55,
    91, 55,  0,  0,  0,  0,  0,  0, 55, 55, 55, 55, 55, 55,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 91, 55,  0,  0,  0, 55,
    91, 55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 91, 55,  0,  0,  0, 55,
    91, 55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 91, 55,  0,  0,  0, 55,
    91, 55, 55,  0,  0, 55, 55, 55, 55, 91, 91, 91, 91, 55, 55, 55, 55,  0,  0, 55, 55, 55, 55, 91, 91, 55,  0,  0,  0, 55,
    91, 91, 55,  0,  0, 55, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 55,  0,  0, 55, 91, 91, 91, 91, 91, 55,  0,  0,  0, 55,
    91, 91, 55,  0,  0, 55, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 55,  0,  0, 55, 91, 91, 91, 91, 91, 55,  0,  0,  0, 55,
    55, 55, 55,  0,  0, 55, 55, 55, 91, 91, 91, 91, 91, 91, 91, 91, 55,  0,  0, 55, 91, 91, 91, 91, 91, 55,  0,  0,  0, 55,
    55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55, 55, 55, 55, 55,  0,  0, 55, 55, 55, 55, 55, 91, 55,  0,  0,  0, 55,
    55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 55,  0,  0,  0, 55,
    55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55, 55, 55,  0,  0,  0, 55,
    55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55,
    55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55,
    55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55, 55, 55,  0,  0,  0, 55,
    55,  0,  0,  0,  0,  0,  0, 55, 91, 91, 91, 91, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55, 91, 55, 55, 55, 55, 55,
    55, 55, 55, 55, 55, 55, 55, 55, 91, 91, 91, 91, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 91, 91, 91, 91, 91, 91
};

void Level1::Initialize() {
    
    glClearColor(1.0f, 0.7529f, 0.4784f, 1.0f);
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 18, 8);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(4, -4, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 2.5f;
    state.player->textureID = Util::LoadTexture("link.png");
    
    state.player->animLeft = new int[2] {16, 1};
    state.player->animRight = new int[2] {18, 3};
    state.player->animUp = new int[2] {2, 17};
    state.player->animDown = new int[2] {0, 15};
    
    state.player->animLeftAttack = new int[2] {46, 1};
    state.player->animRightAttack = new int[2] {48, 3};
    state.player->animUpAttack = new int[2] {47, 2};
    state.player->animDownAttack = new int[2] {45, 0};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 15;
    state.player->animRows = 11;
    state.player->height = 0.5f;
    state.player->width = 0.5f;
    
    // Initialize weapon
    state.weapon = new Entity();
    state.weapon->entityType = WEAPON;
    state.weapon->position = glm::vec3(3.75f, -16, 0);
    state.weapon->textureID = Util::LoadTexture("sword.png");
    state.weapon->height = 0.7f;
    state.weapon->width = 0.3f;
    
    // Initialize Enemies
    state.enemies = new Entity[LEVEL_1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("font.png");

//    state.enemies[0].entityType = ENEMY;
//    state.enemies[0].textureID = enemyTextureID;
//    state.enemies[0].position = glm::vec3(11, 3, 0);
//    state.enemies[0].movement = glm::vec3(1, 0, 0);
//    state.enemies[0].speed = 0.7;
//    state.enemies[0].aiType = WALKER;
//    state.enemies[0].aiState = WALKING;
//    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
//
//    state.enemies[0].animRight = new int[2] {0, 1};
//    state.enemies[0].animLeft = new int[2] {0, 1};
//
//    state.enemies[0].animIndices = state.enemies[0].animRight;
//    state.enemies[0].animFrames = 2;
//    state.enemies[0].animIndex = 0;
//    state.enemies[0].animTime = 0;
//    state.enemies[0].animCols = 2;
//    state.enemies[0].animRows = 1;
//    state.enemies[0].height = 1;
//    state.enemies[0].width = 1;
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL_1_ENEMY_COUNT, state.map);
    state.player->Update(deltaTime, state.player, state.weapon, 1, state.map);
    state.weapon->Update(deltaTime, state.weapon, state.player, 1, state.map);
    state.enemies[0].Update(deltaTime, state.enemies, state.player, 1, state.map);
    
    if (state.player->position.x >= 26.7 and state.player->position.x <= 27.3 and state.player->position.y == -2.75) {
        state.nextScene = 1;
    }
}

void Level1::Render(ShaderProgram *program) {
    if (not state.player->startGame) {
        GLuint fontID = Util::LoadTexture("font.png");
        Util::DrawText(program, fontID, "zelda", 1.3f, 0, glm::vec3(2.5f, -2, 0));
        Util::DrawText(program, fontID, "mini", 1.3f, 0, glm::vec3(3, -3.2f, 0));
        Util::DrawText(program, fontID, "press enter", 0.6f, 0, glm::vec3(1.75f, -5, 0));
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
        state.weapon->Render(program);
        state.enemies[0].Render(program);
    }
}
