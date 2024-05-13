#pragma once
#include "Triangle.h"
#include "vec.h"
#include "Matrix.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include <vector>

class model
{
private:
    std::vector<Triangle> tris;
    std::vector<std::pair<Point,Point>> lines;
    unsigned char* pTextureData = nullptr;
    int texWidth,texHeight;
    friend class rasterizer;

    vec3 getTexColor (double u,double v) const;
public:
    Matrix modelMatrix;
    model() { modelMatrix = Matrix::identity(); }

    void addTriangle(const Triangle &t);
    void addLine(const Point& start,const Point& end);

    model translate(vec3 v);
    model rotate(double deg, vec3 r);
    model scale(vec3 v);
    Triangle &getTriangle(int idx) { return tris[idx]; }

    void loadTexture(const char* name);
    static model cube(bool frame = false);
    static model plain(bool frame = false);
};