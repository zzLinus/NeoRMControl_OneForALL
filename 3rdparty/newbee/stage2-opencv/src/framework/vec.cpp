#include "vec.h"
#include <iostream>
#include <cmath>

void vec3::setValue(double a, double b, double c) {
    *this = {a, b, c};
}

vec3 vec3::operator+(vec3 v) const {
    vec3 res;
    for (int i = 0; i < 3; i++) {
        res[i] = (*this)[i] + v[i];
    }
    return res;
}

void vec3::operator+=(vec3 v) {
    for (int i = 0; i < 3; i++) {
        (*this)[i] += v[i];
    }
}

vec3 vec3::operator/(double f) const {
    vec3 res;
    for (int i = 0; i < 3; i++) {
        res[i] = (*this)[i] / f;
    }
    return res;
}

void vec3::operator/=(double f) {
    for (double &x: (*this)) {
        x /= f;
    }
}

vec3 vec3::operator-(vec3 v) const {
    vec3 res;
    for (int i = 0; i < 3; i++) {
        res[i] = (*this)[i] - v[i];
    }
    return res;
}

vec3 vec3::operator*(double f) const {
    vec3 res;
    for (int i = 0; i < 3; i++) {
        res[i] = (*this)[i] * f;
    }
    return res;
}

double vec3::operator*(vec3 v) {
    double res = 0;
    for (int i = 0; i < 3; i++)
        res += (*this)[i] * v[i];
    return res;
}

double vec3::len() {
    double len = 0;
    for (double &f: (*this)) {
        len += f * f;
    }
    return sqrt(len);
}

vec3 vec3::normalize() {
    vec3 res = *this;
    res /= len();
    return res;
}

vec3 vec3::cross(vec3 v) {
    vec3 res;
    res[0] = (*this)[1] * v[2] - v[1] * (*this)[2];
    res[1] = (*this)[2] * v[0] - v[2] * (*this)[0];
    res[2] = (*this)[0] * v[1] - v[0] * (*this)[1];
    return res;
}

void vec3::printData() const {
    for (double f: (*this))
        printf("%-8.3f", f);
    printf("\n");
}