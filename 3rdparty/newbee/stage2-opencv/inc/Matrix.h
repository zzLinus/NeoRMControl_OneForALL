#pragma once

#include <initializer_list>
#include "Point.h"
#include "Triangle.h"

class Matrix {
private:
    void init(std::initializer_list<std::initializer_list<double>> list);

    double data[4 * 4] = {};

public:
    Matrix(std::initializer_list<std::initializer_list<double>> list);

    Matrix();

    void printData();

    Matrix &operator=(std::initializer_list<std::initializer_list<double>> list);

    Matrix operator*(Matrix m) const;

    void operator*=(Matrix m);

    Triangle operator*(Triangle tri) const;

    Matrix operator*(double f) const;

    void operator*=(double f);

    Matrix operator+(Matrix m) const;

    void operator+=(Matrix m);

    double* operator[](int x);

    Point operator*(Point p) const;

    vec3 operator*(vec3 v) const;

    Matrix trans();

    Matrix inverse();

    static Matrix identity();
};