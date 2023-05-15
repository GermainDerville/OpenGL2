#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
// MON CODE
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/glm.hpp>
#include <vector>

int window_width  = 1280;
int window_height = 720;

static void key_callback(GLFWwindow* /*window*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{
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

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
    Vertex2DColor()
        : position(glm::vec2(0, 0)), color(glm::vec3(0, 0, 0))
    {
    }
    Vertex2DColor(glm::vec2 pPosition, glm::vec3 pCouleur)
        : position(pPosition), color(pCouleur)
    {
    }
};

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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP1", nullptr, nullptr);
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

    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    /*   Mon code   */

    glimac::FilePath applicationPath(argv[0]);
    glimac::Program  program = loadProgram(applicationPath.dirPath() + "TP3/shaders/triangle.vs.glsl", applicationPath.dirPath() + "TP3/shaders/triangle.fs.glsl");
    program.use();

    // Création des points du cercle
    std::vector<Vertex2DColor> sommetsCerlce;
    int N = 12; // Nombre de faces
    float R = 1.f; //Rayon
    sommetsCerlce.push_back(Vertex2DColor(glm::vec2(0, 0), glm::vec3(1, 0, 0))); // Centre
    for (int i = 0; i < N; i++) {
        double theta = 2 * 3.141592*i/N;
        sommetsCerlce.push_back(Vertex2DColor(glm::vec2(R*cos(theta), R*sin(theta)), glm::vec3(0, 0, 1)));
    }
    

    // Création d'un seul VBO:
    GLuint vbo, vao, ibo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Vertex2DColor vertices[] = {
        Vertex2DColor(glm::vec2(0, 0), glm::vec3(1, 0, 0)),
        Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(0, 0, 1)),
        Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),
        Vertex2DColor(glm::vec2(0, 0.5), glm::vec3(0.5, 0.5, 0.5)),};

    glBufferData(GL_ARRAY_BUFFER, (N + 1) * sizeof(Vertex2DColor), sommetsCerlce.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Création IBO
    glGenBuffers(1, &ibo);
    // => On bind sur GL_ELEMENT_ARRAY_BUFFER, cible reservée pour les IBOs
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    std::vector<uint32_t> indices;
    indices.push_back(0);
    for (int i = 0; i < N; i++) {
        indices.push_back(i+1);
    }
    indices.push_back(1); // Le dernier et deuxième point est le même
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (N + 2) * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    

    // Création VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);                // VAO bind

    // => On bind l'IBO sur GL_ELEMENT_ARRAY_BUFFER; puisqu'un VAO est actuellement bindé,
    // cela a pour effet d'enregistrer l'IBO dans le VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    const GLuint VERTEX_ATTR_POSITION = 3; // Activer l'attribut position, c'est à dire l'index 3
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    const GLuint VERTEX_ATTR_COULEUR = 8; // Activer l'attribut couleur, c'est à dire l'index 8
    glEnableVertexAttribArray(VERTEX_ATTR_COULEUR);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);
    glVertexAttribPointer(VERTEX_ATTR_COULEUR, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (const GLvoid*)(offsetof(Vertex2DColor, color)));
    glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO délié
    glBindVertexArray(0);             // VAO déliée

    /*   Fin De Mon Code   */

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 1.f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // MON CODE
        glBindVertexArray(vao);

        // => On utilise glDrawElements à la place de glDrawArrays
        // Cela indique à OpenGL qu'il doit utiliser l'IBO enregistré dans le VAO
        glDrawElements(GL_TRIANGLE_FAN, N + 2, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // FIN MON CODE

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    // MON CODE
    glDeleteBuffers(6, &vbo);
    glDeleteVertexArrays(6, &vao);
    return 0;
}