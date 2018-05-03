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
//#include "loadTextures.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void openmenu();
void loadNumberTextures();
void loadMenuTextures();
void loadBoardTexture();
void InitializeBlocks();
void DrawBlocks(int i, int j, int value);
void InitializeShaders();
static int menu_value = 0;

// variables
bool open_menu = false;
bool cursorInWindow;
static double xpos, ypos;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursorPositionCallback( GLFWwindow *window, double xpos, double ypos );
void cursorEnterCallback( GLFWwindow *window, int entered );
void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods );
void scrollCallback( GLFWwindow *window, double xoffset, double yoffset );

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

unsigned int BlockVBO[16], BoardVBO, BlockVAO[16], BlockEBO[16], BoardVAO, BoardEBO, MenuVBO[6], MenuVAO[6], MenuEBO[6];
unsigned int BoardTexture, NumberTextures[13], MenuTexture[6];
GLuint vShader, fShader, Prog;

unsigned int WinTexture, LoseTexture;
int continuecheck = 0;
int Reset = 0;
int Continue = 0;


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
    glGenVertexArrays(6, MenuVAO);  //

    glGenBuffers(16, BlockVBO);
    glGenBuffers(1, &BoardVBO);
    glGenBuffers(6, MenuVBO);  //

    glGenBuffers(16, BlockEBO);
    glGenBuffers(1, &BoardEBO);
    glGenBuffers(6, MenuEBO); //

    InitializeBlocks();
    loadNumberTextures();


    // Initialize Board
    glBindVertexArray(BoardVAO);
    glBindBuffer(GL_ARRAY_BUFFER, BoardVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Board_Verticies), Board_Verticies, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoardEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    loadBoardTexture();


    //Some dummy code to load up and bind the Win Screen
    //Currently, the loss screen is the same thing

    glGenTextures(1, &WinTexture);
    glBindTexture(GL_TEXTURE_2D, WinTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int WinWidth, WinHeight, WinnrChannels;
    unsigned char* Windata = stbi_load("resources/Win.jpg", &WinWidth, &WinHeight, &WinnrChannels, 0);
    if (Windata)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WinWidth, WinHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Windata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(Windata);




    // Initialize Menu
    for (int i = 0; i < 6; i++) {
        glBindVertexArray(MenuVAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, MenuVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Board_Verticies), Board_Verticies, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MenuEBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    loadMenuTextures(); //

    glUseProgram(Prog);

    glfwSetCursorPosCallback( window, cursorPositionCallback );
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );

    glfwSetCursorEnterCallback( window, cursorEnterCallback );

    glfwSetMouseButtonCallback( window, mouseButtonCallback );
    glfwSetInputMode( window, GLFW_STICKY_MOUSE_BUTTONS, 1 );

    glfwSetScrollCallback( window, scrollCallback );

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(Prog);


        glfwGetCursorPos( window, &xpos, &ypos );

        if (!open_menu) {
            glActiveTexture(GL_TEXTURE0);
            glDisable(MenuTexture[menu_value]);
            glBindTexture(GL_TEXTURE_2D, BoardTexture);
            glUniform1i(glGetUniformLocation(Prog, "BoardTexture"), 0);
            glBindVertexArray(BoardVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        if (Reset == 1)
        {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    gameBoard.boardArray[r*4+c] = 0;
                }
            }
            gameBoard.ResetMoveArray(-1);
            gameBoard.Generate();
            gameBoard.Generate();
            gameBoard.score = 0;
            gameBoard.state = 0;
            Reset = 0;
        }





        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int value = gameBoard.GetValue(i, j);
                DrawBlocks(i, j, value);
            }
        }

        if (open_menu) {
            glDisable(BoardTexture);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, MenuTexture[menu_value]);
            glUniform1i(glGetUniformLocation(Prog, "MenuTexture"), 0);
            glBindVertexArray(MenuVAO[menu_value]);
//            glBindBuffer(GL_ARRAY_BUFFER, MenuVBO[menu_value]);
//            glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices, GL_STATIC_DRAW);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
                case 4:
                    openmenu();
                default:
                    break;
            }
            keyPressed = -1;
        }



        /*
         * These are some if conditions that check the state of the game
         * If you win, then it pulls up a new screen
         *      Press A to continue (Doubles the win score)
         *      Press Enter to Reset the game
         *      Press ESC to quit the game
         *
         * If you lose, then it pulls up a different screen
         *      Press R to try again
         *      Press ESC to quit the game
         *
         * Otherwise it refreshes the screen and continues
         *
         * At any point in the game, you can press ENTER to reset the game
         */

        if(gameBoard.state == 2)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, WinTexture);
            glUniform1i(glGetUniformLocation(Prog, "WinTexture"), 0);
            glBindVertexArray(BoardVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            if (continuecheck == 1)
            {
                glfwSwapBuffers(window);
                while(Continue == 0)
                {
                    //glfwSwapBuffers(window);
                    glfwPollEvents();
                };
                Continue = 0;
                continuecheck = 0;
                gameBoard.state = 0;
                gameBoard.SetWinValue(gameBoard.winValue * 2);
            }
            continuecheck = 1;

        }
        if(gameBoard.state == 0)
        {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        if(gameBoard.state == 1)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, WinTexture);
            glUniform1i(glGetUniformLocation(Prog, "WinTexture"), 0);
            glBindVertexArray(BoardVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            while(Reset == 0)
            {
                glfwSwapBuffers(window);
                glfwPollEvents();
            };
        }





    }

    glDeleteVertexArrays(1, &BoardVAO);
    glDeleteVertexArrays(16, BlockVAO);
    glDeleteVertexArrays(6, MenuVAO);
    glDeleteBuffers(16, BlockVBO);
    glDeleteBuffers(1, &BoardVBO);
    glDeleteBuffers(6, MenuVBO);
    glDeleteBuffers(1, &BoardEBO);
    glDeleteBuffers(16, BlockEBO);
    glDeleteBuffers(5, MenuEBO);


    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)){

        glfwSetWindowShouldClose(window, true);
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void openmenu(){
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
    printf("%d\n", gameBoard.UpdateState());
    printf("%d\n", gameBoard.GetWinValue());
    gameBoard.SetWinValue(4096);
    printf("%d\n", gameBoard.GetWinValue());
    menu_value++;
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

void loadMenuTextures(){
    glGenTextures(6, MenuTexture);
    int width, height, nrChannels;
    unsigned char* data;

    //Bind Texture for Menu
    glBindTexture(GL_TEXTURE_2D, MenuTexture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/Menu_Set_Goal_2048.png", &width, &height, &nrChannels, 0);
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

    //Bind Texture for Menu
    glBindTexture(GL_TEXTURE_2D, MenuTexture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/Menu_Set_Goal_4096.png", &width, &height, &nrChannels, 0);
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

    //Bind Texture for Menu
    glBindTexture(GL_TEXTURE_2D, MenuTexture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/Menu_Set_Goal_8192.png", &width, &height, &nrChannels, 0);
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


    //Bind Texture for Menu
    glBindTexture(GL_TEXTURE_2D, MenuTexture[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/Return_2048.png", &width, &height, &nrChannels, 0);
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

    //Bind Texture for Menu
    glBindTexture(GL_TEXTURE_2D, MenuTexture[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/Return_4096.png", &width, &height, &nrChannels, 0);
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

    //Bind Texture for Menu
    glBindTexture(GL_TEXTURE_2D, MenuTexture[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/Return_8192.png", &width, &height, &nrChannels, 0);
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

void loadBoardTexture(){
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



    glGenTextures(6, MenuTexture);



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

static void cursorPositionCallback( GLFWwindow *window, double xposition, double yposition )
{
    if (cursorInWindow) {
        std::cout << xposition << " : " << yposition << std::endl;
        // up arrow coordinates
        // x:291 y:64 bottom left (up)
        // x:350 y:64 bottom right (up)
        // x:291 y:11 top left (up)
        // x:350 y:11 top right (up)

        // left arrow coordinates
        // x:290 y:65 top right (left)
        // x:290 y:124 bottom right (left)
        // x:235 y:64
        // x:235 y:124

        // down arrow coordinates
        // x:291 y:126 top left (down)
        // x:350 y:126 top right (down)
        // x:291 y:176 bottom left (down)
        // x:350 y:176 bottom right (down)

        // right arrow coordinates
        // x:351 y:65  top left (right)
        // x:351 y:123 bottom left (right)
        // x:403 y:65  top right (right)
        // x:402 y:123 bottom right (right)
    }


}

void cursorEnterCallback( GLFWwindow *window, int entered )
{
    if ( entered )
    {
        cursorInWindow = true;
        std::cout << "Entered Window" << std::endl;
    }
    else
    {
        cursorInWindow= false;
        std::cout << "Left window" << std::endl;
    }
}

void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods )
{
    if ( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !open_menu)
    {
        if (xpos > 291 && xpos < 350) {
            if (ypos < 64 && ypos > 11) {
                gameBoard.ShiftUp();
            }
        }
        if (xpos > 235 && xpos < 290){
            if (ypos < 124 && ypos > 64){
                gameBoard.ShiftLeft();
            }
        }
        if (xpos > 291 && xpos < 350){
            if (ypos < 176 && ypos > 126){
                gameBoard.ShiftDown();
            }
        }
        if (xpos > 351 && xpos < 403){
            if (ypos < 123 && ypos > 65){
                gameBoard.ShiftRight();
            }
        }
    }

}

void scrollCallback( GLFWwindow *window, double xoffset, double yoffset )
{
    std::cout << xoffset << " : " << yoffset << std::endl;
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        keys[key] = true;
    } else if(action == GLFW_RELEASE) {
        keys[key] = false;
    }

    if(keys[GLFW_KEY_D]){
        open_menu = !open_menu;
    }
    if(keys[GLFW_KEY_C])
    {
        glfwSetWindowShouldClose(window, 1);
    }
    else if (keys[GLFW_KEY_UP])
    {
        if (!open_menu) {
            keyPressed = 0;
        } else {
            if (menu_value > 2) {
                menu_value -= 3;
            }
        }
    }
    else if (keys[GLFW_KEY_DOWN])
    {
        if (!open_menu) {
            keyPressed = 1;
        } else {
            if (menu_value <= 2) {
                menu_value += 3;
            }
        }
    }
    else if (keys[GLFW_KEY_LEFT])
    {
        if (!open_menu) {
            keyPressed = 2;
        } else {
            if (menu_value < 3 && menu_value > 0){
                menu_value -= 1;
            }
        }
    }
    else if (keys[GLFW_KEY_RIGHT])
    {
        if (!open_menu) {
            keyPressed = 3;
        } else {
            if (menu_value >= 0 && menu_value < 3){
                menu_value += 1;
            }
        }
    } else if (keys[GLFW_KEY_A])
    {
        keyPressed = 4;
    }
    else if (keys[GLFW_KEY_A])
    {
        Continue = 1;
    }
    else if (keys[GLFW_KEY_ENTER])
    {
        Continue = 1;
        Reset = 1;
    }
}