#include "lightShader.h"
#include <algorithm>
#include <cmath>

lightShader::lightShader() {
    I = 500;
    Ia = 10;
    ka = vec3(0.005, 0.005, 0.005);
    ks = vec3(0.7937, 0.7937, 0.7937);
    p = 150;
}

vec3 lightShader::operator()(vec3 xyz, vec3 normal, vec3 color, vec3 view_pos) {
    vec3 res;
    vec3 view_dir = (view_pos - xyz).normalize();
    for (vec3 position: lightPos) {
        double r = (position - xyz).len();
        vec3 direction = (position - xyz).normalize();
        vec3 h = (direction + view_dir).normalize();
        double intensity = I / r / r;
        res += ka * Ia * 255;
        res += color * intensity * std::max(0.0, normal * direction);
        res += ks * intensity * std::max(0.0, pow(normal * h, p)) * 255;
    }
    for (int i = 0; i < 3; i++)
        if (res[i] > 255)
            res[i] = 255;
    return res;
}
