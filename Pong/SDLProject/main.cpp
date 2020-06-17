#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrixLeft, modelMatrixRight, modelMatrixPuck, projectionMatrix;
glm::vec3 leftPlayerPosition = glm::vec3(0, 0, 0);
glm::vec3 leftPlayerMovement  = glm::vec3(0, 0, 0);
glm::vec3 rightPlayerPosition = glm::vec3(0, 0, 0);
glm::vec3 rightPlayerMovement  = glm::vec3(0, 0, 0);
float playerSpeed = 2.0f;
glm::vec3 puckPosition = glm::vec3(0, 0, 0);
glm::vec3 puckMovement = glm::vec3(0, 0, 0);

bool Collided(glm::vec3 paddlePos, glm::vec3 puckPos) {
    float puckxLeft = puckPos.x - 0.25;
    float puckxRight = puckPos.x + 0.25;
    float puckyTop = puckPos.y + 0.25;
    float puckyBottom = puckPos.y - 0.25;
    float paddleTop = paddlePos.y + 1;
    float paddleBottom = paddlePos.y - 1;
    bool touch = false;
    if ((puckyTop < paddleTop) and (puckyTop > paddleBottom)) {
        touch = true;
    }
    if ((puckyBottom < paddleTop) and (puckyBottom > paddleBottom)) {
        touch = true;
    }
    if (((puckxRight >= 4.0f) or (puckxLeft <= -4.0f)) and touch) {
        return true;
    }
    else {
        return false;
    }
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 640);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrixLeft = glm::mat4(1.0f);
    modelMatrixRight = glm::mat4(1.0f);
    modelMatrixPuck = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ProcessInput() {
    leftPlayerMovement = glm::vec3(0);
    rightPlayerMovement = glm::vec3(0);
    float upSpeed = 1.0f;
    float upHoldSpeed = 4.0f;
    float downSpeed = -1.0f;
    float downHoldSpeed = -4.0f;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        leftPlayerMovement.y = upSpeed;
                        break;
                    case SDLK_s:
                        leftPlayerMovement.y = downSpeed;
                        break;
                    case SDLK_UP:
                        rightPlayerMovement.y = upSpeed;
                        break;
                    case SDLK_DOWN:
                        rightPlayerMovement.y = downSpeed;
                        break;
                    case SDLK_SPACE:
                        puckMovement.x = -0.6;
                        puckMovement.y = -0.6;
                        break;
                }
            break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_W]) {
        leftPlayerMovement.y = upHoldSpeed;
    }
    
    else if (keys[SDL_SCANCODE_S]) {
        leftPlayerMovement.y = downHoldSpeed;
    }
    
    if (keys[SDL_SCANCODE_UP]) {
        rightPlayerMovement.y = upHoldSpeed;
    }
    
    else if (keys[SDL_SCANCODE_DOWN]) {
        rightPlayerMovement.y = downHoldSpeed;
    }
    
    if (glm::length(leftPlayerMovement) > 1.0f) {
        leftPlayerMovement = glm::normalize(leftPlayerMovement);
    }
    
    if (glm::length(rightPlayerMovement) > 1.0f) {
        rightPlayerMovement = glm::normalize(rightPlayerMovement);
    }
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    //puckMovement = glm::vec3(0);
    
    leftPlayerPosition += leftPlayerMovement * playerSpeed * deltaTime;
    rightPlayerPosition += rightPlayerMovement * playerSpeed * deltaTime;
    puckPosition += puckMovement * playerSpeed * deltaTime;
    
    modelMatrixLeft = glm::mat4(1.0f);
    modelMatrixRight = glm::mat4(1.0f);
    modelMatrixPuck = glm::mat4(1.0f);
    if (leftPlayerPosition.y < 4.0f and leftPlayerPosition.y > -4.0f) {
        modelMatrixLeft = glm::translate(modelMatrixLeft, leftPlayerPosition);
    }
    else if (leftPlayerPosition.y >= 4.0f){
        leftPlayerPosition = glm::vec3(0, 4, 0);
        modelMatrixLeft = glm::translate(modelMatrixLeft, leftPlayerPosition);
    }
    else if (leftPlayerPosition.y <= -4.0f){
        leftPlayerPosition = glm::vec3(0, -4, 0);
        modelMatrixLeft = glm::translate(modelMatrixLeft, leftPlayerPosition);
    }
    if (rightPlayerPosition.y < 4.0f and rightPlayerPosition.y > -4.0f) {
        modelMatrixRight = glm::translate(modelMatrixRight, rightPlayerPosition);
    }
    else if (rightPlayerPosition.y >= 4.0f){
        rightPlayerPosition = glm::vec3(0, 4, 0);
        modelMatrixRight = glm::translate(modelMatrixRight, rightPlayerPosition);
    }
    else if (rightPlayerPosition.y <= -4.0f){
        rightPlayerPosition = glm::vec3(0, -4, 0);
        modelMatrixRight = glm::translate(modelMatrixRight, rightPlayerPosition);
    }
    if (Collided(leftPlayerPosition, puckPosition) or Collided(rightPlayerPosition, puckPosition)) {
        puckMovement.x = -1 * puckMovement.x;
    }
    else if (fabs(puckPosition.y) >= 4.5f) {
        puckMovement.y = -1 * puckMovement.y;
    }
    else if (fabs(puckPosition.x) >= 4.0f) {
        SDL_Quit();
    }
    modelMatrixPuck = glm::translate(modelMatrixPuck, puckPosition);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetModelMatrix(modelMatrixLeft);

    float leftVertices[] = { -5.0f, -1.0f, -5.0f, 1.0f, -4.0f, 1.0f, -5.0f, -1.0f, -4.0f, -1.0f, -4.0f, 1.0f };
    float leftTexCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, leftVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, leftTexCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    //glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    program.SetModelMatrix(modelMatrixRight);

    float rightVertices[] = { 5.0f, -1.0f, 5.0f, 1.0f, 4.0f, 1.0f, 5.0f, -1.0f, 4.0f, -1.0f, 4.0f, 1.0f};
    float rightTexCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, rightVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, rightTexCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    //glBindTexture(GL_TEXTURE_2D, sunTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    program.SetModelMatrix(modelMatrixPuck);

    float puckVertices[] = { -0.25f, -0.25f, -0.25f, 0.25f, 0.25f, 0.25f, -0.25f, -0.25f, 0.25f, -0.25f, 0.25f, 0.25f};
    float puckTexCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, puckVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, puckTexCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    //glBindTexture(GL_TEXTURE_2D, sunTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

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
        Render();
    }

    Shutdown();
    return 0;
}

