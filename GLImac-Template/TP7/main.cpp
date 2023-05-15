#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
// Mon code
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
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

/*  Mon code : struct et fonctions utilitaires  */

struct Vertex2DUV {
    glm::vec2 position;
    glm::vec2 texture;

    Vertex2DUV()
        : position(glm::vec2(0, 0)), texture(glm::vec2(0, 0))
    {
    }
    Vertex2DUV(glm::vec2 pPosition, glm::vec2 pTexture)
        : position(pPosition), texture(pTexture)
    {
    }
    Vertex2DUV(GLfloat x, GLfloat y, GLfloat u, GLfloat v)
        : position(glm::vec2(x, y)), texture(glm::vec2(u, v))
    {
    }
};

glm::mat3 translate(float tx, float ty)
{
    return glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1)); // Les matrices s'écrivent colonne par colonne
}
glm::mat3 scale(float sx, float sy)
{
    return glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
}
glm::mat3 rotate(float a)
{ // En degrés
    a = glm::radians(a);
    return glm::mat3(glm::vec3(cos(a), sin(a), 0), glm::vec3(-sin(a), cos(a), 0), glm::vec3(0, 0, 1));
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP7", nullptr, nullptr);
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


     glimac::Sphere Sphere1 = glimac::Sphere::Sphere(1, 32, 16);


    // Incorpore les shaders
    glimac::FilePath applicationPath(argv[0]);
    glimac::Program  program = loadProgram(applicationPath.dirPath() + "TP7/shaders/3D.vs.glsl", applicationPath.dirPath() + "TP7/shaders/normals.fs.glsl");
    program.use();
    
    // Chargement des textures
    /*
    std::unique_ptr<glimac::Image> texture = glimac::loadImage(applicationPath.dirPath() + "assets/textures/triforce.png");
    if (texture == NULL)
        std::cout << "PROBLEM chargement texture" << std::endl;
    GLuint textureName;
    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);
    // Envoi la texture à la carte graphique
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->getWidth(), texture->getHeight(), 0, GL_RGBA, GL_FLOAT, texture->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    */

    /***********************************/
    /******  Variables Unifomres  ******/

    // Définir la location de la variable uniforme uModelMatrix
    GLint uMVPMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uMVMatrixLocation  = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint uNormalMatrixLocation = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
    
    // Autorise le GPU à tester la profondeur permettant de n'afficher que les objets devant
    glEnable(GL_DEPTH_TEST);

    // Initialise les var uniformes
    //glUniform1i(uTextureLocation, 0);
    // glUniform1f(uTimeLoction, 45.0f);

    // Calcule des variables uniformes
    glm::mat4 ProjMatrix     = glm::perspective(glm::radians(70.f), (float)window_width / window_height, 0.1f, 100.f);
    glm::mat4 MVMatrix       = glm::translate(glm::mat4(), glm::vec3(0, 0, -0.5f)); // translation selon glm::vec3(x, y, z)
    glm::mat4 NormalMatrix   = glm::transpose(glm::inverse(MVMatrix));

    /******  VBO  ******/

    // Création d'un seul VBO:
    GLuint vbo, vao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    const glimac::ShapeVertex* vertices = Sphere1.getDataPointer(); // Les sommets d'une sphère
     

    glBufferData(GL_ARRAY_BUFFER, sizeof(glimac::ShapeVertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /******  VAO  ******/

    // Création VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);                // VAO bind
    const GLuint VERTEX_ATTR_POSITION = 0; // Activer l'attribut position, c'est à dire l'index 0 à cause du vertex shader
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    const GLuint VERTEX_ATTR_NORMALE = 1; // Activer l'attribut normale, c'est à dire l'index 1 à cause du vertex shader
    glEnableVertexAttribArray(VERTEX_ATTR_NORMALE);
    const GLuint VERTEX_ATTR_TEXTURE_COORD = 2; // Activer l'attribut texture coordinate, c'est à dire l'index 2 à cause du vertex shader
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE_COORD);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION,      3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), 0);
    glVertexAttribPointer(VERTEX_ATTR_NORMALE,       3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(sizeof(glm::vec3)));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(2 * sizeof(glm::vec3)));

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO délié
    glBindVertexArray(0);             // VAO déliée

    /*   Fin De Mon Code   */

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // MON CODE

        glBindVertexArray(vao);

        glm::mat4 MVPMat = ProjMatrix * MVMatrix;
        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMat)); // Update la variable uniforme uMVPMatrix
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));   // Update la variable uniforme uMVMatrix
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix)); // Update la variable uniforme uNormalMatrix

        glDrawArrays(GL_TRIANGLES, 0, Sphere1.getVertexCount());

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