#version 330 core

in vec2 vVertexTex;
in vec3 vVertexNormal;
in vec3 vVertexPos;

uniform sampler2D uTexture; 

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightIntensity;
uniform vec3 uLightPos_vs; // viewspace

out vec3 fFragColor;

vec3 pointLight() {
    vec3 N = normalize(vVertexNormal);
    vec3 L = normalize(uLightPos_vs - vVertexPos);
    vec3 V = normalize(-vVertexPos); // Opposé de la position du fragment

    vec3 H = normalize(L + V);

    float d = max(distance(vVertexPos, uLightPos_vs), 0.001);
    float attenuation = 1.0 / (d);

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = uKd * diff * (uLightIntensity * attenuation);

    float spec = 0.2;
    if (diff > 0.0) {
        spec = pow(max(dot(N, H), 0.0), uShininess);
    }
    vec3 specular = uKs * spec * (uLightIntensity * attenuation);

    // Combine results
    return diffuse + specular;
}

void main() {
    vec3 earthTex = texture(uTexture, vVertexTex).xyz;

    vec3 lighting = pointLight();

    fFragColor = clamp(earthTex * lighting, 0.0, 1.0);
}
