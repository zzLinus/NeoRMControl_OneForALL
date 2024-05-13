#include "camera.h"
#include <cmath>

void camera::updViewMartx() {
    if (viewTarget) {
        lookat = (*viewTarget - pos).normalize();
    }
    right = lookat.cross(worldUp).normalize();
    up = right.cross(lookat).normalize();
    viewMatrix = Matrix::identity();
    for (int i = 0; i < 3; i++)
        viewMatrix[i][3] = -pos[i];
    viewMatrix = Matrix{
            {right[0],   right[1],   right[2]},
            {up[0],      up[1],      up[2]},
            {-lookat[0], -lookat[1], -lookat[2]},
            {0,          0,          0, 1}} *
                 viewMatrix;
}

void camera::init(vec3 _pos, double _fov, double _aspect_ratio, double _zNear, double _zFar, vec3 _lookat) {
    pos = _pos;
    fov = _fov * acos(-1) / 180;
    aspect_ratio = _aspect_ratio;
    zNear = -_zNear;
    zFar = -_zFar;
    lookat = _lookat.normalize();

    top = -zNear * tan(this->fov / 2);

    updViewMartx();
}

void camera::resize(int width, int height) {
    aspect_ratio = (double) width / height;
    double right_t = top * aspect_ratio;

    projectionMatrix =
            {{zNear / right_t, 0,           0,                               0},
             {0,             zNear / top, 0,                               0},
             {0,             0,           (zNear + zFar) / (zNear - zFar), (2 * zNear * zFar) / (zFar - zNear)},
             {0,             0,           1,                               0}};
}

void camera::transform(vec3 v) {
    pos += right * v[0];
    pos += up * v[1];
    pos += lookat * -v[2];
    updViewMartx();
}

void camera::setViewTarget(vec3 target) {
    viewTarget = target;
    updViewMartx();
}
