#include "model.h"
#include <cmath>

using namespace std;

vec3 model::getTexColor(double u, double v) const {
    int uTex = int(u * texWidth);
    int vTex = int((1 - v) * texHeight);

    uTex = max(uTex, 0);
    vTex = max(vTex, 0);
    uTex = min(uTex, texWidth - 1);
    vTex = min(vTex, texHeight - 1);
    vec3 res;
    for (int i = 0; i < 3; i++)
        res[i] = *((pTextureData + (vTex * texWidth + uTex) * 3) + i);
    return res;
}

void model::addTriangle(const Triangle &t) {
    tris.push_back(t);
}

void model::addLine(const Point &start, const Point &end) {
    lines.emplace_back(start, end);
}

model model::translate(vec3 v) {
    model res = *this;
    Matrix t = Matrix::identity();
    for (int i = 0; i < 3; i++)
        t[i][3] = v[i];
    res.modelMatrix *= t;
    return res;
}

model model::rotate(double deg, vec3 r) {
    model res = *this;
    deg = deg * acos(-1) / 180;
    Matrix rot = Matrix::identity();
    rot *= cos(deg);
    double len = sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
    r = vec3(r[0] / len, r[1] / len, r[2] / len);
    Matrix n = {{r[0], r[1], r[2]}};

    Matrix N = {
            {0,     -r[2], r[1]},
            {r[2],  0,     -r[0]},
            {-r[1], r[0],  0}};
    rot += n.trans() * n * (1 - cos(deg)) + N * sin(deg);
    rot[3][3] = 1;
    res.modelMatrix *= rot;
    return res;
}

model model::scale(vec3 v) {
    model res = *this;
    Matrix t = Matrix::identity();
    for (int i = 0; i < 3; i++)
        t[i][i] = v[i];
    res.modelMatrix *= t;
    return res;
}

model model::cube(bool frame) {
    model res;
    Point p[8];
    for (int i = 0; i < 8; i++)
        p[i] = {double(i % 2), double((i / 2) % 2), -double((i / 4) % 2)};

    if (frame) {
        for (int i = 0; i < 8; i++)
            for (int j = i + 1; j < 8; j++) {
                int sum = 0;
                for (int k = 0; k < 3; k++)
                    sum += (p[i][k] != p[j][k]);
                if (sum == 1)
                    res.addLine(p[i], p[j]);
            }
    }

    res.addTriangle(Triangle{p[1], p[2], p[0]});
    res.addTriangle(Triangle{p[3], p[2], p[1]});
    res.addTriangle(Triangle{p[4], p[6], p[5]});
    res.addTriangle(Triangle{p[5], p[6], p[7]});

    res.addTriangle(Triangle{p[0], p[4], p[1]});
    res.addTriangle(Triangle{p[1], p[4], p[5]});
    res.addTriangle(Triangle{p[3], p[6], p[2]});
    res.addTriangle(Triangle{p[7], p[6], p[3]});

    res.addTriangle(Triangle{p[2], p[4], p[0]});
    res.addTriangle(Triangle{p[6], p[4], p[2]});
    res.addTriangle(Triangle{p[1], p[5], p[3]});
    res.addTriangle(Triangle{p[3], p[5], p[7]});

    return res;
}

model model::plain(bool frame) {
    model res;
    Point p[4];
    for (int i = 0; i < 4; i++)
        p[i] = {double(i % 2), double((i / 2) % 2)};
    if (frame) {
        res.addLine(p[0], p[1]);
        res.addLine(p[0], p[2]);
        res.addLine(p[1], p[3]);
        res.addLine(p[2], p[3]);
    }
    Triangle t1 = {p[1], p[2], p[0]};
    t1.setTexCoord(1, 0, 0);
    t1.setTexCoord(0, 1, 1);
    t1.setTexCoord(0, 0, 2);

    Triangle t2 = {p[3], p[2], p[1]};
    t2.setTexCoord(1, 1, 0);
    t2.setTexCoord(0, 1, 1);
    t2.setTexCoord(1, 0, 2);

    res.addTriangle(t1);
    res.addTriangle(t2);
    return res;
}

void model::loadTexture(const char *name) {
    pTextureData = stbi_load(name, &texHeight, &texWidth, nullptr, 3);
}
