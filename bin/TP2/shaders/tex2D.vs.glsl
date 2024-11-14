#version 330 core

layout(location = 1) in vec2 aVertexPosition;
layout(location = 2) in vec2 aVertexTex;

out vec2 vFragTex;
out vec2 vVertexPosition;

//uniform float uTime;
uniform mat3 uModelMatrix;

mat3 translate(float tx, float ty) {
  return mat3(vec3(1,0,0), vec3(0,1,0), vec3(tx,ty,1));
};

mat3 scale(float sx, float sy) {
  return mat3(vec3(sx, 0, 0), vec3(0,sy, 0), vec3(0,0,1));
};

mat3 rotate(float a) {
  return mat3(vec3(cos(radians(a)), sin(radians(a)), 0), vec3(-sin(radians(a)), cos(radians(a)), 0), vec3(0, 0, 1));
};

void main() {
  vFragTex = aVertexTex;
  vVertexPosition = aVertexPosition;
  //gl_Position = vec4(aVertexPosition, 0, 1);
  //gl_Position = vec4(aVertexPosition+0.5, 0, 1); translation
  //gl_Position = vec4(aVertexPosition*2, 0, 1); scale
  //gl_Position = vec4(aVertexPosition*vec2(2,0.5), 0, 1); //scale 2*x et y/2;
  //gl_Position = vec4((translate(0.5, 0.5) * vec3(aVertexPosition, 1)).xy, 0, 1);*
  //gl_Position = vec4((scale(2, 2) * vec3(aVertexPosition, 1)).xy, 0, 1);
  //gl_Position = vec4((scale(2, 0.5) * vec3(aVertexPosition, 1)).xy, 0, 1);
  //gl_Position = vec4((rotate(uTime) * vec3(aVertexPosition, 1)).xy, 0, 1);
  gl_Position = vec4((uModelMatrix * vec3(aVertexPosition, 1)).xy, 0, 1);
};