#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Sphere.hpp>
#include <glimac/glm.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/getTime.hpp>

int window_width  = 800;
int window_height = 800;

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
    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "TP3/shaders/" + vertex_shader,
                                          applicationPath.dirPath() + "TP3/shaders/" + fragment_shader);
    program.use();

    auto mvp_loc = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    auto mv_loc = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    auto normal_loc = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
    auto tex_loc = glGetUniformLocation(program.getGLId(), "uTexture");

    std::cout << tex_loc << std::endl;

    auto earth_tex_ptr = glimac::loadImage("/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/assets/texture/EarthMap.jpg");
    auto moon_tex_ptr = glimac::loadImage("/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/assets/texture/MoonMap.jpg");
    if (earth_tex_ptr == NULL || moon_tex_ptr == NULL) {
        std::cout << "Erreur chargement de texture terre" << std::endl;
        return -1;
    } 

    glEnable(GL_DEPTH_TEST);
    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    glimac::Sphere sphere(1, 32, 16);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    auto vertices = sphere.getDataPointer();
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount()*sizeof(glimac::ShapeVertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const GLuint VERTEX_ATTR_POSITION = 1;
    const GLuint VERTEX_ATTR_NORMAL = 2;
    const GLuint VERTEX_ATTR_TEX = 3;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEX);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEX, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint earth_tex;
    glGenTextures(1, &earth_tex);
    glBindTexture(GL_TEXTURE_2D, earth_tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earth_tex_ptr->getWidth(), earth_tex_ptr->getHeight(), 0, GL_RGBA, GL_FLOAT, earth_tex_ptr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint moon_tex;
    glGenTextures(1, &moon_tex);
    glBindTexture(GL_TEXTURE_2D, moon_tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moon_tex_ptr->getWidth(), moon_tex_ptr->getHeight(), 0, GL_RGBA, GL_FLOAT, moon_tex_ptr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    
    auto mv_matrix = glm::translate(glm::mat4(1), glm::vec3(0., 0., -5.));
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));
    std::vector<glm::vec3> vectors;

    for (auto i = 0; i < 32; i++) {
        vectors.push_back(glm::sphericalRand(2.));
    }
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*********************************
        * HERE SHOULD COME THE RENDERING CODE
        *********************************/
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, earth_tex); // earth
        glUniform1i(tex_loc, 0);

        // rotation terre
        auto e_mv_matrix = glm::rotate(mv_matrix, glimac::getTime(), glm::vec3(0, -1, 0));
        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix * e_mv_matrix)); // mvp matrix with rotation
        // sending matrix
        //glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
        glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));
        glUniformMatrix4fv(normal_loc, 1, GL_FALSE, glm::value_ptr(normal_matrix));

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        
        glBindTexture(GL_TEXTURE_2D, moon_tex); // moon tex

        for (auto i = 0; i < 32; i++) {
            auto a_mv_matrix = glm::rotate(mv_matrix, glimac::getTime(), glm::vec3(0, 1, 0));
            a_mv_matrix = glm::translate(a_mv_matrix, vectors[i]); // Translation * Rotation * Translation
            a_mv_matrix = glm::scale(a_mv_matrix, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteTextures(1, &earth_tex);
    glDeleteTextures(1, &moon_tex);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();
    return 0;
}