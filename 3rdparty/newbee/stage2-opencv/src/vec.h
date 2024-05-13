#pragma once
class vec3
{
private:
    double data[3];
    void setValue(double a, double b, double c);

public:
    vec3(double a, double b, double c) { setValue(a, b, c); }
    vec3() { setValue(0, 0, 0); }
    double &operator[](int i) { return data[i]; }
    vec3 operator+(vec3 v);
    void operator+=(vec3 v);
    vec3 operator/(double f);
    void operator/=(double f);
    vec3 operator-(vec3 v);
    vec3 operator*(double f);
    double operator*(vec3 v);
    double len();
    vec3 normalize();
    vec3 cross(vec3 v);
    void abs();
    void printData();
};
