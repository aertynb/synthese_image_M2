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

bool animationStop = false;
bool lightOn = true;
float timeR = 0.0f;
float intensity = 30.0f;

glm::FreeflyCamera camera {};

void drawEarthMoon(const PointLightProgram& program, const SimpleTextureProgram& moonProgram, const std::vector<std::unique_ptr<Object>>& objs, 
                   const std::vector<glm::vec3>& moons, glm::vec3 origin, int objIdx, GLuint earth_tex, GLuint moon_tex) {

    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto mv_matrix = camera.getViewMatrix() * model_matrix;
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));

    // Calcul de la direction de la lumière en fonction du temps
    //glm::vec3 light_dir_world = glm::rotate(glm::mat4(1.f), timeR, glm::vec3(0, 1, 0)) * glm::vec4(1, 1, 1, 0);
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
        auto a_mv_matrix = glm::rotate(mv_matrix, timeR, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::translate(a_mv_matrix, moon_pos);
        a_mv_matrix = glm::scale(a_mv_matrix, glm::vec3(0.2f));

        glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
        glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());
    }
}

void drawPlanetarySystem(const PointLightProgram& program, const SimpleTextureProgram& moonProgram, glm::vec3 origin, int objIdx, 
                   GLuint sunTexture, const std::vector<Planet>& planets, const std::vector<std::unique_ptr<Object>>& objs) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, timeR, glm::vec3(0, 1, 0));
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
    glUniform3f(program.uLightIntensity, intensity, intensity/2, intensity/2);

    glUniform3f(program.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(program.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(program.uShininess, 32.0f);

    glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());

    for (const auto& planet : planets) {
        auto a_mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, timeR*planet.speed, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::translate(a_mv_matrix, planet.position);
        a_mv_matrix = glm::rotate(a_mv_matrix, timeR*planet.rotationSpeed, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::scale(glm::scale(a_mv_matrix, glm::vec3(0.2f)), glm::vec3(planet.scale));

        glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));

        glBindTexture(GL_TEXTURE_2D, planet.texture);
        lightPos_world = -planet.position;
        lightPos_vs = glm::vec3(glm::scale( glm::rotate( model_matrix, timeR*planet.rotationSpeed, glm::vec3(0, -1, 0)), glm::vec3(5) ) * glm::vec4(lightPos_world, 0.0));
        glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
        glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());
    }
}

void drawPlanetarySystem(const SimpleTextureProgram& program, glm::vec3 origin, int objIdx, 
                   GLuint sunTexture, const std::vector<Planet>& planets, const std::vector<std::unique_ptr<Object>>& objs) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, timeR, glm::vec3(0, 1, 0));
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));

    program.m_Program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTexture);

    glUniform1i(program.uTexture, 0);

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());

    for (const auto& planet : planets) {
        auto a_mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, timeR*planet.speed, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::translate(a_mv_matrix, planet.position);
        a_mv_matrix = glm::rotate(a_mv_matrix, timeR*planet.rotationSpeed, glm::vec3(0, 1, 0));
        a_mv_matrix = glm::scale(glm::scale(a_mv_matrix, glm::vec3(0.2f)), glm::vec3(planet.scale));

        glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));

        glBindTexture(GL_TEXTURE_2D, planet.texture);
        glDrawArrays(GL_TRIANGLES, 0, objs[objIdx]->getVertexCount());
    }
}

void drawQuad(const PointLightProgram& program, GLsizei vertexCount, glm::mat4 mv_matrix, glm::mat4 proj_matrix,
              glm::mat4 model_matrix, glm::vec3 origin, glm::vec3 lightPos, glm::vec3 scale, float rotation, bool xAxis) {
    auto a_mv_matrix = glm::translate(mv_matrix, origin);
    a_mv_matrix = glm::scale(a_mv_matrix, scale);

    if (xAxis) {
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(1, 0, 0));
    } else {
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(1, 0, 0));
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(0, 0, 1));
    }

    auto a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

    glm::vec3 lightPos_vs = glm::vec3(model_matrix * glm::vec4(-lightPos, 1.0));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));
    glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void drawCircle(const BlinnPhongProgram& bpProgram, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, GLuint texture, int idx) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    
    bpProgram.m_Program.use();

    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec3 scale (1.5, 0.01, 1.5);
    glm::vec3 pos(0, 1.5, 0);

    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), timeR * 2, glm::vec3(0, 0, 1));

    glm::vec3 light_dir_world = glm::vec3(rotation_matrix * glm::vec4(1, 1, 1, 0));
    glm::vec3 light_dir_vs = glm::vec3(camera.getViewMatrix() * glm::vec4(light_dir_world, 0.0));

    auto mv_matrix = glm::scale(glm::translate(camera.getViewMatrix() * model_matrix, pos), scale);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));

    glUniformMatrix4fv(bpProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(bpProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(bpProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glUniform3fv(bpProgram.uLightDir_vs, 1, glm::value_ptr(light_dir_vs));
    glUniform3f(bpProgram.uLightIntensity, 1.f, 1.0f, 1.0f);

    glUniform3f(bpProgram.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(bpProgram.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(bpProgram.uShininess, 32.0f);

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
}

void drawCircle(const SimpleTextureProgram& program, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, GLuint texture, int idx) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    
    program.m_Program.use();

    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec3 scale (1.5, 0.01, 1.5);
    glm::vec3 pos(0, 1.5, 0);

    auto mv_matrix = glm::scale(glm::translate(camera.getViewMatrix() * model_matrix, pos), scale);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
}

void drawBoard(const PointLightProgram& program, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, glm::vec3 lightPos_world, GLuint texture, GLuint boardTexture, int idx) {
    program.m_Program.use();
    
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec3 scale (0.5, 3, 0.5);

    glm::vec3 lightPos_vs = glm::vec3( model_matrix * glm::vec4(lightPos_world, 0.0));

    glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
    glUniform3f(program.uLightIntensity, 30.0f, 30.0f, 30.0f);

    glUniform3f(program.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(program.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(program.uShininess, 32.0f);

    auto mv_matrix = glm::scale( camera.getViewMatrix() * model_matrix, scale );
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));
    
    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
}

void drawBoard(const SimpleTextureProgram& program, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, GLuint texture, GLuint boardTexture, int idx) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec3 scale (0.2, 3, 0.2);

    auto mv_matrix = glm::scale( camera.getViewMatrix() * model_matrix, scale );
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));
    
    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
}

void drawPanel(const SimpleTextureProgram& program, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, GLuint texture, int idx) {
    auto model_matrix = glm::rotate( glm::translate(glm::mat4(1.0f), origin), glm::radians(90.0f), glm::vec3(-1,0,0) );
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec3 scale (2, 0.1, 1.5);

    auto mv_matrix = glm::scale( camera.getViewMatrix() * model_matrix, scale );
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));
    
    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
}

void drawQuad(const SimpleTextureProgram& program, GLsizei vertexCount, glm::mat4 mv_matrix, glm::mat4 proj_matrix,
              glm::vec3 origin, glm::vec3 scale, float rotation, bool xAxis) {
    auto a_mv_matrix = glm::translate(mv_matrix, origin);
    a_mv_matrix = glm::scale(a_mv_matrix, scale);

    if (xAxis) {
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(1, 0, 0));
    } else {
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(1, 0, 0));
        a_mv_matrix = glm::rotate(a_mv_matrix, glm::radians(rotation), glm::vec3(0, 0, 1));
    }

    auto a_normal_matrix = glm::transpose(glm::inverse(a_mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * a_mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(a_mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(a_normal_matrix));

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void drawPyramide(const SimpleTextureProgram& program, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, GLuint texture, int idx) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec3 scale (10, 10, 10);
    glm::vec3 pos (0, -6, 0);

    for (auto step = 0; step < 10; step++) {
        auto mv_matrix = glm::scale( glm::translate( camera.getViewMatrix() * model_matrix, pos ), scale );
        auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
        auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));
        
        glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
        glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
        glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

        glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

        scale = glm::vec3(scale.x-1, scale.y-1, scale.z-1);

        pos = glm::vec3(0, pos.y+1, 0);
    }
}

void drawPyramide(const PointLightProgram& program, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, glm::vec3 lightPos_world, GLuint texture, int idx) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);

    program.m_Program.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec3 lightPos_vs = glm::vec3(glm::scale( model_matrix, glm::vec3(1) ) * glm::vec4(-lightPos_world, 0.0));

    glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(lightPos_vs));
    glUniform3f(program.uLightIntensity, 30.0f, 30.0f, 30.0f);

    glUniform3f(program.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(program.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(program.uShininess, 32.0f);

    glm::vec3 scale (10, 10, 10);
    glm::vec3 pos (0, -6, 0);

    for (auto step = 0; step < 10; step++) {
        auto mv_matrix = glm::scale( glm::translate( camera.getViewMatrix() * model_matrix, pos ), scale );
        auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
        auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));
        
        glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
        glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
        glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

        glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());

        scale = glm::vec3(scale.x-1, scale.y-1, scale.z-1);

        pos = glm::vec3(0, pos.y+1, 0);
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

    glUniform3f(program.uLightIntensity, intensity, intensity/2, intensity/2);

    glUniform3f(program.uKd, 0.8f, 0.8f, 0.8f);
    glUniform3f(program.uKs, 0.5f, 0.5f, 0.5f);
    glUniform1f(program.uShininess, 32.0f);

    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(0, -2, 0), sunPos, scale, 0, true);

    glBindTexture(GL_TEXTURE_2D, wallTexture);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(0, -2, scale.z/2), sunPos, scale, 90, true);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(scale.x/2, -2, 0), sunPos, scale, 90, false);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(-scale.x/2, -2, 0), sunPos, scale, 90, false);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(-scale.x/2 - 2, -2, -scale.z/2), sunPos, scale, 90, true);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, model_matrix, glm::vec3(scale.x/2 + 2, -2, -scale.z/2), sunPos, scale, 90, true);
}

void drawRoom(const SimpleTextureProgram& program, const std::vector<std::unique_ptr<Object>>& objs, float rotateValue, glm::vec3 origin,
            int idx, GLuint floorTexture, GLuint wallTexture) {
    auto model_matrix = glm::translate(glm::mat4(1.0f), origin);
    auto mv_matrix = glm::rotate(camera.getViewMatrix() * model_matrix, glm::radians(rotateValue), glm::vec3(0, 1, 0) );
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    glm::vec3 scale(24, 15, 20);

    program.m_Program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, glm::vec3(0, -2, 0), scale, 0, true);

    glBindTexture(GL_TEXTURE_2D, wallTexture);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, glm::vec3(0, -2, scale.z/2), scale, 90, true);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, glm::vec3(scale.x/2, -2, 0), scale, 90, false);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, glm::vec3(-scale.x/2, -2, 0), scale, 90, false);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, glm::vec3(-scale.x/2 - 2, -2, -scale.z/2), scale, 90, true);
    drawQuad(program, objs[idx]->getVertexCount(), mv_matrix, proj_matrix, glm::vec3(scale.x/2 + 2, -2, -scale.z/2), scale, 90, true);
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

void drawTransparent(const TransparencyProgram& program, const std::vector<std::unique_ptr<Object>>& objs, glm::vec3 origin, 
                     glm::vec3 scale, int idx, GLuint texture) {
    
    program.m_Program.use();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDepthMask(GL_FALSE);

    auto model_matrix = glm::rotate( glm::scale( glm::translate(glm::mat4(1.0f), origin), scale ), glm::radians(90.0f), glm::vec3(1, 0, 0));
    auto mv_matrix = camera.getViewMatrix() * model_matrix;
    auto proj_matrix = glm::perspective(glm::radians(70.f), (float) window_width / window_height, 0.1f, 100.f);
    auto normal_matrix = glm::transpose(glm::inverse(mv_matrix));

    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glUniform1i(program.uTexture, 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, objs[idx]->getVertexCount());
    
    glDepthMask(GL_TRUE);
    
    glDisable(GL_BLEND);
}