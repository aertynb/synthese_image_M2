#version 330 core

in vec3 vFragColor;

out vec3 fFragColor;

void main() {
  fFragColor.r = (vFragColor.r + vFragColor.g + vFragColor.b) / 3.;
  fFragColor.g = (vFragColor.r + vFragColor.g + vFragColor.b) / 3.;
  fFragColor.b = (vFragColor.r + vFragColor.g + vFragColor.b) / 3.;
};