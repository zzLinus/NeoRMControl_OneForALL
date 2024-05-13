#include "Triangle.h"
#include <stdio.h>
#include <memory>

void Triangle::initColor()
{
    setColor(255, 255, 255);
}

Triangle::Triangle(std::initializer_list<Point> list)
{
    *this = list;
    initColor();
}

Triangle &Triangle::operator=(std::initializer_list<Point> list)
{
    auto it = list.begin();
    for (int i = 0; i < 3; i++)
    {
        ver[i] = it[i];
        ver[i][3] = 1;
    }

    return *this;
}

void Triangle::printData()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%-6.3g", ver[i][j]);
        putchar('\n');
    }
}

bool Triangle::inside(double x, double y)
{
    double ax = ver[0][0], ay = ver[0][1];
    double bx = ver[1][0], by = ver[1][1];
    double cx = ver[2][0], cy = ver[2][1];

    bool fg1 = (x - ax) * (cy - ay) >= (y - ay) * (cx - ax);
    bool fg2 = (x - bx) * (ay - by) >= (y - by) * (ax - bx);
    bool fg3 = (x - cx) * (by - cy) >= (y - cy) * (bx - cx);

    return fg1 == fg2 && fg2 == fg3;
}

Triangle Triangle::normalize()
{
    Triangle res = *this;
    for (int i = 0; i < 3; i++)
        res.ver[i] = ver[i].normalize();
    return res;
}

void Triangle::setVertex(vec3 p, int idx)
{
    ver[idx] = {p[0], p[1], p[2], 1};
}

void Triangle::setColor(int r, int g, int b, int idx)
{
    for (int i = 0; i < 3; i++)
        if (idx == -1 || idx == i)
        {
            colorR[i] = r;
            colorG[i] = g;
            colorB[i] = b;
        }
}

void Triangle::setNormal(vec3 n, int idx)
{
    normal[idx] = n;
}

void Triangle::setTexCoord(double u, double v, int idx)
{
    uTex[idx] = u;
    vTex[idx] = v;
}