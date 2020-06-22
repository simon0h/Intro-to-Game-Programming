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

#include "Entity.h"

#define PLATFORM_COUNT 11

struct GameState {
    Entity *player;
    Entity *platforms;
    //Entity *landingPad;
    Entity *text;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    state.player = new Entity();
    state.player->position = glm::vec3(0, 4.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0.0f, -0.3f, 0);
    state.player->speed = 0.7f;
    state.player->textureID = LoadTexture("Lander.png");
    
//    state.player->animRight = new int[4] {3, 7, 11, 15};
//    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[2] {1, 0};
//    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animUp;
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 2;
    state.player->animRows = 1;
    state.player->height = 1.8f;
    state.player->width = 0.65f;
    state.player->jumpPower = 0.7f;
    
    state.platforms = new Entity[PLATFORM_COUNT];
    
    GLuint platformTextureID = LoadTexture("Tile.png");
    GLuint landingPadTextureID = LoadTexture("LandingPad.png");
    
    state.platforms[0].textureID = platformTextureID;
    state.platforms[0].position = glm::vec3(-5, -3.25f, 0);
    
    state.platforms[1].textureID = platformTextureID;
    state.platforms[1].position = glm::vec3(-4, -3.25f, 0);
    
    state.platforms[2].textureID = platformTextureID;
    state.platforms[2].position = glm::vec3(-3, -3.25f, 0);
    
    state.platforms[3].textureID = platformTextureID;
    state.platforms[3].position = glm::vec3(-2, -3.25f, 0);
    
    state.platforms[4].textureID = platformTextureID;
    state.platforms[4].position = glm::vec3(-1, -3.25f, 0);
    
    state.platforms[5].textureID = platformTextureID;
    state.platforms[5].position = glm::vec3(0, -3.25f, 0);
    
    state.platforms[6].textureID = platformTextureID;
    state.platforms[6].position = glm::vec3(1, -3.25f, 0);
    
    state.platforms[7].textureID = platformTextureID;
    state.platforms[7].position = glm::vec3(2, -3.25f, 0);
    
    state.platforms[8].textureID = platformTextureID;
    state.platforms[8].position = glm::vec3(4, -3.25f, 0);
    
    state.platforms[9].textureID = platformTextureID;
    state.platforms[9].position = glm::vec3(5, -3.25f, 0);
    
    state.platforms[10].textureID = landingPadTextureID;
    state.platforms[10].position = glm::vec3(3, -3.25f, 0);
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, 0);
    }
    
//    state.landingPad = new Entity;
//
//    GLuint landingPadTextureID = LoadTexture("LandingPad.png");
//
//    state.landingPad->textureID = landingPadTextureID;
//    state.landingPad->position = glm::vec3(3, -3.25f, 0);
//    state.landingPad->Update(0, NULL, 0);
}

void ProcessInput() {
    
    state.player->movement = glm::vec3(0);
    
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
                        state.player->movement.x = -1.0f;
                        break;
                        
                    case SDLK_RIGHT:
                        state.player->movement.x = 1.0f;
                        break;
                        
                    case SDLK_SPACE:
                        state.player->jump = true;
                        state.player->animIndices = state.player->animUp;
                        //state.player->animIndices = state.player->animUp;
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->acceleration.x = -30.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->acceleration.x = 30.0f;
    }
    else if (keys[SDL_SCANCODE_SPACE]) {
        state.player->jump = true;
        state.player->animIndices = state.player->animUp;
    }
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks; lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program, 0);
    }
    
    state.player->Render(&program, 1);
    
    if (state.player->isActive == false and state.player->landingSuccessful == false) {
    
        //state.text = new Entity[13];
        
        GLuint fontID = LoadTexture("Font.png");
        
        int characterIndex[] = {77, 105, 115, 115, 105, 111, 110, 70, 97, 105, 108, 101, 100};
        float originVertices[]  = { -3.5, 4.0, -2.5, 4.0, -2.5, 5.0, -3.5, 4.0, -2.5, 5.0, -3.5, 5.0 };
        float offset = 0;
        float yCoords = 4.0;
        
        for (int i = 0; i < 13; i++) {
            float u = (float)(characterIndex[i] % 16) / 16.0f;
            float v = (float)(characterIndex[i] / 16) / 16.0f;

            float width = 1.0f / 16.0f;
            float height = 1.0f / 16.0f;
            
            if (i == 7) {
                yCoords -= 1.0;
                offset = -6.5;
            }

            float texCoords[] = { u, v + height, u + width, v + height, u + width, v, u, v + height, u + width, v, u, v};
            float vertices[]  = { originVertices[0] + i + offset - state.player->position.x, yCoords, originVertices[2] + i + offset - state.player->position.x, yCoords, originVertices[4] + i + offset - state.player->position.x, yCoords + 1, originVertices[6] + i + offset - state.player->position.x, yCoords, originVertices[8] + i + offset - state.player->position.x, yCoords + 1, originVertices[10] + i + offset - state.player->position.x, yCoords + 1 };

            glBindTexture(GL_TEXTURE_2D, fontID);

            glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
            glEnableVertexAttribArray(program.positionAttribute);

            glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
            glEnableVertexAttribArray(program.texCoordAttribute);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDisableVertexAttribArray(program.positionAttribute);
            glDisableVertexAttribArray(program.texCoordAttribute);
        }
    }
    else if (state.player->isActive == false and state.player->landingSuccessful == true) {
    
        //state.text = new Entity[13];
        
        GLuint fontID = LoadTexture("Font.png");
        
        int characterIndex[] = {77, 105, 115, 115, 105, 111, 110, 83, 117, 99, 99, 101, 115, 102, 117, 108};
        float originVertices[]  = { -3.5, 4.0, -2.5, 4.0, -2.5, 5.0, -3.5, 4.0, -2.5, 5.0, -3.5, 5.0 };
        float offset = 0;
        float yCoords = 4.0;
        
        for (int i = 0; i < 16; i++) {
            float u = (float)(characterIndex[i] % 16) / 16.0f;
            float v = (float)(characterIndex[i] / 16) / 16.0f;

            float width = 1.0f / 16.0f;
            float height = 1.0f / 16.0f;
            
            if (i == 7) {
                yCoords -= 1.0;
                offset = -8.0;
            }

            float texCoords[] = { u, v + height, u + width, v + height, u + width, v, u, v + height, u + width, v, u, v};
            float vertices[]  = { originVertices[0] + i + offset - state.player->position.x, yCoords, originVertices[2] + i + offset - state.player->position.x, yCoords, originVertices[4] + i + offset - state.player->position.x, yCoords + 1, originVertices[6] + i + offset - state.player->position.x, yCoords, originVertices[8] + i + offset - state.player->position.x, yCoords + 1, originVertices[10] + i + offset - state.player->position.x, yCoords + 1 };

            glBindTexture(GL_TEXTURE_2D, fontID);

            glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
            glEnableVertexAttribArray(program.positionAttribute);

            glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
            glEnableVertexAttribArray(program.texCoordAttribute);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDisableVertexAttribArray(program.positionAttribute);
            glDisableVertexAttribArray(program.texCoordAttribute);
        }
    }
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
