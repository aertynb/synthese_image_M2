#version 330 core

in vec2 vVertexTex;
in vec3 vVertexNormal;
in vec3 vVertexPos;

uniform sampler2D uEarthTexture; 
uniform sampler2D uCloudTexture; 

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightIntensity;
uniform vec3 uLightDir_vs; // viewspace

out vec3 fFragColor;

vec3 blinnPhong(vec3 normal, vec3 fragPos_vs) {
    vec3 N = normalize(normal);
    vec3 L = normalize(uLightDir_vs);
    vec3 V = normalize(-fragPos_vs); // Opposé de la position du fragment
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = uKd * diff * uLightIntensity;

    float spec = 0.0;
    if (diff > 0.0) {
        spec = pow(max(dot(N, H), 0.0), uShininess);
    }
    vec3 specular = uKs * spec * uLightIntensity;

    return diffuse + specular;
}


void main() {
    vec3 earthTex = texture(uEarthTexture, vVertexTex).xyz;
    vec3 cloudTex = texture(uCloudTexture, vVertexTex).xyz;

    vec3 lighting = blinnPhong(vVertexNormal, vVertexPos);

    fFragColor = (earthTex + cloudTex) * lighting;
}