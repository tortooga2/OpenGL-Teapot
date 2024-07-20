#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Vectors.hpp"
#include "MeshLoader.hpp"


#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform vec4 ourColor;\n"
                                 // "uniform mat4 transform;\n"
                                 "out vec4 outColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   outColor = ourColor;\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"

                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec4 geoColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = geoColor;\n"
                                   "}\n\0";

const char *geomatryShaderSource = "#version 330 core\n"
                                   "layout (triangles) in;\n"
                                   "layout (triangle_strip, max_vertices = 3) out;\n"
                                   "uniform mat4 transform;\n"
                                   "in vec4 outColor[];\n"
                                   "out vec4 geoColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   for(int i = 0; i < gl_in.length(); i++)\n"
                                   "   {\n"
                                   "       gl_Position = transform * gl_in[i].gl_Position;\n"
                                   "       geoColor = outColor[i];\n"
                                   "       EmitVertex();\n"
                                   "   }\n"
                                   "   EndPrimitive();\n"
                                   "}\n\0";

float tx = 0.0f;
float ty = 0.0f;
float tz = 0.0f;

float sX = 1.0f;
float sY = 1.5f;
float sZ = 1.0f;



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);  // Request a depth buffer with 24 bits


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
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

    unsigned int geomatryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geomatryShader, 1, &geomatryShaderSource, NULL);
    glCompileShader(geomatryShader);

    glGetShaderiv(geomatryShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geomatryShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, geomatryShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geomatryShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.5f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f

    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------







    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);


        // render
        // ------
        glClearColor(0.9f, 0.5f, 0.8f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




        float color[4] = {0.5f, 0.5f, 0.9f, 1.0f};

        // draw our first triangle
        glUseProgram(shaderProgram);


        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4fv(vertexColorLocation, 1, color);




        glm::mat4x4 translateMatrix = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
                                                  0.0f, 1.0f, 0.0f, 0.0f,
                                                  0.0f, 0.0f, 1.0f, 0.0f,
                                                  tx, ty, tz, 1.0f);

        glm::mat4x4 scaleingMatrix = glm::mat4x4(sX, 0.0f, 0.0f, 0.0f,
                                                 0.0f, sY, 0.0f, 0.0f,
                                                 0.0f, 0.0f, sZ, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 1.0f);


        glm::mat4x4 transformMatrix = translateMatrix * scaleingMatrix;

        GLint transformLocation = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &transformMatrix[0][0]);


        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
        // glBindVertexArray(0); // no need to unbind it every time



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        tx += 0.002f;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        tx -= 0.002f;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ty += 0.002f;

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ty -= 0.002f;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        sX += 0.002f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        sX -= 0.002f;

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        sY += 0.002f;

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        sY -= 0.002f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}