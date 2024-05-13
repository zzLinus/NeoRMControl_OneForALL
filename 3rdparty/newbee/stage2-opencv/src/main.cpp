#include "simpleWindow.h"
#include "Matrix.h"
#include "Point.h"
#include "Triangle.h"
#include "vec.h"
#include "rasterizer.h"
#include "model.h"
#include "OBJ_Loader.h"
#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include <any>
#include <opencv2/opencv.hpp>

#ifdef _WINDOWS
#include <windows.h>
#endif

using namespace std;

extern int width, height;
camera cam;
simpleWindow wnd;
rasterizer ras(width, height);
void solve(cv::Mat &);
float getfps()
{
    static queue<int> q;
    int dur = 1;
    clock_t nt = clock();
    while (q.size() && (nt - q.front()) > dur * CLOCKS_PER_SEC)
        q.pop();
    q.push(nt);
    if (q.size() == 1)
        return 0;
    return q.size() / (float(nt - q.front()) / CLOCKS_PER_SEC);
}

struct ballinfo
{
    model ball;
    vec3 pos;
    vec3 force;
};

vector<ballinfo> vballs;

extern int stage;
extern string imgroot;
int main()
{
    srand((unsigned)time(0));
#ifdef _WINDOWS
    SetProcessDPIAware();
#endif
    wnd.create("hello world", width, height);
    // wnd.setBkColor(220, 230, 210);
    wnd.setBkColor(246, 246, 246);

    cam.init(vec3(0, 0, 10), 45, (double)width / height, 0.1, 100);
    cam.setViewTarget(vec3(0, 0, 0));
    ras.setCamera(cam);

    // ras.addLight(vec3(20, 20, 20));
    // ras.addLight(vec3(-20, 20, 0));

    ras.bindSetPixelFunc([](int x, int y, int r, int g, int b)
                         { wnd.setPixel(x, y, r, g, b); });

    wnd.setResizeCallback(bind(&rasterizer::setRasterizeSize, &ras, placeholders::_1, placeholders::_2));

    model mod, mod2, ball;

    mod = model::plain();
    ball = model::plain();
    ball = ball.scale(vec3(0.2, 0.2, 0.2));

    ball.loadTexture((imgroot + "2.jpg").c_str());
    mod.loadTexture((imgroot + "1.jpg").c_str());
    mod2 = mod;
    mod2 = mod2.translate(vec3(2, 2, 0));
    mod = mod.translate(vec3(-2, -2, 0));
    // mod = mod.scale(vec3(2.5, 2.5, 2.5));
    // mod = mod.rotate(140, vec3(0, 1, 0));

    static clock_t lst = clock();

    while (!wnd.shouldClose())
    {
        char key = wnd.getKey();
        if (key == 27)
            break;
        mod = mod.rotate(.5 * cos(clock() / 1000.), vec3(0, 1, 0));
        mod = mod.rotate(2 * cos(clock() / 1000.), vec3(0, 0, 1));
        ras.pushModel(mod);

        mod2 = mod2.rotate(.5 * sin(clock() / 1000.), vec3(0, 1, 0));
        mod2 = mod2.rotate(2 * sin(clock() / 1000.), vec3(0, 0, 1));
        ras.pushModel(mod2);

        if (stage == 2)
        {
            if (rand() % 100 == 0 || clock() - lst > 1000)
            {
                vballs.push_back({ball,
                                  vec3(0, 0, 0),
                                  vec3(1. * rand() / RAND_MAX - .5, 1. * rand() / RAND_MAX - .5, 1. * rand() / RAND_MAX - .5)});
                lst = clock();
            }
            for (auto it = vballs.begin(); it != vballs.end(); it++)
            {
                it->pos += it->force;
                
                auto ball = it->ball.translate(it->pos);
                ras.pushModel(ball);
            }
            for (int idx = 0;idx < vballs.size();idx++){
                double x = vballs[idx].pos[0];
                double y = vballs[idx].pos[1];
                double z = vballs[idx].pos[2];
                if (abs(x) > 100 || abs(y) > 100 || abs(z) > 100)
                {
                    vballs.erase(vballs.begin() + idx);
                    idx--;
                }
            }
        }
        wnd.clear();
        ras.draw();
        solve(wnd.wndMat);
        wnd.show();
        cout << '\r' << getfps() << "      ";
    }
    return 0;
}