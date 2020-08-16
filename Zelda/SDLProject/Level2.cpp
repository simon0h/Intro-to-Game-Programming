#include "Level2.h"

#define LEVEL2_WIDTH 30
#define LEVEL2_HEIGHT 20

#define LEVEL_2_ENEMY_COUNT 4

unsigned int level2_data[] = {
    106,  67, 67, 67, 67, 67,  67,  67,  67, 106,  106, 106, 106,  67,  67,  67, 67, 67, 67, 67,  67,  67,  67, 106, 106,  67,  67,  67,  67,  67,
    106,  67,  0,  0,  0,  0,   0,   0,  67, 106,  106, 106, 106,  67,   0,   0,  0,  0,  0,  0,   0,   0,  67, 106, 106,  67,   0,   0,   0,  67,
    106,  67,  0,  0,  0,  0,   0,   0,  67, 106,  106, 106, 106,  67,   0,   0,  0,  0,  0,  0,   0,   0,  67, 106, 106,  67,   0,   0,   0,  67,
    106,  67,  0,  0,  0,  0,   0,   0,  67,  67,   67,  67,  67,  67,   0,   0,  0,  0,  0,  0,   0,   0,  67, 106, 106,  67,   0,   0,   0,  67,
    106,  67,  0,  0,  0,  0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,  0,  0,  0,  0,   0,   0,  67, 106, 106,  67,   0,   0,   0,  67,
    106,  67,  0,  0,  0,  0,   0,   0,  67,  67,   67,  67,  67,  67,   0,   0,  0,  0,  0,  0,   0,   0,  67, 106, 106,  67,   0,   0,   0,  67,
    106,  67,  0,  0,  0,  0,   0,   0,  67,  106, 106, 106, 106,  67,   0,   0,  0,  0,  0,  0,   0,   0,  67, 106, 106,  67,   0,   0,   0,  67,
    106,  67,  0,  0,  0,  0,   0,   0,  67,  106, 106, 106, 106,  67,   0,   0,  0,  0,  0,  0,   0,   0,  67, 106, 106,  67,   0,   0,   0,  67,
    106,  67, 67,  0,  0, 67,  67,   67, 67,  106, 106, 106, 106,  67,  67,  67, 67,  0,  0, 67,  67,  67,  67, 106, 106,  67,   0,   0,   0,  67,
    106, 106, 67,  0,  0, 67,  106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 67,  0,  0, 67, 106, 106, 106, 106, 106,  67,   0,   0,   0,  67,
    106, 106, 67,  0,  0, 67,  106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 67,  0,  0, 67, 106, 106, 106, 106, 106,  67,   0,   0,   0,  67,
    67,   67, 67,  0,  0, 67,  67,   67, 106, 106, 106, 106, 106, 106, 106, 106, 67,  0,  0, 67, 106, 106, 106, 106, 106,  67,   0,   0,   0,  67,
    67,    0,  0,  0,  0,  0,   0,   67, 106, 106, 106, 106,  67,  67,  67,  67, 67,  0,  0, 67,  67,  67,  67,  67, 106,  67,   0,   0,   0,  67,
    67,    0,  0,  0,  0,  0,   0,   67, 106, 106, 106, 106,  67,   0,   0,   0,  0,  0,  0,  0,   0,   0,   0,  67, 106,  67,   0,   0,   0,  67,
    67,    0,  0, 15, 16, 17,   0,   67, 106, 106, 106, 106,  67,   0,   0,   0,  0,  0,  0,  0,   0,   0,   0,  67,  67,  67,   0,   0,   0,  67,
    67,    0,  0, 33, 34, 35,   0,   67, 106, 106, 106, 106,  67,   0,   0,   0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  67,
    67,    0,  0,  0,  0,  0,   0,   67, 106, 106, 106, 106,  67,   0,   0,   0,  0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  67,
    67,    0,  0,  0,  0,  0,   0,   67, 106, 106, 106, 106,  67,   0,   0,   0,  0,  0,  0,  0,   0,   0,   0,  67,  67,  67,   0,   0,   0,  67,
    67,    0,  0,  0,  0,  0,   0,   67, 106, 106, 106, 106,  67,   0,   0,   0,  0,  0,  0,  0,   0,   0,   0,  67, 106,  67,  67,  67,  67,  67,
    67,   67, 67, 67, 67, 67,  67,   67, 106, 106, 106, 106,  67,  67,  67,  67, 67, 67, 67, 67,  67,  67,  67,  67, 106, 106, 106, 106, 106, 106
};

void Level2::Initialize() {
    
    glClearColor(0.4784f, 0.4784f, 0.4784f, 1.0f);
    
    state.nextScene = -1;
       
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 18, 8);
   
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->spawnPosition = glm::vec3(27, -2.75f, 0);
    state.player->position = glm::vec3(27, -2.75f, 0);
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

    // Initialize Enemies
    state.enemies = new Entity[LEVEL_2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemies.png");
    for (int i = 0; i < LEVEL_2_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].speed = 0.8f;
        state.enemies[i].aiType = WALKER;
        state.enemies[i].aiState = WALKING;

        state.enemies[i].animRight = new int[2] {3, 18};
        state.enemies[i].animLeft = new int[2] {1, 16};
        state.enemies[i].animUp = new int[2] {2, 17};
        state.enemies[i].animDown = new int[2] {0, 15};

        state.enemies[i].animIndices = state.enemies[0].animRight;
        state.enemies[i].animFrames = 2;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 15;
        state.enemies[i].animRows = 11;
        state.enemies[i].height = 0.9f;
        state.enemies[i].width = 0.9f;
    }
    state.enemies[0].position = glm::vec3(3.75f, -16, 0);
    state.enemies[0].movement = glm::vec3(0, 1.5f, 0);
    state.enemies[1].position = glm::vec3(16, -1, 0);
    state.enemies[1].movement = glm::vec3(0, 1.5f, 0);
    state.enemies[2].position = glm::vec3(17, -7, 0);
    state.enemies[2].movement = glm::vec3(1.5f, 0, 0);
    state.enemies[3].position = glm::vec3(27, -16, 0);
    state.enemies[3].movement = glm::vec3(0, -1.5f, 0);
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL_2_ENEMY_COUNT, state.map);
    for (int i = 0; i < LEVEL_2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.enemies, state.player, 1, state.map);
    }
    if (state.player->position.x >= 3.75 and state.player->position.x <= 4.29 and state.player->position.y == -15.75) {
        state.nextScene = 2;
    }
}

void Level2::Render(ShaderProgram *program) {
    if (state.player->gameOver) {
        GLuint fontID = Util::LoadTexture("font.png");
        Util::DrawText(program, fontID, "game over", 1, 0, glm::vec3(state.player->position.x - 7, state.player->position.y, 0));
    }
    else {
        state.map->Render(program);
        state.player->Render(program);
        for (int i = 0; i < LEVEL_2_ENEMY_COUNT; i++) {
            state.enemies[i].Render(program);
        }
    }
}
