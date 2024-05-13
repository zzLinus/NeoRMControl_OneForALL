#include "simpleWindow.h"
#include "vec.h"
#include "rasterizer.h"
#include "config.h"
#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <opencv2/opencv.hpp>

#ifdef _WINDOWS
#include <windows.h>
#endif

using namespace std;
using namespace std::chrono;

camera cam;
simpleWindow wnd;
rasterizer ras(width, height);
std::mt19937 gen(std::random_device{}());
system_clock::time_point program_start_time = system_clock::now();

template<typename T = double>
T getSecond() {
    return duration<T>(system_clock::now() - program_start_time).count();
}

void solve(cv::Mat &);

float getFps() {
    static queue<system_clock::time_point> q;
    const auto now_time = system_clock::now();
    while (!q.empty() && (now_time - q.front()) > 1s) {
        q.pop();
    }
    q.push(now_time);
    if (q.size() <= 1) {
        return 0;
    }
    return (float) q.size() / duration<float>(system_clock::now() - q.front()).count();
}

struct BallInfo {
    model ball;
    vec3 pos;
    vec3 force;
};

vector<BallInfo> vballs;

int main() {
#ifdef _WINDOWS
    SetProcessDPIAware();
#endif
    wnd.create("hello world", width, height);
    wnd.setBkColor(246, 246, 246);

    cam.init(vec3(0, 0, 10), 45, (double) width / height, 0.1, 100);
    cam.setViewTarget(vec3(0, 0, 0));
    ras.setCamera(cam);

    ras.bindSetPixelFunc([](int x, int y, int r, int g, int b) { wnd.setPixel(x, y, r, g, b); });

    model mod, mod2, ball;

    mod = model::plain();
    ball = model::plain();
    ball = ball.scale(vec3(0.2, 0.2, 0.2));

    ball.loadTexture((std::string(img_root) + "2.jpg").c_str());
    mod.loadTexture((std::string(img_root) + "1.jpg").c_str());
    mod2 = mod;
    mod2 = mod2.translate(vec3(2, 2, 0));
    mod = mod.translate(vec3(-2, -2, 0));

    static system_clock::time_point lst = system_clock::now();

    while (true) {
        char key = wnd.getKey();
        if (key == 27) {
            break;
        }
        mod = mod.rotate(.5 * cos(getSecond()), vec3(0, 1, 0));
        mod = mod.rotate(2 * cos(getSecond()), vec3(0, 0, 1));
        ras.pushModel(mod);

        mod2 = mod2.rotate(.5 * sin(getSecond()), vec3(0, 1, 0));
        mod2 = mod2.rotate(2 * sin(getSecond()), vec3(0, 0, 1));
        ras.pushModel(mod2);

        if (stage == 2) {
            std::uniform_int_distribution percent(0, 99);
            std::uniform_real_distribution real_rand(-1., 1.);
            if (percent(gen) == 0 || system_clock::now() - lst > 1s) {
                vballs.push_back({
                                         ball,
                                         vec3(0, 0, 0),
                                         vec3(real_rand(gen), real_rand(gen), real_rand(gen))
                                 });
                lst = system_clock::now();
            }
            for (auto &[this_ball, pos, force]: vballs) {
                pos += force;
                ras.pushModel(this_ball.translate(pos));
            }
            for (int idx = 0; idx < vballs.size(); idx++) {
                double x = vballs[idx].pos[0];
                double y = vballs[idx].pos[1];
                double z = vballs[idx].pos[2];
                if (abs(x) > 100 || abs(y) > 100 || abs(z) > 100) {
                    vballs.erase(vballs.begin() + idx);
                    idx--;
                }
            }
        }
        wnd.clear();
        ras.draw();
        solve(wnd.wndMat);
        wnd.show();
        cout << '\r' << "fps: " << getFps() << std::flush;
    }
    return 0;
}
