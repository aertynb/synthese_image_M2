#version 330 core

in vec2 vVertexTex;
in vec3 vVertexNormal;
in vec3 vVertexPos;

uniform sampler2D uEarthTexture; 
uniform sampler2D uCloudTexture; 

out vec3 fFragColor;

// shader optimise les variables donc si j'en utilise pas 1 l'autre sera prise à la place et sera en 1ère place.

void main() {
    vec3 earthTex = texture(uEarthTexture, vVertexTex).xyz;
    vec3 cloudTex = texture(uCloudTexture, vVertexTex).xyz;
    //vec3(fVertexNormal.x, 1, 1);
    fFragColor = earthTex + cloudTex;
}