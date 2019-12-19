#include <stdlib.h>
#include <stack>
#include <unistd.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader_util.h"

GLuint createTerrain(glm::vec3 color);
GLuint createCube(glm::vec3 color);

shader_prog shader("project.vert.glsl", "project.frag.glsl");

GLuint cubeVAO; // cube Vertex Array Object
GLuint terrainVAO;


void initTerrain() {
    terrainVAO = createTerrain(glm::vec3(0.22, 0.22, 0.22));
}

GLuint createTerrain(glm::vec3 color) {
    float s = 10.0;
    
    GLfloat vertices[] = {
                            -s, -s, 0.0,
                             s, -s, 0.0,
                             s,  s, 0.0,
                            -s,  s, 0.0
                        };

    GLubyte indices[] = {
                            0, 1, 2,
                            0, 2, 3
                        };

    GLfloat colors[] = {
                            color[0], color[1], color[2],
                            color[0], color[1], color[2],
                            color[0], color[1], color[2],
                            color[0], color[1], color[2]
                        };

    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);

    shader.attribute3fv("position", vertices, 12);
    shader.attribute3fv("color", colors, 12);

    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*12, indices, GL_STATIC_DRAW);
    return vertexArrayHandle;
}

GLuint createCube(glm::vec3 color) {
    GLfloat vertices[] = {
        -1.0, -1.0, -1.0, // 0
         1.0, -1.0, -1.0, // 1
         1.0,  1.0, -1.0, // 2
        -1.0,  1.0, -1.0, // 3
        -1.0,  1.0,  1.0, // 4
        -1.0, -1.0,  1.0, // 5
         1.0, -1.0,  1.0, // 6
         1.0,  1.0,  1.0  // 7
    };
    
    GLubyte indices[] = { // faces
        7, 5, 6, // front
        7, 4, 5, //
        2, 1, 0, // back
        2, 0, 3, //
        7, 2, 4, // top
        2, 3, 4, //
        6, 5, 1, // bottom
        1, 5, 0, //
        7, 1, 2, // right
        7, 6, 1, //
        4, 3, 0, // left
        4, 0, 5  //
    };
    
    glm::vec3 darkColor = color * 0.5f;
    int varticesCount = 8;
    int elementsPerVertex = 3;
    GLfloat colors[varticesCount * elementsPerVertex];
    for (int i = 0; i < varticesCount; i++) {
        int k = i * elementsPerVertex;
        GLfloat x = vertices[k];
        glm::vec3 currentColor = x > 0 ? darkColor : color;
        colors[k] = currentColor[0];
        colors[k + 1] = currentColor[1];
        colors[k + 2] = currentColor[1];
    }
    
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);
    
    shader.attribute3fv("position", vertices, 8 * 3);
    shader.attribute3fv("color", colors, 8 * 3);
    
    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 3, indices, GL_STATIC_DRAW);
    
    return vertexArrayHandle;
}

void drawTerrain() {
    std::stack<glm::mat4> ms;
    ms.push(glm::mat4(1.0));

    ms.push(ms.top()); //Terrain
        ms.top() = glm::rotate(ms.top(), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        ms.top() = glm::translate(ms.top(), glm::vec3(0.0, 0.0, -10.0));
        shader.uniformMatrix4fv("modelMatrix", ms.top());
        glBindVertexArray(terrainVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    ms.pop();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char *argv[]) {
    GLFWwindow *win;

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    win = glfwCreateWindow(640, 480, "Earthbending", NULL, NULL);
    
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(win);

    glewExperimental = GL_TRUE;
    GLenum status = glewInit();

    if(status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(status));
    }

    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);

    glfwSetKeyCallback(win, key_callback);

    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(80.0f), 4.0f / 3.0f, 0.1f, 100.f);
    
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0, 6.0, 15.0), //Position
        glm::vec3(0.0, 0.0, 0.0),  //LookAt
        glm::vec3(0.0, 1.0, 0.0)   //Up
    );
    shader.uniformMatrix4fv("projectionMatrix", projection);
    shader.uniformMatrix4fv("viewMatrix", view);

    initTerrain();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawTerrain();
        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);

    return 0;
}
