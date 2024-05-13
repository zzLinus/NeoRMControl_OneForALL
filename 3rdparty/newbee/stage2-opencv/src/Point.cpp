#include "Point.h"
#include <stdio.h>
#include <assert.h>
using namespace std;


Point Point::normalize()
{
    Point res = *this;
    for (int i = 0; i < 4; i++)
        res[i] /= res[3];
    return res;
}


Point::Point()
{
    for (double &i : data)
        i = 0;
}


Point::Point(std::initializer_list<double> list)
{
    *this = list;
}


void Point::printData()
{
    for (double v : data)
        printf("%-8.3f", v);
    putchar('\n');
}


Point &Point::operator=(initializer_list<double> list)
{
    auto it = list.begin();
    for (int i = 0; i < list.size(); i++)
    {
        data[i] = it[i];
    }
    return *this;
}


double &Point::operator[](int idx)
{
    return data[idx];
}


Point Point::operator-(Point &p)
{
    Point res;
    for (int i = 0; i < 4; i++)
        res[i] = data[i] - p[i];
    return res;
}