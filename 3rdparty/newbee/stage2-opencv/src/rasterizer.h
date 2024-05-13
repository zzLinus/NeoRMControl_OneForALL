#pragma once
#include "Matrix.h"
#include "model.h"
#include "vec.h"
#include "camera.h"
#include "lightShader.h"
#include "ThreadPool.h"
#include <functional>
#include <atomic>

class rasterizer
{
private:
    int width, newWidth;
    int height, newHeight;
    camera *pCam = nullptr;
    std::atomic<float> *zBuffer;
    lightShader lig;
    ThreadPool &poolIns;
    std::function<void(int, int, int, int, int)> setPixel;
    std::vector<model> models;

    void clearZBuffer();
    void drawTriangle(Triangle tri, Triangle ctri,const model& mod);

public:
    rasterizer(int width, int height);
    void setRasterizeSize(int width, int height);
    void draw();
    void setCamera(camera &cam) { pCam = &cam; }
    void addLight(vec3 pos);
    void drawLine(Point begin, Point end, vec3 lineColor = {255, 255, 255});
    void bindSetPixelFunc(const std::function<void(int, int, int, int, int)> &f) { setPixel = f; }
    void pushModel(const model &m) { models.push_back(m); }
};
