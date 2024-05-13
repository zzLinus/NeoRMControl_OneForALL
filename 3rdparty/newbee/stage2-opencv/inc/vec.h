#pragma once

#include <array>

class vec3 : public std::array<double, 3> {
private:
    using Inherited = std::array<double, 3>;
    using std::array<double, 3>::array;

    void setValue(double a, double b, double c);

public:
    vec3(double a, double b, double c) : Inherited{a, b, c} {}

    vec3() : Inherited{0, 0, 0} {}

    vec3 operator+(vec3 v) const;

    void operator+=(vec3 v);

    vec3 operator/(double f) const;

    void operator/=(double f);

    vec3 operator-(vec3 v) const;

    vec3 operator*(double f) const;

    double operator*(vec3 v);

    double len();

    vec3 normalize();

    vec3 cross(vec3 v);

    void printData() const;
};
