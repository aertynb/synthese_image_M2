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
#include "include/CubeCustom.hpp"

#include "pointer.hpp"

int window_width  = 800;
int window_height = 800;

float last_xpos = 400;
float last_ypos = 400;
bool first_mouse = true;

glm::FreeflyCamera camera {};

struct MultiTextureProgram {
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

    MultiTextureProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "Project/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "Project/shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

struct BlinnPhongProgram {
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

    BlinnPhongProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "Project/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "Project/shaders/directionallight.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
        uKd = glGetUniformLocation(m_Program.getGLId(), "uKd");
        uKs = glGetUniformLocation(m_Program.getGLId(), "uKs");
        uShininess = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        uLightDir_vs = glGetUniformLocation(m_Program.getGLId(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};

struct PointLightProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;
    GLint uKd;
    GLint uKs;
    GLint uShininess;
    GLint uLightPos_vs;
    GLint uLightIntensity;

    PointLightProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "Project/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "Project/shaders/pointlight.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
        uKd = glGetUniformLocation(m_Program.getGLId(), "uKd");
        uKs = glGetUniformLocation(m_Program.getGLId(), "uKs");
        uShininess = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        uLightPos_vs = glGetUniformLocation(m_Program.getGLId(), "uLightPos_vs");
        uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};

struct SimpleTextureProgram {
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

    SimpleTextureProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "Project/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "Project/shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};

struct TransparencyProgram {
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

    TransparencyProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "Project/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "Project/shaders/transparency.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};

struct Planet{
    glm::vec3 position;
    GLuint texture;
    float scale;
    float speed;
    float rotationSpeed;
};

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action == GLFW_PRESS) {
        switch(key) {
            //case GLFW_KEY_W : camera.moveUp(1.); break;
            //case GLFW_KEY_A : camera.moveLeft(1.); break;
            //case GLFW_KEY_S : camera.moveUp(-1.); break;
            //case GLFW_KEY_D : camera.moveLeft(-1.); break;
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

void drawEarthMoon(const PointLightProgram& program, const SimpleTextureProgram& moonProgram, glm::vec3 origin, int objIdx, 
                                 GLuint earth_tex, GLuint cloud_tex, GLuint moon_tex, const std::vector<std::unique_ptr<Object>>& objs,
                                 const std::vector<glm::vec3>& moons) {

    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto mv_matrix = camera.getViewMatrix() * model_matrix;
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));

    // Calcul de la direction de la lumière en fonction du temps
    //glm::vec3 light_dir_world = glm::rotate(glm::mat4(1.f), glimac::getTime(), glm::vec3(0, 1, 0)) * glm::vec4(1, 1, 1, 0);
    //glm::vec3 light_dir_vs = glm::vec3(mv_matrix * glm::vec4(light_dir_world, 0.0));

    glm::vec3 lightPos_world (1, 1, 1);
    glm::vec3 lightPos_vs = glm::vec3(glm::scale( glm::translate(model_matrix, glm::vec3(5)), glm::vec3(5) ) * glm::vec4(lightPos_world, 0.0));

    // Calcul de la position de la lumière en fonction du temps

    program.m_Program.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earth_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cloud_tex);

    glUniform1i(program.uEarthTexture, 0);
    glUniform1i(program.uCloudTexture, 1);

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    // Envoi des paramètres de lumière
    //glUniform3fv(program.uLightDir_vs, 1, glm::value_ptr(light_dir_vs));
    glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
    glUniform3f(program.uLightIntensity, 100.0f, 100.0f, 100.0f);

    // Envoi des coefficients de matériaux pour la Terre
    glUniform3f(program.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(program.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(program.uShininess, 32.0f);

    glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());

    moonProgram.m_Program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moon_tex);

    for (const auto &moon_pos : moons) {
        auto a_mv_matrix = glm::rotate(mv_matrix, glimac::getTime(), glm::vec3(0, 1, 0));
        a_mv_matrix = glm::translate(a_mv_matrix, moon_pos);
        a_mv_matrix = glm::scale(a_mv_matrix, glm::vec3(0.2f));

        glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
        glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());
    }
}

void drawPlanetarySystem(const PointLightProgram& program, const SimpleTextureProgram& moonProgram, glm::vec3 origin, int objIdx, 
                   GLuint sunTexture, const std::vector<Planet>& planets, const std::vector<std::unique_ptr<Object>>& objs) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, glimac::getTime(), glm::vec3(0, 1, 0));
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));

    program.m_Program.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTexture);

    glUniform1i(program.uEarthTexture, 0);
    glUniform1i(program.uCloudTexture, 1);

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glm::vec3 lightPos_world (0, 0, 0);
    glm::vec3 lightPos_vs = glm::vec3(glm::scale( model_matrix, glm::vec3(1) ) * glm::vec4(lightPos_world, 0.0));

    glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
    glUniform3f(program.uLightIntensity, 20.0f, 20.0f, 20.0f);

    glUniform3f(program.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(program.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(program.uShininess, 32.0f);

    glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());

    for (const auto& planet : planets) {
        auto a_mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, glimac::getTime()*planet.rotationSpeed, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::translate(a_mv_matrix, planet.position);
        a_mv_matrix = glm::rotate(a_mv_matrix, glimac::getTime()*planet.speed, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::scale(glm::scale(a_mv_matrix, glm::vec3(0.2f)), glm::vec3(planet.scale));

        glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));

        glBindTexture(GL_TEXTURE_2D, planet.texture);
        lightPos_world = -planet.position;
        lightPos_vs = glm::vec3(glm::scale( glm::rotate( model_matrix, glimac::getTime() * planet.speed, glm::vec3(0, -1, 0)), glm::vec3(5) ) * glm::vec4(lightPos_world, 0.0));
        glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
        glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());
    }
}

void drawRoom(const SimpleTextureProgram& moonProgram, const std::vector<std::unique_ptr<Object>>& objs, float rotateValue, glm::vec3 origin, int idx, 
                GLuint floor_tex, GLuint wall_brick_tex) {
    auto mv_matrix = glm::rotate( glm::translate( camera.getViewMatrix(), origin ), glm::radians(rotateValue), glm::vec3(0, 1, 0) );
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    glm::vec3 scale(24, 15, 20);

    moonProgram.m_Program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floor_tex);
    
    //floor
    auto a_mv_matrix = glm::scale(glm::translate(mv_matrix, glm::vec3(0, -2, 0)), scale);
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
    glBindTexture(GL_TEXTURE_2D, wall_brick_tex);

    //wall1
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(0, -2, scale.z/2));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    //wall2
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(scale.x/2, -2, 0));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    //wall3
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(-scale.x/2, -2, 0));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    //wall4 1/2
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(-scale.x/2 - 2, -2, -scale.z/2));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    //wall4 2/2
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(scale.x/2 + 2, -2, -scale.z/2));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
}

void drawCorridor(const SimpleTextureProgram& moonProgram, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, int idx, 
                GLuint floor_tex, GLuint wall_brick_tex) {
    auto mv_matrix =  glm::translate( camera.getViewMatrix(), origin );
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    glm::vec3 scale(4, 15, 2);

    moonProgram.m_Program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floor_tex);
    
    //floor
    auto a_mv_matrix = glm::scale(glm::translate(mv_matrix, glm::vec3(0, -2, 0)), scale);
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    glBindTexture(GL_TEXTURE_2D, wall_brick_tex);

    //wall2
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(scale.x/2, -2, 0));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    //wall3
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(-scale.x/2, -2, 0));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
}

void draw_skybox(const SimpleTextureProgram& moonProgram, const std::vector<std::unique_ptr<Object>>& objs, int idx, 
                GLuint sky_tex) {
    auto mv_matrix = camera.getViewMatrix();
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 300.f);
    glm::vec3 scale(24, 1, 20);

    moonProgram.m_Program.use();
    glActiveTexture(GL_TEXTURE0);

    mv_matrix =  glm::scale( glm::rotate( glm::translate(mv_matrix, glm::vec3(0, 0, 0)), glm::radians(180.0f), glm::vec3(1, 1, 0)), glm::vec3(200));
    glBindTexture(GL_TEXTURE_2D, sky_tex);

    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
}

void drawTransparent(const TransparencyProgram& program, glm::vec3 origin, int objIdx, 
                    GLuint texture, const std::vector<std::unique_ptr<Object>>& objs) {
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDepthMask(GL_FALSE);

    program.m_Program.use();

    auto model_matrix = glm::rotate( glm::translate(glm::mat4(1.0f), origin), glm::radians(90.0f), glm::vec3(1, 0, 0));
    auto mv_matrix = camera.getViewMatrix() * model_matrix;
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glUniform1i(program.uTexture, 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());
    
    glDepthMask(GL_TRUE);
    
    glDisable(GL_BLEND);
}

void process_continuous_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.tryMoveUp(0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.tryMoveUp(-0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.tryMoveLeft(0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.tryMoveLeft(-0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLuint getTexture(const std::string& path) {
    auto ptr = glimac::loadImage(path);
    if (ptr == NULL) {
        std::cout << "Erreur chargement de texture : " << path << std::endl;
        return -1;
    }
    return gen_tex_gluint(ptr);
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
    MultiTextureProgram mtProgram (applicationPath);
    BlinnPhongProgram bpProgram (applicationPath);
    PointLightProgram plProgram (applicationPath);
    SimpleTextureProgram moonProgram (applicationPath);
    TransparencyProgram tProgram (applicationPath);

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
    glimac::CubeCustom cube(1, 1, 1);

    std::vector<std::unique_ptr<Object>> objs;
    objs.emplace_back(std::make_unique<glimac::SphereCustom>(1, 32, 16)); 
    objs.emplace_back(std::make_unique<glimac::QuadCustom>(1, 1));
    objs.emplace_back(std::make_unique<glimac::CubeCustom>(cube));

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

    GLuint earth_tex = getTexture(applicationPath.dirPath() + "assets/texture/EarthMap.jpg");
    GLuint moon_tex = getTexture(applicationPath.dirPath() + "assets/texture/MoonMap.jpg");
    GLuint cloud_tex = getTexture(applicationPath.dirPath() + "assets/texture/CloudMap.jpg");
    GLuint wall_brick_tex = getTexture(applicationPath.dirPath() + "assets/texture/wall_brick.jpg");
    GLuint floor_tex = getTexture(applicationPath.dirPath() + "assets/texture/floor.png");
    GLuint sky_tex = getTexture(applicationPath.dirPath() + "assets/texture/sky.png");
    GLuint sun_tex = getTexture(applicationPath.dirPath() + "assets/texture/sun.png");
    GLuint space_tex = getTexture(applicationPath.dirPath() + "assets/texture/SpaceMap.jpg");
    GLuint mars_tex = getTexture(applicationPath.dirPath() + "assets/texture/MarsMap.jpg");
    GLuint mercury_tex = getTexture(applicationPath.dirPath() + "assets/texture/MercuryMap.jpg");
    GLuint venus_tex = getTexture(applicationPath.dirPath() + "assets/texture/VenusMap.jpg");
    GLuint jupiter_tex = getTexture(applicationPath.dirPath() + "assets/texture/JupiterMap.jpg");
    GLuint saturn_tex = getTexture(applicationPath.dirPath() + "assets/texture/SaturnMap.jpg");
    GLuint uranus_tex = getTexture(applicationPath.dirPath() + "assets/texture/UranusMap.jpg");
    GLuint neptune_tex = getTexture(applicationPath.dirPath() + "assets/texture/NeptuneMap.jpg");

    for (auto i = 0; i < 32; i++) {
        moons.push_back(glm::sphericalRand(2.));
    }

    //std::vector<GLuint> planetTextures {earth_tex, mars_tex};
    std::vector<Planet> planets { {{0, 0, -1.5}, mercury_tex, 0.3f, 2.f, 3.f}, {{0, 0, -2}, venus_tex, 0.4f, 1.5f, 2.5f},
                                {{0, 0, -3}, earth_tex, 1.f, 1.f, 1.f}, {{0.5, 0, -4}, mars_tex, 0.8f, 1.2f, 1.2f},
                                {{0, 0, -6}, jupiter_tex, 2.5f, 0.5f, 0.8f}, {{1, 0, -7}, saturn_tex, 2.f, 0.8f, 0.8f},
                                {{0, 0, -8}, uranus_tex, 1.2f, 0.6f, 0.9f}, {{0, 0, -9}, neptune_tex, 0.4, 0.8f, 1.f}};

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        process_continuous_input(window);
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
                drawPlanetarySystem(plProgram, moonProgram, glm::vec3(0, 3, 11), idx, sun_tex, planets, objs);
                draw_skybox(moonProgram, objs, idx, space_tex);
            }

            if (idx == 1) {
                drawRoom(moonProgram, objs, 0.0f, glm::vec3(0, 0, 11), idx, floor_tex, wall_brick_tex);
                drawRoom(moonProgram, objs, 180.0f, glm::vec3(0, 0, -11), idx, floor_tex, wall_brick_tex);
                drawCorridor(moonProgram, objs, glm::vec3(0, 0, 0), idx, floor_tex, wall_brick_tex);
            }

            if (idx == 2) {
                drawTransparent(tProgram, glm::vec3(0, 0, -11), idx, earth_tex, objs);
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
    glfwTerminate();
    return 0;
}