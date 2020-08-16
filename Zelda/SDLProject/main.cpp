#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Mix_Music *music;
Mix_Chunk *attack;

Scene *currentScene;
Scene *sceneList[3];

int lives = 3;
bool holdingWeapon = false;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(30, 139, 195, 1);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    sceneList[0] = new Level1();
    sceneList[1] = new Level2();
    sceneList[2] = new Level3();
    SwitchToScene(sceneList[1]);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("theme.mp3");
    attack = Mix_LoadWAV("attack.wav");
    Mix_PlayMusic(music, -1);
}

void ProcessInput() {
    
    currentScene->state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_SPACE:
                        if (currentScene->state.player->holdingWeapon) {
                            Mix_PlayChannel(-1, attack, 0);
                            currentScene->state.player->attack = true;
                            if (currentScene->state.player->lookingLeft) {
                                currentScene->state.player->animIndices = currentScene->state.player->animLeftAttack;
                            }
                            if (currentScene->state.player->lookingRight) {
                                currentScene->state.player->animIndices = currentScene->state.player->animRightAttack;
                            }
                            if (currentScene->state.player->lookingUp) {
                                currentScene->state.player->animIndices = currentScene->state.player->animUpAttack;
                            }
                            if (currentScene->state.player->lookingDown) {
                                currentScene->state.player->animIndices = currentScene->state.player->animDownAttack;
                            }
                        }
                        break;
                    case SDLK_RETURN:
                        currentScene->state.player->startGame = true;
                }
            break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->attack = false;
        currentScene->state.player->lookingLeft = true;
        currentScene->state.player->lookingRight = false;
        currentScene->state.player->lookingUp = false;
        currentScene->state.player->lookingDown = false;
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->attack = false;
        currentScene->state.player->lookingLeft = false;
        currentScene->state.player->lookingRight = true;
        currentScene->state.player->lookingUp = false;
        currentScene->state.player->lookingDown = false;
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    else if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->attack = false;
        currentScene->state.player->lookingLeft = false;
        currentScene->state.player->lookingRight = false;
        currentScene->state.player->lookingUp = true;
        currentScene->state.player->lookingDown = false;
        currentScene->state.player->movement.y = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animUp;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->attack = false;
        currentScene->state.player->lookingLeft = false;
        currentScene->state.player->lookingRight = false;
        currentScene->state.player->lookingUp = false;
        currentScene->state.player->lookingDown = true;
        currentScene->state.player->movement.y = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animDown;
    }

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    lives += currentScene->state.player->lives;
    currentScene->state.player->lives = 0;
    if (currentScene->state.player->holdingWeapon) {
        holdingWeapon = true;
    }
    if (holdingWeapon) {
        currentScene->state.player->holdingWeapon = true;
    }
    if (lives == 0) {
        currentScene->state.player->isActive = false;
        currentScene->state.player->gameOver = true;
    }
    if (currentScene->state.player->position.x >= 5 and currentScene->state.player->position.x < currentScene->state.map->getMapLength() - 6) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 0, 0));
    }
    else {
        if (currentScene->state.player->position.x < 5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 0, 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-(currentScene->state.map->getMapLength() - 6), 0, 0));
        }
    }
    if (currentScene->state.player->position.y <= -4 and currentScene->state.player->position.y > -(currentScene->state.map->getMapHeight() - 4.5)) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0, -currentScene->state.player->position.y, 0));
    }
    else {
        if (currentScene->state.player->position.y > -4) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 4, 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(0, currentScene->state.map->getMapHeight() - 4.5, 0));
        }
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        
        if (currentScene->state.nextScene >= 0) {
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        }
        
        Render();
    }
    
    Shutdown();
    return 0;
}
