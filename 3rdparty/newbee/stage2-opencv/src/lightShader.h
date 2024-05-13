#pragma once
#include "vec.h"
#include <vector>

class lightShader
{
public:
    double I, Ia, p;
    vec3 ks, ka;
    std::vector<vec3> lightPos;

    lightShader();

    vec3 operator()(vec3 xyz, vec3 normal, vec3 color, vec3 view_pos);
};
