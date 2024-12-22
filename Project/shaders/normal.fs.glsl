#version 330 core

in vec2 fVertexTex;
in vec3 fVertexNormal;

out vec3 fFragColor;

void main() {
    fFragColor = vec3(fVertexNormal.x, 1, 1);
}