#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/glm.hpp>
#include <glimac/Cone.hpp>


using namespace glm;

int window_width  = 1280;
int window_height = 720;

float alphaAdd = 5/10000;
bool enable = 0;
bool escapePressed = false;

float L = 15;
float D = 5;
float E = 1;

float alpha = 0;
float betha = 0;

    static void key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
    {
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            alphaAdd += 1/10000;
        }

        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            alphaAdd -= 1/10000;
        }

        if (key == GLFW_KEY_E && action == GLFW_PRESS) {
            enable = !enable;
        }

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            escapePressed = true;
        }
    }

    static void mouse_button_callback(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/)
    {
    }

    static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/)
    {
    }

    static void cursor_position_callback(GLFWwindow* /*window*/, double /*xpos*/, double /*ypos*/)
    {
    }

    static void size_callback(GLFWwindow* /*window*/, int width, int height)
    {
        window_width  = width;
        window_height = height;
    }

    int main(int argc, char** argv)
    {
        /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a window and its OpenGL context */
    #ifdef __APPLE__
        /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Projet", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    glimac::Cone cone(1, 1, 32, 16);

    //VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glimac::ShapeVertex) * cone.getVertexCount(), cone.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //IBO
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    std::vector<uint32_t> indices;
    for (int i = 0; i < cone.getVertexCount() - 1; i++) {
        indices.push_back(i);
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cone.getVertexCount() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 0, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 1, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glimac::FilePath applicationPath(argv[0]);
    glimac::Program  program = glimac::loadProgram(applicationPath.dirPath() + ("Proj/shaders/3D.vs.glsl"),
        applicationPath.dirPath() + ("Proj/shaders/normals.fs.glsl"));
    program.use();
    glGetUniformLocation;
    glEnable(GL_DEPTH_TEST);

    mat4 MVP;
    mat4 BrasMVInit;
    mat4 BrasMV;
    mat4 BrasNorm;
    mat4 RailMV;
    mat4 RailNorm;
    mat4 ArbreMVInit;
    mat4 ArbreMV;
    mat4 ArbreNorm;

    MVP = perspective(radians(70.f), 1.5f, 0.1f, 100.f);

    BrasMVInit = scale(BrasMVInit, vec3(E, 2 * D + E, E));
    BrasMVInit = translate(BrasMVInit, vec3(-(L + D) / 2, 0, -10.0f));
    BrasMVInit = rotate(BrasMVInit, 3.1415f, vec3(0, 0, 1));
    BrasNorm = transpose(inverse(BrasMVInit));

    RailMV = scale(RailMV, vec3(2 * E * E, L + D + 2 * E, E / 4));
    RailMV = rotate(RailMV, -3.1415f, vec3(0, 0, 1));
    RailMV = translate(RailMV, vec3(0, 0, -10.0f));
    RailNorm = transpose(inverse(RailMV));

    ArbreMVInit = scale(ArbreMVInit, vec3(E, L, E));
    ArbreMVInit = translate(ArbreMVInit, vec3(D / 2, 0, -10.0f));
    ArbreMVInit = rotate(ArbreMVInit, -3.1415f, vec3(0, 0, 1));
    ArbreNorm = transpose(inverse(ArbreMVInit));

    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        if (escapePressed) {
            //GLFWwindowclosefun(window);
        }
        glClearColor(1.f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        alpha += enable * alphaAdd;
        betha  = asin(-sin(alpha)*D/L);
        BrasMV = rotate(BrasMVInit, alpha, vec3(0, 0, 1));

        glUniformMatrix4fv(0, 1, false, value_ptr(MVP * BrasMV));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, cone.getVertexCount());
        glBindVertexArray(0);

        glUniformMatrix4fv(0, 1, false, value_ptr(MVP * RailMV));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, cone.getVertexCount());
        glBindVertexArray(0);

        ArbreMV = translate(ArbreMVInit, vec3(0, cos(alpha)*2*D, 0));
        ArbreMV = rotate(ArbreMV, betha - alpha, vec3(0, 0, 1));

        glUniformMatrix4fv(0, 1, false, value_ptr(MVP * ArbreMV));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, cone.getVertexCount());
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}