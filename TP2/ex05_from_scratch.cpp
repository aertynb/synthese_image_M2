#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/glm.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>

int window_width  = 800;
int window_height = 800;

glm::mat3 translate(float tx, float ty) {
  return glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(tx,ty,1));
};

glm::mat3 scale(float sx, float sy) {
  return glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0,sy, 0), glm::vec3(0,0,1));
};

glm::mat3 rotate(float a) {
  return glm::mat3(glm::vec3(cos(glm::radians(a)), sin(glm::radians(a)), 0), glm::vec3(-sin(glm::radians(a)), cos(glm::radians(a)), 0), glm::vec3(0, 0, 1));
};

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

struct Vertex2DUV {
public:
    Vertex2DUV() {};
    Vertex2DUV(glm::vec2 pos, glm::vec2 tex) : _pos {std::move(pos)}, _tex {std::move(tex)} {};
    glm::vec2 _pos;
    glm::vec2 _tex;
};

int main(int argc, char *argv[])
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

    auto vertex_shader = argv[1];
    auto fragment_shader = argv[2];
    glimac::FilePath applicationPath(argv[0]);
    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "TP2/shaders/" + vertex_shader,
                                          applicationPath.dirPath() + "TP2/shaders/" + fragment_shader);
    program.use();

    //GLuint loc = glGetUniformLocation(program.getGLId(), "uTime");
    GLuint loc = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Vertex2DUV vertices[] = { Vertex2DUV(glm::vec2(-1., -1.), glm::vec2(0., 0.)),
                              Vertex2DUV(glm::vec2(1., -1.), glm::vec2(0., 0.)),
                              Vertex2DUV(glm::vec2(0., 1.), glm::vec2(0., 0.)),
    };
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_COLOR = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), offsetof(Vertex2DUV, _pos));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (GLvoid*)offsetof(Vertex2DUV, _tex));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glUniform1f(loc, 45.);
    float a = 0;
    float b = 0;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        /*********************************
        * HERE SHOULD COME THE RENDERING CODE
        *********************************/
        
        //glUniform1f(loc, rotate);
        glBindVertexArray(vao);
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(rotate(a+45) * translate(0.5, 0.5) * rotate(b) * scale(0.25, 0.25)));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(rotate(a+45+90) * translate(0.5, 0.5) * rotate(b) * scale(0.25, 0.25)));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(rotate(a+45+90+90) * translate(0.5, 0.5) * rotate(b) * scale(0.25, 0.25)));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(rotate(a+45+90+90+90) * translate(0.5, 0.5) * rotate(b) * scale(0.25, 0.25)));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        a += 1;
        b += 1;
        glBindVertexArray(0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}