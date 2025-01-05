#version 330 core

uniform sampler2D uTexture;

in vec2 vVertexTex;
in vec3 vVertexNormal;
in vec3 vVertexPos;

out vec4 fFragColor;

void main() {
    vec4 texColor = texture(uTexture, vVertexTex);
    fFragColor = texColor; // Debug: Force semi-transparency
}
