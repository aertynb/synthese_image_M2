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
#include "include/DrawingUtils.hpp"

#include "pointer.hpp"

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
    SimpleTextureProgram stProgram (applicationPath);
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
                                {{0, 0, -6}, jupiter_tex, 2.5f, 0.3f, 0.8f}, {{1, 0, -7}, saturn_tex, 2.f, 0.6f, 0.8f},
                                {{0, 0, -8}, uranus_tex, 1.2f, 0.6f, 0.9f}, {{0, 0, -9}, neptune_tex, 0.4, 0.8f, 1.f}};

    glm::vec3 sunPos (0, 0, 8);

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
                switch(camera.isInFirstRoom()) {
                    case true : 
                        drawPlanetarySystem(plProgram, stProgram, sunPos, idx, sun_tex, planets, objs);
                        draw_skybox(stProgram, objs, idx, space_tex);
                        break;
                    case false :
                        drawPlanetarySystem(plProgram, stProgram, sunPos, idx, moon_tex, planets, objs);
                        draw_skybox(stProgram, objs, idx, sky_tex);
                        break;
                }
                
            }

            if (idx == 1) {
                drawRoom(plProgram, objs, 0.0f, glm::vec3(0, 0, 11), sunPos, idx, floor_tex, space_tex);
                drawRoom(plProgram, objs, 180.0f, glm::vec3(0, 0, -11), sunPos, idx, floor_tex, space_tex);
                drawCorridor(stProgram, objs, glm::vec3(0, 0, 0), idx, floor_tex, wall_brick_tex);
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