#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;

out vec3 fFragColor;

float color_particule(float a, float b, float dist) {
    return a * exp(-b * pow(dist, 2));
};

void main() {
  //fFragColor = vFragColor;
  fFragColor = vFragColor * color_particule(1., 100., distance(vec2(0., 0.), vVertexPosition));
  //fFragColor = vFragColor * smoothstep(0.4, 0.5, max(abs(fract(8.0 * vVertexPosition.x - 0.5 * mod(floor(8.0 * vVertexPosition.y), 2.0)) - 0.5), abs(fract(8.0 * vVertexPosition.y) - 0.5)));
};