#include "vec.h"
#include <iostream>
#include <cmath>

void vec3::setValue(double a, double b, double c)
{
    data[0] = a;
    data[1] = b;
    data[2] = c;
}

vec3 vec3::operator+(vec3 v)
{
    vec3 res;
    for (int i = 0; i < 3; i++)
        res[i] = data[i] + v[i];
    return res;
}

void vec3::operator+=(vec3 v)
{
    for (int i = 0; i < 3; i++)
        data[i] += v[i];
}

vec3 vec3::operator/(double f)
{
    vec3 res;
    for (int i = 0; i < 3; i++)
        res[i] = data[i] / f;
    return res;
}

void vec3::operator/=(double f)
{
    for (double &x : data)
        x /= f;
}

vec3 vec3::operator-(vec3 v)
{
    vec3 res;
    for (int i = 0; i < 3; i++)
        res[i] = data[i] - v[i];
    return res;
}

vec3 vec3::operator*(double f)
{
    vec3 res;
    for (int i = 0;i < 3;i++)
        res[i] = data[i] * f;
    return res;
}

double vec3::operator*(vec3 v)
{
    double res = 0;
    for (int i = 0; i < 3; i++)
        res += data[i] * v[i];
    return res;
}

double vec3::len()
{
    double len = 0;
    for (double &f : data)
        len += f * f;
    return sqrt(len);
}

vec3 vec3::normalize()
{
    vec3 res = *this;
    res /= len();
    return res;
}

vec3 vec3::cross(vec3 v)
{
    vec3 res;
    res[0] = data[1] * v[2] - v[1] * data[2];
    res[1] = data[2] * v[0] - v[2] * data[0];
    res[2] = data[0] * v[1] - v[0] * data[1];
    return res;
}

void vec3::abs()
{
    double f = 0;
    for (double g : data)
        f += g;
    if (f < 0)
    {
        for (double& g : data)
            g = -g;
    }
}

void vec3::printData()
{
    for (double f : data)
        printf("%-8.3f",f);
    printf("\n");
}
