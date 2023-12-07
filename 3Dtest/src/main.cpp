#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "utils.h"
#include <math.h>

/**/


const char* vertexShaderSource = "#version 330\n"
"layout(location = 0) in vec3 Position;\n"
"layout(location = 1) in vec3 vertexColor;\n"

"uniform mat4 gWorld;\n"
"out vec3 fragmentColor;\n"

"void main()\n"
"{\n"
"gl_Position = gWorld * vec4(Position, 1.0);\n"
"fragmentColor = vertexColor;\n"
"}\0";



const char* fragmentShaderSource = "#version 330\n"
"out vec4 FragColor;\n"
"in vec3 fragmentColor;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    color = fragmentColor;\n"
"}\0";


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static float Scale = 0.0f;
static float angleX = Scale;
static float angleY = Scale;
int main(void)
{

    static constexpr GLfloat vertices[] = {
        // front
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
    };

    static constexpr GLuint indices[] = {
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7
    };

    static constexpr GLfloat g_color_buffer_data[] = {
    1.0, 0.4, 0.6,
    1.0, 0.9, 0.2,
    0.7, 0.3, 0.8,
    0.5, 0.3, 1.0,
    0.2, 0.6, 1.0,
    0.6, 1.0, 0.4,
    0.6, 0.8, 0.8,
    0.4, 0.8, 0.8,
    };


    GLuint colorbuffer;
    GLuint gWorldLocation{};
    GLuint VBO{};
    GLuint EBO{};


    GLFWwindow* window;


    if (!glfwInit())
        return -1;


    window = glfwCreateWindow(1000,800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);
/*    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);*/
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //---------------------------------------------------------------------------------------------------





    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, vertices, GL_STATIC_DRAW);


    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, indices, GL_STATIC_DRAW);
    std::cout << sizeof(indices);


    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*24, g_color_buffer_data, GL_STATIC_DRAW);
    glBindVertexArray(0);

/*    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
*/

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------->



    // compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);


        // Calculate half angles for quaternion
        float halfAngleX = angleX * 0.5f;
        float halfAngleY = angleY * 0.5f;

        // Quaternion representations of rotations around X and Y axes
        Quaternionf quatRotationX = Quaternionf(cosf(halfAngleX), sinf(halfAngleX), 0.0f, 0.0f);
        Quaternionf quatRotationY = Quaternionf(cosf(halfAngleY), 0.0f, sinf(halfAngleY), 0.0f);

        Quaternionf quatRotation = quatRotationX * quatRotationY;

        Matrix4f Rotation = quatRotation.toMatrix();
        
        /*Matrix4f RotationY = { cosf(Scale), 0.0f,   -sinf(Scale), 0.0f,
                                0.0f,        1.0f,   0.0f,         0.0f,
                                sinf(Scale), 0.0f,   cosf(Scale),  0.0f,
                                0.0f,        0.0f,   0.0f,         1.0f };

        Matrix4f RotationX = {  1.0f ,        0.0f,   0.0f,           0.0f,
                                0.0f, cosf(Scale),   sinf(Scale),   0.0f,
                                0.0f, -sinf(Scale),  cosf(Scale),   0.0f,
                                0.0f,        0.0f,    0.0f,            1.0f };

        Matrix4f Rotation = RotationX * RotationY;*/

        Matrix4f Translation = { 1.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 1.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 1.0f, 2.0f,
                                 0.0f, 0.0f, 0.0f, 1.0f };

        float FOV = 100.0f;
        float tanHalfFOV = tanf(toRadian(FOV / 2.0f));
        float f = 1 / tanHalfFOV;

        Matrix4f Projection = { f, 0.0f, 0.0f, 0.0f,
                                0.0f, f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f };

        Matrix4f FinalMatrix = Projection * Translation * Rotation;
        glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &FinalMatrix.data[0][0]);



        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


        glEnableVertexAttribArray(0);
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();

    }
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    std::cout << key << "\n";
    switch(key){
    case GLFW_KEY_A:
    {
        angleY += 0.07f;
        break;
    }
    case GLFW_KEY_D:
    {
        angleY -= 0.07f;
        break;
    }
    case GLFW_KEY_W:
    {
        angleX += 0.07f;
        break;
    }
    case GLFW_KEY_S:
    {
        angleX -= 0.07f;
        break;
    }
    default:
    {
        break;
    }
    }
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}