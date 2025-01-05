#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/glm.hpp>
#include "Programs.hpp"
#include "FreeflyCamera.hpp"
#include "Planet.hpp"
#include "Object.hpp"

int window_width  = 800;
int window_height = 800;

float last_xpos = 400;
float last_ypos = 400;
bool first_mouse = true;

glm::FreeflyCamera camera {};

void drawEarthMoon(const PointLightProgram& program, const SimpleTextureProgram& moonProgram, const std::vector<std::unique_ptr<Object>>& objs, 
                   const std::vector<glm::vec3>& moons, glm::vec3 origin, int objIdx, GLuint earth_tex, GLuint cloud_tex, GLuint moon_tex) {

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

    glUniform1i(program.uTexture, 0);

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

    glUniform1i(program.uTexture, 0);

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glm::vec3 lightPos_world (0, 0, 0);
    glm::vec3 lightPos_vs = glm::vec3(glm::scale( model_matrix, glm::vec3(1) ) * glm::vec4(lightPos_world, 0.0));

    glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
    glUniform3f(program.uLightIntensity, 30.0f, 30.0f, 30.0f);

    glUniform3f(program.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(program.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(program.uShininess, 32.0f);

    glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());

    for (const auto& planet : planets) {
        auto a_mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, glimac::getTime()*planet.speed, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::translate(a_mv_matrix, planet.position);
        a_mv_matrix = glm::rotate(a_mv_matrix, glimac::getTime()*planet.rotationSpeed, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::scale(glm::scale(a_mv_matrix, glm::vec3(0.2f)), glm::vec3(planet.scale));

        glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));

        glBindTexture(GL_TEXTURE_2D, planet.texture);
        lightPos_world = -planet.position;
        lightPos_vs = glm::vec3(glm::scale( glm::rotate( model_matrix, glimac::getTime() * planet.rotationSpeed, glm::vec3(0, -1, 0)), glm::vec3(5) ) * glm::vec4(lightPos_world, 0.0));
        glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
        glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());
    }
}

void drawQuad(const PointLightProgram& program, GLsizei vertexCount, glm::mat4 mv_matrix, glm::mat4 proj_matrix,
              glm::mat4 model_matrix, glm::vec3 origin, glm::vec3 lightPos, glm::vec3 scale, float rotation, bool xAxis) {
    if (xAxis) {
        auto a_mv_matrix = glm::translate(mv_matrix, origin);
        a_mv_matrix = glm::scale(a_mv_matrix, scale);
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(1, 0, 0));
        auto a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

        lightPos = lightPos - origin;

        glm::vec3 lightPos_vs = glm::vec3(glm::scale( model_matrix, glm::vec3(1) ) * glm::vec4(lightPos, 0.0));

        glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));

        glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
        glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
        glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    } else {
        auto a_mv_matrix = glm::translate(mv_matrix, origin);
        a_mv_matrix = glm::scale(a_mv_matrix, scale);
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(1, 0, 0));
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(0, 0, 1));
        auto a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

        lightPos = lightPos - origin;

        glm::vec3 lightPos_vs = glm::vec3(glm::scale( model_matrix, glm::vec3(1) ) * glm::vec4(lightPos, 0.0));

        glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));

        glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
        glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
        glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

}

void drawRoom(const PointLightProgram& program, const std::vector<std::unique_ptr<Object>>& objs, float rotateValue, glm::vec3 origin,
              glm::vec3 sunPos, int idx, GLuint floorTexture, GLuint wallTexture) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, glm::radians(rotateValue), glm::vec3(0, 1, 0) );
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    glm::vec3 scale(24, 15, 20);

    program.m_Program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    sunPos = sunPos - origin;

    glm::vec3 lightPos_vs = glm::vec3(glm::scale( model_matrix, glm::vec3(1) ) * glm::vec4(sunPos, 0.0));

    glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
    glUniform3f(program.uLightIntensity, 10.0f, 10.0f, 10.0f);

    glUniform3f(program.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(program.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(program.uShininess, 32.0f);
    
    //floor
    /*auto a_mv_matrix = glm::scale(glm::translate(mv_matrix, glm::vec3(0, -2, 0)), scale);
    auto a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());*/

    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(0, -2, 0), sunPos, scale, 0, true);

    //glBindTexture(GL_TEXTURE_2D, wallTexture);

    //wall1
    /*auto a_mv_matrix = glm::translate(mv_matrix, glm::vec3(0, -2, scale.z/2));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    auto a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());*/

    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(0, -2, scale.z/2), sunPos, scale, 90, true);

    //wall2
    /*auto a_mv_matrix = glm::translate(mv_matrix, glm::vec3(scale.x/2, -2, 0));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
    auto a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));*/

    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(scale.x/2, -2, 0), sunPos, scale, 90, false);

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    //wall3
    /*a_mv_matrix = glm::translate(mv_matrix, glm::vec3(-scale.x/2, -2, 0));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
    a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));*/

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(-scale.x/2, -2, 0), sunPos, scale, 90, false);

    //wall4 1/2
    /*a_mv_matrix = glm::translate(mv_matrix, glm::vec3(-scale.x/2 - 2, -2, -scale.z/2));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    //wall4 2/2
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(scale.x/2 + 2, -2, -scale.z/2));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());*/

    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(-scale.x/2 - 2, -2, -scale.z/2), sunPos, scale, 90, true);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(scale.x/2 + 2, -2, -scale.z/2), sunPos, scale, 90, true);
}

void drawCorridor(const SimpleTextureProgram& program, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, int idx, 
                GLuint floor_tex, GLuint wall_brick_tex) {
    auto mv_matrix =  glm::translate( camera.getViewMatrix(), origin );
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    glm::vec3 scale(4, 15, 2);

    program.m_Program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floor_tex);
    
    //floor
    auto a_mv_matrix = glm::scale(glm::translate(mv_matrix, glm::vec3(0, -2, 0)), scale);
    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    glBindTexture(GL_TEXTURE_2D, wall_brick_tex);

    //wall2
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(scale.x/2, -2, 0));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

    //wall3
    a_mv_matrix = glm::translate(mv_matrix, glm::vec3(-scale.x/2, -2, 0));
    a_mv_matrix = glm::scale(a_mv_matrix, scale);
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
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