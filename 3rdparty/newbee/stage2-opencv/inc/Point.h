#pragma once

#include <initializer_list>
#include "vec.h"

class Point {
private:
    friend class Triangle;

    Point normalize();

public:
    double data[4] = {};

    Point();

    Point(std::initializer_list<double> list);

    void printData();

    Point &operator=(std::initializer_list<double> list);

    double &operator[](int idx);

    Point operator-(Point &p);

};

