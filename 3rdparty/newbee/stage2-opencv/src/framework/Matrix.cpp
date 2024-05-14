#include "Matrix.h"
#include "Triangle.h"
#include <iostream>

using namespace std;

void Matrix::init(initializer_list<initializer_list<double>> list) {
    for (double &i: data)
        i = 0;
    auto l = list.begin();
    for (int i = 0; i < 4 && l != list.end(); i++) {
        auto ll = l->begin();
        for (int j = 0; j < 4 && ll != l->end(); j++) {
            data[i * 4 + j] = *ll;
            ll++;
        }
        l++;
    }
}

void Matrix::printData() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%-8.3f", data[i * 4 + j]);
        }
        putchar('\n');
    }
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) {
    init(list);
}

Matrix::Matrix() {
    for (double &i: data) {
        i = 0;
    }
}

Matrix &Matrix::operator=(std::initializer_list<std::initializer_list<double>> list) {
    init(list);
    return *this;
}

Matrix Matrix::operator*(Matrix m) const {
    Matrix res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                res.data[i * 4 + j] += data[i * 4 + k] * m.data[k * 4 + j];
            }
        }
    }

    return res;
}

void Matrix::operator*=(Matrix m) {
    *this = *this * m;
}

Triangle Matrix::operator*(Triangle tri) const {
    Triangle res = tri;
    Matrix tmp;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            tmp[i][j] = tri.getVertex(j)[i];
        }
    }

    tmp = *this * tmp;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            res.getVertex(i)[j] = tmp[j][i];

    return res;
}

Matrix Matrix::operator*(double f) const {
    Matrix res = *this;
    for (double &v: res.data) {
        v *= f;
    }
    return res;
}

void Matrix::operator*=(double f) {
    for (double &v: data) {
        v *= f;
    }
}

Matrix Matrix::operator+(Matrix m) const {
    Matrix res = *this;
    for (int i = 0; i < 4 * 4; i++) {
        res.data[i] += m.data[i];
    }
    return res;
}

void Matrix::operator+=(Matrix m) {
    for (int i = 0; i < 4 * 4; i++) {
        data[i] += m.data[i];
    }
}

Point Matrix::operator*(Point m) const {
    Point res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.data[i] += data[i * 4 + j] * m.data[j];
        }
    }
    return res;
}

vec3 Matrix::operator*(vec3 v) const {
    vec3 res;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i] += data[i * 4 + j] * v[j];
        }
    }
    return res;
}

double *Matrix::operator[](int x) {
    return data + x * 4;
}

Matrix Matrix::trans() {
    Matrix res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i][j] = (*this)[j][i];
        }
    }
    return res;
}

Matrix Matrix::inverse() {
    Matrix res;
    int i, j, k;
    double W[4][8];
    double tem_1, tem_2, tem_3;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            if (j < 4) {
                W[i][j] = (*this)[i][j];
            }
            else {
                W[i][j] = (double) (j - 4 == i ? 1 : 0);
            }
        }
    }

    for (i = 0; i < 4; i++) {
        if ((W[i][i]) == 0) {
            for (j = i + 1; j < 4; j++) {
                if ((W[j][i]) != 0) {
                    break;
                }
            }
            if (j == 4) {
                throw logic_error("unable to inverse Martix");
            }
            for (k = 0; k < 2 * 4; k++) {
                W[i][k] += W[j][k];
            }
        }

        tem_1 = W[i][i];
        for (j = 0; j < 2 * 4; j++) {
            W[i][j] = W[i][j] / tem_1;
        }

        for (j = i + 1; j < 4; j++) {
            tem_2 = W[j][i];
            for (k = i; k < 2 * 4; k++) {
                W[j][k] = W[j][k] - tem_2 * W[i][k];
            }
        }
    }

    for (i = 4 - 1; i >= 0; i--) {
        for (j = i - 1; j >= 0; j--) {
            tem_3 = W[j][i];
            for (k = i; k < 2 * 4; k++) {
                W[j][k] = W[j][k] - tem_3 * W[i][k];
            }
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 4; j < 2 * 4; j++) {
            res[i][j - 4] = W[i][j];
        }
    }

    return res;
}

Matrix Matrix::identity() {
    static Matrix res;
    static bool bInit = false;
    if (bInit) {
        return res;
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i][j] = (i == j);
        }
    }
    return res;
}
