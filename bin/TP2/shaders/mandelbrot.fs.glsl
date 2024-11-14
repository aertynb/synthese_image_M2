#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;

out vec3 fFragColor;

vec2 complexSqr(vec2 z){
  return vec2(pow(z.x, 2) - pow(z.y, 2), 2 * z.x * z.y);
}

void main() {
    int n_max = 10000;

    vec2 c0 = vVertexPosition;
    for (int i = 0; i < n_max; i++){
        vec2 zn_1 = complexSqr(c0) + c0;
        if (zn_1.x > 2. && zn_1.y > 2.){
            fFragColor = vec3(1., 1., 1.);
            return;
        }
        c0 = zn_1;
    }

    fFragColor = vec3(0., 0., 0.);
}