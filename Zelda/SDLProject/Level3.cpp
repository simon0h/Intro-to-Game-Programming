#include "Level3.h"

#define LEVEL3_WIDTH 35
#define LEVEL3_HEIGHT 8

#define LEVEL_3_ENEMY_COUNT 8

unsigned int level3_data[] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
};

void Level3::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("blocks.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 5, 4);
    
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
    state.enemies = new Entity[LEVEL_3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemies.png");
    
    for (int i = 0; i < LEVEL_3_ENEMY_COUNT; i ++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].movement = glm::vec3(1, 0, 0);
        state.enemies[i].speed = 0.7;
        state.enemies[i].aiType = WALKER;
        state.enemies[i].aiState = WALKING;
        state.enemies[i].acceleration = glm::vec3(0, -9.81f, 0);
        
        state.enemies[i].animRight = new int[2] {0, 1};
        state.enemies[i].animLeft = new int[2] {0, 1};
        
        state.enemies[i].animIndices = state.enemies[i].animRight;
        state.enemies[i].animFrames = 2;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 2;
        state.enemies[i].animRows = 1;
        state.enemies[i].height = 1;
        state.enemies[i].width = 1;
    }
    state.enemies[0].position = glm::vec3(1, 3, 0);
    state.enemies[1].position = glm::vec3(10, 3, 0);
    state.enemies[2].position = glm::vec3(10.5f, 3, 0);
    state.enemies[3].position = glm::vec3(11, 3, 0);
    state.enemies[4].position = glm::vec3(15, 3, 0);
    state.enemies[5].position = glm::vec3(15.5f, 3, 0);
    state.enemies[6].position = glm::vec3(16, 3, 0);
    state.enemies[7].position = glm::vec3(16.5f, 3, 0);
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL_3_ENEMY_COUNT, state.map);
    for (int i = 0; i < LEVEL_3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.enemies, state.player, 1, state.map);
    }
}

void Level3::Render(ShaderProgram *program) {
    if (state.player->position.x >= 34) {
        GLuint fontID = Util::LoadTexture("font.png");
        Util::DrawText(program, fontID, "You win", 1.3f, 0, glm::vec3(25, -3, 0));
    }
    else if (state.player->gameOver) {
        GLuint fontID = Util::LoadTexture("font.png");
        Util::DrawText(program, fontID, "Game Over", 1, 0, glm::vec3(state.player->position.x - 3.25f, -3, 0));
        state.map->Render(program);
        state.player->Render(program);
        for (int i = 0; i < LEVEL_3_ENEMY_COUNT; i++) {
            state.enemies[i].Render(program);
        }
    }
    else {
        state.map->Render(program);
        state.player->Render(program);
        for (int i = 0; i < LEVEL_3_ENEMY_COUNT; i++) {
            state.enemies[i].Render(program);
        }
    }
}
