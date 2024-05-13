#pragma once
#include <initializer_list>
#include "Point.h"
#include "Triangle.h"
class Matrix
{
private:
    void init(std::initializer_list<std::initializer_list<double>> list);
    double data[4 * 4];

public:
    Matrix(std::initializer_list<std::initializer_list<double>> list);
    Matrix();
    void printData();

    Matrix &operator=(std::initializer_list<std::initializer_list<double>> list);

    Matrix operator*(Matrix m);
    void operator*=(Matrix m);

    Triangle operator*(Triangle tri);

    Matrix operator*(double f);
    void operator*=(double f);

    Matrix operator+(Matrix m);
    void operator+=(Matrix m);

    double &getData(int row, int col) { return data[row * 4 + col]; }
    void setData(int row, int col, double newData) { data[row * 4 + col] = newData; }

    Point operator*(Point p);
    vec3 operator*(vec3 v);

    Matrix trans();

    Matrix inverse();

    static Matrix identity();
};