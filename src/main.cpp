#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#include "board.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 720;

const char* Vertex_Shader =
        "#version 410\n"
        "in vec3 vp;"
        "in vec2 aTexCoord;"
        "out vec2 TexCoord;"
        "void main () {"
        "  gl_Position = vec4 (vp, 1.0);"
        "  TexCoord = vec2(aTexCoord.x, aTexCoord.y);"
        "}";

const char* Fragment_Shader =
        "#version 410\n"
        "out vec4 FragColor;"
        "in vec2 TexCoord;"
        "uniform sampler2D myTexture;"
        "void main()"
        "{"
        "    FragColor = texture(myTexture, TexCoord);"
        "}";

GLfloat Block_Verticies[16][20] =
{
    {
        //Block 0
        //Position              //Texture Coordinates
        -0.375f, 0.419444f, 0.0f, 1.0f, 1.0f,  // top right
        -0.375f, 0.152778f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.675f, 0.152778f, 0.0f, 0.0f, 0.0f,  // bottom left
        -0.675f, 0.419444f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
        //Block 1
        //Position              //Texture Coordinates
        -0.025f, 0.419444f, 0.0f, 1.0f, 1.0f,  // top right
        -0.025f, 0.152778f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.325f, 0.152778f, 0.0f, 0.0f, 0.0f,  // bottom left
        -0.325f, 0.419444f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
        //Block 2
        //Position              //Texture Coordinates
        0.325f, 0.419444f, 0.0f, 1.0f, 1.0f,  // top right
        0.325f, 0.152778f, 0.0f, 1.0f, 0.0f,  // bottom right
        0.025f, 0.152778f, 0.0f, 0.0f, 0.0f,  // bottom left
        0.025f, 0.419444f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
        //Block 3
        //Position              //Texture Coordinates
        0.675f, 0.419444f, 0.0f, 1.0f, 1.0f,  // top right
        0.675f, 0.152778f, 0.0f, 1.0f, 0.0f,  // bottom right
        0.375f, 0.152778f, 0.0f, 0.0f, 0.0f,  // bottom left
        0.375f, 0.419444f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
        //Block 4
        //Position              //Texture Coordinates
        -0.375f, 0.108333f, 0.0f, 1.0f, 1.0f,  // top right
        -0.375f, -0.158333f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.675f, -0.158333f, 0.0f, 0.0f, 0.0f,  // bottom left
        -0.675f, 0.108333f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 5
            //Position              //Texture Coordinates
            -0.025f, 0.108333f, 0.0f, 1.0f, 1.0f,  // top right
            -0.025f, -0.158333f, 0.0f, 1.0f, 0.0f,  // bottom right
            -0.325f, -0.158333f, 0.0f, 0.0f, 0.0f,  // bottom left
            -0.325f, 0.108333f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 6
            //Position              //Texture Coordinates
            0.325f, 0.108333f, 0.0f, 1.0f, 1.0f,  // top right
            0.325f, -0.158333f, 0.0f, 1.0f, 0.0f,  // bottom right
            0.025f, -0.158333f, 0.0f, 0.0f, 0.0f,  // bottom left
            0.025f, 0.108333f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 7
            //Position              //Texture Coordinates
            0.675f, 0.108333f, 0.0f, 1.0f, 1.0f,  // top right
            0.675f, -0.158333f, 0.0f, 1.0f, 0.0f,  // bottom right
            0.375f, -0.158333f, 0.0f, 0.0f, 0.0f,  // bottom left
            0.375f, 0.108333f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
        //Block 8
        //Position              //Texture Coordinates
        -0.375f, -0.202777f, 0.0f, 1.0f, 1.0f,  // top right
        -0.375f, -0.469444f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.675f, -0.469444f, 0.0f, 0.0f, 0.0f,  // bottom left
        -0.675f, -0.202777f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 9
            //Position              //Texture Coordinates
            -0.025f, -0.202777f, 0.0f, 1.0f, 1.0f,  // top right
            -0.025f, -0.469444f, 0.0f, 1.0f, 0.0f,  // bottom right
            -0.325f, -0.469444f, 0.0f, 0.0f, 0.0f,  // bottom left
            -0.325f, -0.202777f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 10
            //Position              //Texture Coordinates
            0.325f, -0.202777f, 0.0f, 1.0f, 1.0f,  // top right
            0.325f, -0.469444f, 0.0f, 1.0f, 0.0f,  // bottom right
            0.025f, -0.469444f, 0.0f, 0.0f, 0.0f,  // bottom left
            0.025f, -0.202777f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 11
            //Position              //Texture Coordinates
            0.675f, -0.202777f, 0.0f, 1.0f, 1.0f,  // top right
            0.675f, -0.469444f, 0.0f, 1.0f, 0.0f,  // bottom right
            0.375f, -0.469444f, 0.0f, 0.0f, 0.0f,  // bottom left
            0.375f, -0.202777f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
        //Block 12
        //Position              //Texture Coordinates
        -0.375f, -0.513888f, 0.0f, 1.0f, 1.0f,  // top right
        -0.375f, -0.780555f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.675f, -0.780555f, 0.0f, 0.0f, 0.0f,  // bottom left
        -0.675f, -0.513888f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 13
            //Position              //Texture Coordinates
            -0.025f, -0.513888f, 0.0f, 1.0f, 1.0f,  // top right
            -0.025f, -0.780555f, 0.0f, 1.0f, 0.0f,  // bottom right
            -0.325f, -0.780555f, 0.0f, 0.0f, 0.0f,  // bottom left
            -0.325f, -0.513888f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 14
            //Position              //Texture Coordinates
            0.325f, -0.513888f, 0.0f, 1.0f, 1.0f,  // top right
            0.325f, -0.780555f, 0.0f, 1.0f, 0.0f,  // bottom right
            0.025f, -0.780555f, 0.0f, 0.0f, 0.0f,  // bottom left
            0.025f, -0.513888f, 0.0f, 0.0f, 1.0f   // top left
    },
    {
            //Block 15
            //Position              //Texture Coordinates
            0.675f, -0.513888f, 0.0f, 1.0f, 1.0f,  // top right
            0.675f, -0.780555f, 0.0f, 1.0f, 0.0f,  // bottom right
            0.375f, -0.780555f, 0.0f, 0.0f, 0.0f,  // bottom left
            0.375f, -0.513888f, 0.0f, 0.0f, 1.0f   // top left
    }

};

GLuint indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
};

GLuint Position = 0;
GLuint Number = 0;
bool keys[1024] = { false };

int keyPressed = -1;
board gameBoard;

unsigned int BlockVBO[16], BoardVBO, BlockVAO[16], BlockEBO[16], BoardVAO, BoardEBO;
unsigned int BoardTexture, NumberTextures[13];
GLuint vShader, fShader, Prog;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        keys[key] = true;
    } else if(action == GLFW_RELEASE) {
        keys[key] = false;
    }

    if(keys[GLFW_KEY_ESCAPE])
    {
        glfwSetWindowShouldClose(window, 1);
    }
    else if (keys[GLFW_KEY_UP])
    {
        keyPressed = 0;
    }
    else if (keys[GLFW_KEY_DOWN])
    {
        keyPressed = 1;
    }
    else if (keys[GLFW_KEY_LEFT])
    {
        keyPressed = 2;
    }
    else if (keys[GLFW_KEY_RIGHT])
    {
        keyPressed = 3;
    }
}

void InitializeBlocks()
{
    for (int i = 0; i < 16; i++) {
        glBindVertexArray(BlockVAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, BlockVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Block_Verticies[i]), Block_Verticies[i], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BlockEBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void DrawBlocks(int i, int j, int value)
{
    int arr = i * 4 + j;
    int number = -1;
    switch (value) {
        case 2:
            number = 0;
            break;
        case 4:
            number = 1;
            break;
        case 8:
            number = 2;
            break;
        case 16:
            number = 3;
            break;
        case 32:
            number = 4;
            break;
        case 64:
            number = 5;
            break;
        case 128:
            number = 6;
            break;
        case 256:
            number = 7;
            break;
        case 512:
            number = 8;
            break;
        case 1024:
            number = 9;
            break;
        case 2048:
            number = 10;
            break;
        case 4096:
            number = 11;
            break;
        case 8192:
            number = 12;
            break;
        default:
            break;
    }
    if (number != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, NumberTextures[number]);
        glUniform1i(glGetUniformLocation(Prog, "NumberTexture"), 0);
        glBindVertexArray(BlockVAO[arr]);
        glBindBuffer(GL_ARRAY_BUFFER, BlockVBO[arr]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Block_Verticies[arr]), Block_Verticies[arr], GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void loadNumberTextures()
{
    glGenTextures(13, NumberTextures);
    int width, height, nrChannels;
    unsigned char* data;

    //Bind Texture for 2
    glBindTexture(GL_TEXTURE_2D, NumberTextures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/2.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 4
    glBindTexture(GL_TEXTURE_2D, NumberTextures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/4.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 8
    glBindTexture(GL_TEXTURE_2D, NumberTextures[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/8.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 16
    glBindTexture(GL_TEXTURE_2D, NumberTextures[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/16.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 32
    glBindTexture(GL_TEXTURE_2D, NumberTextures[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/32.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 64
    glBindTexture(GL_TEXTURE_2D, NumberTextures[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/64.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 128
    glBindTexture(GL_TEXTURE_2D, NumberTextures[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/128.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 256
    glBindTexture(GL_TEXTURE_2D, NumberTextures[7]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/256.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 512
    glBindTexture(GL_TEXTURE_2D, NumberTextures[8]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/512.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 1024
    glBindTexture(GL_TEXTURE_2D, NumberTextures[9]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/1024.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 2048
    glBindTexture(GL_TEXTURE_2D, NumberTextures[10]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/2048.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 4096
    glBindTexture(GL_TEXTURE_2D, NumberTextures[11]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/4096.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Bind Texture for 8192
    glBindTexture(GL_TEXTURE_2D, NumberTextures[12]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/8192.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void InitializeShaders()
{
    glShaderSource(vShader, 1, &Vertex_Shader, NULL);
    glCompileShader(vShader);
    glShaderSource(fShader, 1, &Fragment_Shader, NULL);
    glCompileShader(fShader);
    glAttachShader(Prog, vShader);
    glAttachShader(Prog, fShader);
    glLinkProgram(Prog);
    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

int main()
{
    // Initialize GLFW
    if(!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        return 1;
    }

    // Always terminate GLFW when exiting
    atexit(glfwTerminate);

    // Build GL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2048 Game", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    //Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Build shaders and program
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    Prog = glCreateProgram();

    //Init shaders
    InitializeShaders();

    GLfloat Board_Verticies[] = {
            //Position               //Texture Coordinates
            1.0f,  1.0f, 0.0f,      1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,      1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f,      0.0f, 0.0f,  // bottom left
            -1.0f,  1.0f, 0.0f,      0.0f, 1.0f   // top left
    };

    glGenVertexArrays(16, BlockVAO);
    glGenVertexArrays(1, &BoardVAO);
    glGenBuffers(16, BlockVBO);
    glGenBuffers(1, &BoardVBO);
    glGenBuffers(16, BlockEBO);
    glGenBuffers(1, &BoardEBO);

    InitializeBlocks();

    glBindVertexArray(BoardVAO);
    glBindBuffer(GL_ARRAY_BUFFER, BoardVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Board_Verticies), Board_Verticies, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoardEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &BoardTexture);
    glBindTexture(GL_TEXTURE_2D, BoardTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../resources/board.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    loadNumberTextures();

    glUseProgram(Prog);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(Prog);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BoardTexture);
        glUniform1i(glGetUniformLocation(Prog, "BoardTexture"), 0);
        glBindVertexArray(BoardVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int value = gameBoard.GetValue(i, j);
                DrawBlocks(i, j, value);
            }
        }

        if (keyPressed != -1) {
            switch (keyPressed) {
                case 0:
                    gameBoard.ShiftUp();
                    break;
                case 1:
                    gameBoard.ShiftDown();
                    break;
                case 2:
                    gameBoard.ShiftLeft();
                    break;
                case 3:
                    gameBoard.ShiftRight();
                    break;
                default:
                    break;
            }
            keyPressed = -1;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &BoardVAO);
    glDeleteVertexArrays(16, BlockVAO);
    glDeleteBuffers(16, BlockVBO);
    glDeleteBuffers(1, &BoardVBO);
    glDeleteBuffers(1, &BoardEBO);
    glDeleteBuffers(16, BlockEBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}