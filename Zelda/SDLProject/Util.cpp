#include "Util.h"
#define STB_IMAGE_IMPLEMENTATION
#include <SDL_image.h>
#include "stb_image.h"

GLuint Util::LoadTexture(const char* filePath) {
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

void Util::DrawText(ShaderProgram *program, GLuint fontTexture, std::string text, float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 9.0f;
    float height = 1.0f / 3.0f;
    std::vector<float> vertices;
    std::vector<float> texCoords;
    for(int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        if ((int)text[i] != 32) {
            index += 11;
        }
        else {
            index += 21;
        }
//        int index = (int)text[i] + 11;
//        std::cout << text[i] << " " << (int)text[i] << "\n";
        float u = (float)(index % 9) / 9.0f + 0.02f;
        float v = (float)(index / 9) / 3.0f + 0.02f;
        float offset = (size + spacing) * i - 0.04f;
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height - 0.04f,
            u + width - 0.04f, v,
            u + width - 0.04f, v + height - 0.04f,
            u + width - 0.04f, v,
            u, v + height - 0.04f,
        });
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
    }
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
