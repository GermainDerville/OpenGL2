#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
//Mon code
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/glm.hpp>
#include <vector>
#include <glimac/Image.hpp>

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


glm::mat3 translate(float tx, float ty) {
    return glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1)); // Les matrices s'écrivent colonne par colonne
}
glm::mat3 scale(float sx, float sy) {
    return glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
}
glm::mat3 rotate(float a)   { // En degrés
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP6", nullptr, nullptr);
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

    // Incorpore les shaders
    glimac::FilePath applicationPath(argv[0]);
    glimac::Program  program = loadProgram(applicationPath.dirPath() + "TP6/shaders/tex2D.vs.glsl", applicationPath.dirPath() + "TP6/shaders/tex2D.fs.glsl");
    program.use();

    // Chargement des textures
    std::unique_ptr<glimac::Image>  texture = glimac::loadImage(applicationPath.dirPath() + "assets/textures/triforce.png");
    if (texture == NULL)
        std::cout << "PROBLEM chargement texture" << std::endl;
    GLuint   textureName;
    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);
    // Envoi la texture à la carte graphique
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->getWidth(), texture->getHeight(), 0, GL_RGBA, GL_FLOAT, texture->getPixels()); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

            /***********************************/
            /******  Variables Unifomres  ******/

    // Définir la location de la variable uniforme uModelMatrix 
    GLint   uPosLocation = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    GLint   uColorLocation = glGetUniformLocation(program.getGLId(), "uColor");
    GLint   uTextureLocation = glGetUniformLocation(program.getGLId(), "uTexture");

    //Initialise les var uniformes
    glUniform1i(uTextureLocation, 0);
    //glUniform1f(uTimeLoction, 45.0f);


        /******  VBO  ******/

    // Création d'un seul VBO:
    GLuint vbo, vao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Vertex2DUV vertices[] = {
        Vertex2DUV(-1.0f, -1.0f, 0, 1), // premier sommet
        Vertex2DUV(1.0f, -1.0f, 0.5f, 0),  // deuxième sommet
        Vertex2DUV(0.0f, 1.0f, 1, 1)    // troisième sommet
    };

    
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


        /******  VAO  ******/

    // Création VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);                // VAO bind
    const GLuint VERTEX_ATTR_POSITION = 0; // Activer l'attribut position, c'est à dire l'index 0 à cause du vertex shader
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    const GLuint VERTEX_ATTR_TEXTURE = 1; // Activer l'attribut texture, c'est à dire l'index 1 à cause du vertex shader
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO délié
    glBindVertexArray(0);             // VAO déliée

    /*   Fin De Mon Code   */

    /* Loop until the user closes the window */
    GLfloat i = 0;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // MON CODE
        i += 0.25f;
        glm::mat3 transformed;
        
        glBindVertexArray(vao);

        transformed = rotate(i) * translate(0.5f, 0.5f) * scale(0.25f, 0.25f) * rotate(i);
        glUniform3f(uColorLocation, 1, 0, 0);                                       // Update la couleur (variable uniforme uColorLocation)
        glUniformMatrix3fv(uPosLocation, 1, GL_FALSE, glm::value_ptr(transformed)); // Update la position (variable uniforme uModelMatrix)
        glBindTexture(GL_TEXTURE_2D, textureName); //Bind texture sur la cible GL_TEXTURE_2D
        glUniform1i(uTextureLocation, 0); // Init la texture
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        
        transformed = rotate(i) * translate(-0.5f, 0.5f) * scale(0.25f, 0.25f) * rotate(-2*i);
        glUniform3f(uColorLocation, 0, 1, 0);                                       // Update la couleur (variable uniforme uColorLocation)
        glUniformMatrix3fv(uPosLocation, 1, GL_FALSE, glm::value_ptr(transformed)); // Update la position (variable uniforme uModelMatrix)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        transformed = rotate(i) * translate(0.5f, -0.5f) * scale(0.25f, 0.25f) * rotate(i);
        glUniform3f(uColorLocation, 0, 0, 1);                                       // Update la couleur (variable uniforme uColorLocation)
        glUniformMatrix3fv(uPosLocation, 1, GL_FALSE, glm::value_ptr(transformed)); // Update la position (variable uniforme uModelMatrix)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        transformed = rotate(i) * translate(-0.5f, -0.5f) * scale(0.25f, 0.25f) * rotate(-2*i);
        glUniform3f(uColorLocation, 0.33f, 0.33f, 0.33f);                           // Update la couleur (variable uniforme uColorLocation)
        glUniformMatrix3fv(uPosLocation, 1, GL_FALSE, glm::value_ptr(transformed)); // Update la position (variable uniforme uModelMatrix)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindTexture(GL_TEXTURE_2D, 0); // Débind texture
        glBindVertexArray(0);
        // FIN MON CODE

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    // MON CODE
    glDeleteTextures(1, &textureName);
    glDeleteBuffers(6, &vbo);
    glDeleteVertexArrays(6, &vao);
    return 0;
}