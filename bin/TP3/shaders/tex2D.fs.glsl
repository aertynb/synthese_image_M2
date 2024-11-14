#version 330 core

in vec2 vFragTex;
in vec2 vVertexPosition;

uniform sampler2D uTexture;

out vec3 fFragColor;

void main() {
  vec3 tex = texture(uTexture, vFragTex).xyz;
  //fFragColor = vec3(1., 0., 0.);
  fFragColor = tex;
};