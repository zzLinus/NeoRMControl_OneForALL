#pragma once
#include "Point.h"
#include <optional>
#include <array>

class Triangle
{
    friend class rasterizer;

private:
    int colorR[3];
    int colorG[3];
    int colorB[3];

    std::optional<vec3> normal[3];
    double uTex[3], vTex[3];
    Point ver[3];

    void initColor();

public:
    Triangle(std::initializer_list<Point> list);
    Triangle() { initColor(); }

    Triangle &operator=(std::initializer_list<Point> list);

    void printData();

    bool inside(double x, double y);

    Triangle normalize();

    void setVertex(vec3 p, int idx);
    void setColor(int r, int g, int b, int idx = -1);
    void setNormal(vec3 n, int idx);
    void setTexCoord(double u, double v, int idx);

    Point &getVertex(int idx) { return ver[idx]; }
    const Point& getVertex(int idx) const {return ver[idx];}
};
