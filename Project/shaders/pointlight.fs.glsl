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
uniform vec3 uLightPos_vs; // viewspace

out vec3 fFragColor;

vec3 pointLight() {
    // Normalize vectors
    vec3 N = normalize(vVertexNormal);
    vec3 L = normalize(uLightPos_vs - vVertexPos);
    vec3 V = normalize(-vVertexPos); // Opposé de la position du fragment

    // Calculate halfway vector
    vec3 H = normalize(L + V);

    // Distance and attenuation
    float d = max(distance(vVertexPos, uLightPos_vs), 0.001);
    float attenuation = 1.0 / (d * d);

    // Diffuse term
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = uKd * diff * (uLightIntensity * attenuation);

    // Specular term
    float spec = 0.0;
    if (diff > 0.0) {
        spec = pow(max(dot(N, H), 0.0), uShininess);
    }
    vec3 specular = uKs * spec * (uLightIntensity * attenuation);

    // Combine results
    return diffuse + specular;
}

void main() {
    vec3 earthTex = texture(uEarthTexture, vVertexTex).xyz;
    vec3 cloudTex = texture(uCloudTexture, vVertexTex).xyz;

    // Blend or combine textures
    vec3 combinedTex = mix(earthTex, cloudTex, 0.5);

    // Compute lighting
    vec3 lighting = pointLight();

    // Final fragment color
    fFragColor = clamp((earthTex + cloudTex) * lighting, 0.0, 1.0);
}
