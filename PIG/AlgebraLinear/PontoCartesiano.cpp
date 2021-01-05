//
// Created by samuel on 1/5/21.
//

#include "PontoCartesiano.h"

PontoCartesiano::PontoCartesiano(double x, double y) {
    this->x = x;
    this->y = y;
}

PontoCartesiano PontoCartesiano::operator-(PontoCartesiano &p1) {
    return PontoCartesiano(this->x - p1.x, this->y - p1.y);
}

PontoCartesiano PontoCartesiano::operator*(PontoCartesiano &p) {
    return PontoCartesiano(this->x * p.x, this->y * p.y);
}
