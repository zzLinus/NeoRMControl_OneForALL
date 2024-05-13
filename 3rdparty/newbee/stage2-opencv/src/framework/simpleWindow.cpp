#include "simpleWindow.h"
#include <thread>

using namespace std;
using namespace cv;

void simpleWindow::create(const char *name, int width, int height) {
    wndMat.create(height, width, CV_8UC3);
    nWidth = width;
    nHeight = height;

    windowName = name;

    show();
}

void simpleWindow::show() {
    imshow(windowName, wndMat);
    lastPress = (char)waitKey(1);
}

bool simpleWindow::shouldClose() {
    return getWindowProperty(windowName, WND_PROP_VISIBLE) < 1;
}

void simpleWindow::setPixel(int x, int y, int r, int g, int b) {
    y = wndMat.rows - y;
    y = min(y, wndMat.rows - 1);
    x = min(x, wndMat.cols - 1);
    x = max(x, 0);
    y = max(y, 0);
    wndMat.at<Vec3b>(y, x)[0] = b;
    wndMat.at<Vec3b>(y, x)[1] = g;
    wndMat.at<Vec3b>(y, x)[2] = r;
}

void simpleWindow::setPixel(int x, int y, vec3 color) {
    setPixel(x, y, (int) color[0], (int) color[1], (int) color[2]);
}

void simpleWindow::clear() {
    for (int i = 0; i < wndMat.rows; i++)
        for (int j = 0; j < wndMat.cols; j++)
            wndMat.at<Vec3b>(i, j) = Vec3b{(uchar) bkColor[0], (uchar) bkColor[1], (uchar) bkColor[2]};
}

void simpleWindow::setBkColor(int r, int g, int b) {
    bkColor = vec3(r, g, b);
    clear();
}

char simpleWindow::getKey() const {
    return lastPress;
}
