#pragma once

#include "vec.h"
#include "Matrix.h"
#include <optional>

class camera {
    friend class rasterizer;

private:
    double fov = 0, aspect_ratio = 0, zNear = 0, zFar = 0, top = 0;
    vec3 pos;
    Matrix projectionMatrix;
    Matrix viewMatrix;
    vec3 lookat, right, up;
    const vec3 worldUp = vec3(0, 1, 0);
    std::optional<vec3> viewTarget;

    void updViewMartx();

public:
    camera() = default;

    void init(vec3 _pos, double _fov, double _aspect_ratio, double _zNear, double _zFar, vec3 _lookat = {0, 0, -1});

    void resize(int width, int height);

    void transform(vec3 v);

    void setViewTarget(vec3 target);
};
