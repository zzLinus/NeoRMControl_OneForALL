#include "rasterizer.h"
#include <cmath>

using namespace std;

void rasterizer::clearZBuffer() {
    fill_n(zBuffer, width * height, numeric_limits<float>::infinity());
}

static void computeBarycentric2D(double x, double y, const Triangle &t, double *param) {
    double xa = t.getVertex(0).data[0], ya = t.getVertex(0).data[1];
    double xb = t.getVertex(1).data[0], yb = t.getVertex(1).data[1];
    double xc = t.getVertex(2).data[0], yc = t.getVertex(2).data[1];
    param[0] =
            (x * (yb - yc) + (xc - xb) * y + xb * yc - xc * yb) / (xa * (yb - yc) + (xc - xb) * ya + xb * yc - xc * yb);
    param[1] =
            (x * (yc - ya) + (xa - xc) * y + xc * ya - xa * yc) / (xb * (yc - ya) + (xa - xc) * yb + xc * ya - xa * yc);
    param[2] =
            (x * (ya - yb) + (xb - xa) * y + xa * yb - xb * ya) / (xc * (ya - yb) + (xb - xa) * yc + xa * yb - xb * ya);
}

void rasterizer::drawTriangle(Triangle tri, Triangle ctri, const model &mod) {
    int ax = (int) tri.getVertex(0)[0], ay = (int) ceil(tri.getVertex(0)[1]);
    int bx = (int) tri.getVertex(1)[0], by = (int) ceil(tri.getVertex(1)[1]);
    int cx = (int) tri.getVertex(2)[0], cy = (int) ceil(tri.getVertex(2)[1]);

    int minx = min(min(ax, bx), cx);
    int maxx = max(max(ax, bx), cx);
    int miny = min(min(ay, by), cy);
    int maxy = max(max(ay, by), cy);

    int left = max(minx, 0), right = min(maxx, width - 1);
    int lidx = 0, ridx = 0;
    for (int i = left; i <= right; i++) {
        if (lidx == 0 && ridx == 0) {
            for (int j = max(miny, 0); j <= min(maxy, height - 1); j++)
                if (tri.inside(i, j)) {
                    lidx = j;
                    break;
                }
            for (int j = min(maxy, height - 1); j >= max(miny, 0); j--)
                if (tri.inside(i, j)) {
                    ridx = j;
                    break;
                }
            if (lidx == 0 || ridx == 0) {
                continue;
            }
        } else {
            while (tri.inside(i, lidx) && lidx > max(miny, 0)) {
                lidx--;
            }
            while (!tri.inside(i, lidx) && lidx < min(maxy, height - 1)) {
                lidx++;
            }
            if (lidx == min(maxy, height - 1)) {
                while (!tri.inside(i, lidx) && lidx > max(miny, 0)) {
                    lidx--;
                }
                while (tri.inside(i, lidx) && lidx > max(miny, 0)) {
                    lidx--;
                }
                lidx++;
            }
            ridx = max(ridx, lidx);
            while (tri.inside(i, ridx) && ridx < min(maxy, height - 1))
                ridx++;
            while (!tri.inside(i, ridx) && ridx > max(miny, 0))
                ridx--;
        }
        for (int j = max(lidx, 0); j <= min(ridx, height - 1); j++) {
            if (!tri.inside(i, j))
                continue;
            double param[3];
            computeBarycentric2D(i, j, tri, param);
            vec3 p, p2;
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    p[x] += param[y] * tri.getVertex(y)[x];
                    p2[x] += param[y] * ctri.getVertex(y)[x];
                }
            }
            auto z = float(-p[2]);
            if (z >= zBuffer[j * width + i])
                continue;

            double r = 0, g = 0, b = 0;
            for (int k = 0; k < 3; k++) {
                r += param[k] * tri.colorR[k];
                g += param[k] * tri.colorG[k];
                b += param[k] * tri.colorB[k];
            }

            vec3 nor;
            double uTex = 0, vTex = 0;
            for (int k = 0; k < 3; k++) {
                nor += tri.normal[k].value() * param[k];
                uTex += tri.uTex[k] * param[k];
                vTex += tri.vTex[k] * param[k];
            }

            vec3 color;
            vec3 baseColor = mod.pTextureData ? mod.getTexColor(uTex, vTex) : vec3(r, g, b);
            if (lig.lightPos.empty()) {
                color = baseColor;
            } else {
                color = lig(p2, nor, baseColor, pCam ? pCam->pos : vec3(0, 0, 1));
            }
            float oldValue = zBuffer[j * width + i].load();
            while (z < oldValue) {
                if (zBuffer[j * width + i].compare_exchange_weak(oldValue, z)) {
                    setPixel(i, j, int(color[0]), int(color[1]), int(color[2]));
                    break;
                }
            }
        }
    }
}

rasterizer::rasterizer(int width, int height) : width(width), height(height), poolIns(ThreadPool::getInstance()) {
    zBuffer = new atomic<float>[width * height];
}

void rasterizer::draw() {
    if (newWidth && newHeight) {
        width = newWidth;
        height = newHeight;
    }

    if (pCam)
        pCam->resize(width, height);

    static future<void> f;
    if (f.valid()) {
        f.get();
    }
    Matrix viewpointMatrix = {
            {width / 2., 0,           0, width / 2.},
            {0,          height / 2., 0, height / 2.},
            {0,          0,           1, 0},
            {0,          0,           0, 1}};

    vector<future<void>> v;
    for (auto &m: models) {
        Matrix mvpv, mv;
        if (pCam) {
            mvpv = viewpointMatrix * pCam->projectionMatrix * pCam->viewMatrix * m.modelMatrix;
            mv = pCam->viewMatrix * m.modelMatrix;
        } else {
            mvpv = viewpointMatrix * m.modelMatrix;
            mv = m.modelMatrix;
        }
        for (auto p: m.lines) {
            Triangle tri = {p.first, p.second, Point{}};
            tri = (mvpv * tri).normalize();
            drawLine(tri.getVertex(0), tri.getVertex(1));
        }

        for (Triangle ctri: m.tris) {
            Triangle tri = (mvpv * ctri).normalize();
            ctri = (mv * ctri).normalize();

            vec3 ab(ctri.getVertex(1)[0] - ctri.getVertex(0)[0], ctri.getVertex(1)[1] - ctri.getVertex(0)[1],
                    ctri.getVertex(1)[2] - ctri.getVertex(0)[2]);
            vec3 ac(ctri.getVertex(2)[0] - ctri.getVertex(0)[0], ctri.getVertex(2)[1] - ctri.getVertex(0)[1],
                    ctri.getVertex(2)[2] - ctri.getVertex(0)[2]);
            vec3 nor = (ab.cross(ac)).normalize();
            for (auto &vec: tri.normal) {
                if (vec) {
                    vec = (mv * vec.value()).normalize();
                } else {
                    vec = nor;
                }
            }
#ifndef _NOMULTITHREADS
            v.push_back(poolIns.assign([this, tri, ctri, &m] { drawTriangle(tri, ctri, m); }));
#else
            drawTriangle(tri, ctri, m);
#endif
        }
    }
    for (auto &fut: v) {
        fut.get();
    }
    f = async(&rasterizer::clearZBuffer, this);

    models.clear();
}

void rasterizer::addLight(vec3 pos) {
    lig.lightPos.push_back(pos);
}

void rasterizer::drawLine(Point begin, Point end, vec3 lineColor) {
    auto x1 = begin[0];
    auto y1 = begin[1];
    auto x2 = end[0];
    auto y2 = end[1];

    int x, y, dx, dy, dx1, dy1, px, py, xe, ye;
    double lineLen = 0;
    for (int i = 0; i < 2; i++)
        lineLen += pow(begin[i] - end[i], 2);
    lineLen = sqrt(lineLen);

    dx = int(x2 - x1);
    dy = int(y2 - y1);
    dx1 = int(fabs(dx));
    dy1 = int(fabs(dy));
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;

    auto drawPixel = [&]() {
        double curLen = pow(x - begin[0], 2) + pow(y - begin[1], 2);
        curLen = sqrt(curLen);
        double klen = curLen / lineLen;
        float z = -float((1 - klen) * begin[2] + klen * end[2]);
        z -= 0.0001f;
        if (z <= zBuffer[y * width + x]) {
            setPixel(x, y, int(lineColor[0]), int(lineColor[1]), int(lineColor[2]));
            zBuffer[y * width + x] = z;
        }
    };

    if (dy1 <= dx1) {
        if (dx >= 0) {
            x = (int) x1;
            y = (int) y1;
            xe = (int) x2;
        } else {
            x = (int) x2;
            y = (int) y2;
            xe = (int) x1;
        }
        drawPixel();
        for (int i = 0; x < xe; i++) {
            x = x + 1;
            if (px < 0) {
                px = px + 2 * dy1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    y = y + 1;
                } else {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            drawPixel();
        }
    } else {
        if (dy >= 0) {
            x = (int) x1;
            y = (int) y1;
            ye = (int) y2;
        } else {
            x = (int) x2;
            y = (int) y2;
            ye = (int) y1;
        }
        drawPixel();
        for (int i = 0; y < ye; i++) {
            y = y + 1;
            if (py <= 0) {
                py = py + 2 * dx1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    x = x + 1;
                } else {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            drawPixel();
        }
    }
}
