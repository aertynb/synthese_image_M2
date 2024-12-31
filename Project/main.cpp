#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/glm.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/getTime.hpp>
#include <glimac/Sphere.hpp>
#include <memory>

#include "include/SphereCustom.hpp"
#include "include/QuadCustom.hpp"
#include "include/FreeflyCamera.hpp"

#include "pointer.hpp"

int window_width  = 800;
int window_height = 800;

float last_xpos = 400;
float last_ypos = 400;
bool first_mouse = true;

glm::FreeflyCamera camera {};

struct EarthProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;
    GLint uKd;
    GLint uKs;
    GLint uShininess;
    GLint uLightDir_vs;
    GLint uLightIntensity;

    EarthProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "TP5/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "TP5/shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

struct MoonProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;
    GLint uKd;
    GLint uKs;
    GLint uShininess;
    GLint uLightDir_vs;
    GLint uLightIntensity;

    MoonProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "TP5/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "TP5/shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_W : camera.moveUp(1.); break;
            case GLFW_KEY_A : camera.moveLeft(1.); break;
            case GLFW_KEY_S : camera.moveUp(-1.); break;
            case GLFW_KEY_D : camera.moveLeft(-1.); break;
            case GLFW_KEY_ESCAPE : glfwSetWindowShouldClose(window, true); break;
        }
    }
}

static void mouse_button_callback(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/)
{
}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/)
{
}

static void cursor_position_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
    if (first_mouse) {
        last_xpos = xpos;
        last_ypos = ypos;
        first_mouse = false;
    }

    camera.rotateLeft(xpos - last_xpos);
    camera.rotateUp(last_ypos - ypos);

    last_xpos = xpos;
    last_ypos = ypos;
}

static void size_callback(GLFWwindow* /*window*/, int width, int height)
{
    window_width  = width;
    window_height = height;
}

GLuint gen_tex_gluint(std::unique_ptr<glimac::Image>& texture_ptr) {
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_ptr->getWidth(), texture_ptr->getHeight(), 0, GL_RGBA, GL_FLOAT, texture_ptr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
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
    glimac::FilePath applicationPath(argv[0]);
    EarthProgram earthProgram (applicationPath);
    MoonProgram moonProgram (applicationPath);

    auto earth_tex_ptr = glimac::loadImage(applicationPath.dirPath() + "assets/texture/EarthMap.jpg");
    auto moon_tex_ptr = glimac::loadImage(applicationPath.dirPath() + "assets/texture/MoonMap.jpg");
    auto cloud_tex_ptr = glimac::loadImage(applicationPath.dirPath() + "assets/texture/CloudMap.jpg");

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowSizeCallback(window, &size_callback);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    glimac::SphereCustom sphere(1, 32, 16);
    glimac::QuadCustom quad(1, 1);

    std::vector<std::unique_ptr<Object>> objs;
    objs.emplace_back(std::make_unique<glimac::SphereCustom>(1, 32, 16));// objs.emplace_back(std::make_unique<glimac::QuadCustom>(1, 1));


    //GLuint vbo;
    //glGenBuffers(1, &vbo);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //auto vertices = sphere.getDataPointer();
    //glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount()*sphere.getVertexSize(), vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, quad.getVertexCount()*quad.getVertexSize(), quad.getDataPointer(), GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //GLuint vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //const GLuint VERTEX_ATTR_POSITION = 1;
    //const GLuint VERTEX_ATTR_NORMAL = 2;
    //const GLuint VERTEX_ATTR_TEX = 3;

    //glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    //glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    //glEnableVertexAttribArray(VERTEX_ATTR_TEX);

    //sphere.initVaoPointer(VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEX);
    //quad.initVaoPointer(VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEX);

    std::vector<GLuint> vbos(objs.size()); // Initialize with the correct size
    std::vector<GLuint> vaos(objs.size()); // Initialize with the correct size 
    const GLuint VERTEX_ATTR_POSITION = 1;
    const GLuint VERTEX_ATTR_NORMAL = 2;
    const GLuint VERTEX_ATTR_TEX = 3;

    glGenBuffers(objs.size(), vbos.data());
    glGenVertexArrays(objs.size(), vaos.data());

    for (size_t idx = 0; idx < objs.size(); ++idx) {
        glBindVertexArray(vaos[idx]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[idx]);

        const auto &obj = objs[idx];
        glBufferData(GL_ARRAY_BUFFER, obj->getVertexCount() * obj->getVertexSize(), obj->getDataPointer(), GL_STATIC_DRAW);

        obj->initVaoPointer(VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEX);
    }

    // Unbind after setup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::vector<glm::vec3> moons;

    GLuint earth_tex = gen_tex_gluint(earth_tex_ptr);
    GLuint moon_tex = gen_tex_gluint(moon_tex_ptr);
    GLuint cloud_tex = gen_tex_gluint(cloud_tex_ptr);

    for (auto i = 0; i < 32; i++) {
        moons.push_back(glm::sphericalRand(2.));
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        auto mv_matrix = camera.getViewMatrix();
        auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
        auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*********************************
        * HERE SHOULD COME THE RENDERING CODE
        *********************************/
        //glBindVertexArray(vao);
        for (size_t idx = 0; idx < objs.size(); ++idx) {
            glBindVertexArray(vaos[idx]);

            // For Earth
            if (idx == 0) {
                earthProgram.m_Program.use();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, earth_tex);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, cloud_tex);
                glUniform1i(earthProgram.uEarthTexture, 0);
                glUniform1i(earthProgram.uCloudTexture, 1);

                glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
                glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
                glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

                glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

                moonProgram.m_Program.use();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, moon_tex);

                for (const auto &moon_pos : moons) {
                    auto a_mv_matrix = glm::rotate(mv_matrix, glimac::getTime(), glm::vec3(0, 1, 0));
                    a_mv_matrix = glm::translate(a_mv_matrix, moon_pos);
                    a_mv_matrix = glm::scale(a_mv_matrix, glm::vec3(0.2f));

                    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
                    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
                }
            }

            // For Moons
            else {
                // TO DO
            }
        }

        // Unbind after rendering
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteTextures(1, &earth_tex);
    glDeleteTextures(1, &moon_tex);
    glDeleteTextures(1, &cloud_tex);
    glDeleteBuffers(objs.size(), vbos.data());
    glDeleteVertexArrays(objs.size(), vaos.data());
    //glDeleteBuffers(objs.size(), vbo.data());
    //glDeleteVertexArrays(objs.size(), vao.data());
    glfwTerminate();
    return 0;
}